//
// Created by ky3he4ik on 06/12/2019.
//

#include "ECDSA.h"
#include "picosha2.h"

ECDSA::ECDSA(const EllipticCurve *_curve) : curve(_curve) {}

std::pair<UINT, Point> ECDSA::generate_keypair(const Point &base_point) const {
    std::random_device random;
    UINT d = LongInt::get_random(LONG_INT_LEN, random) % curve->get_curve_order(base_point);
    Point Q = base_point * LongInt(LONG_INT_LEN, d);
    return {d, Q};
}

// Create a digital signature for the string message using a given curve with a distinguished
// point P which generates a prime order subgroup of size n.
std::pair<LongInt, UINT>
ECDSA::sign_msg(const std::string &message, const Point &base_point, UINT private_key, const Point &public_key) const {
    UINT curve_order = curve->get_curve_order(base_point);
    LongInt hash = hash_truncated(message, curve_order);
    UINT r = 0;
    LongInt res(LONG_INT_LEN);
    std::random_device random;
    // Choose a randomly selected secret point kP then compute r and s.
    while (r == 0 || res == 0) {
        UINT k = random();
        r = (base_point * k).get_x() % curve_order;
        res = (Point::inverse_mod(k, curve_order) * (hash + r * private_key)) % curve_order;
    }
    return {res, r};
}

// Hash the message and return integer whose binary representation is the the L leftmost bits
// of the hash value, where L is the bit length of num.
LongInt ECDSA::hash_truncated(const std::string &str, UINT num) {
    LongInt res = picosha2::hash256_long_int(str);
    UINT max_bits = LongInt(BITS_BASE, num).get_actual_bits();
    for (UINT i = 0; i < res.get_bits_count() - max_bits; i++)
        res.set_bit(i, false);
    return res.changeLen(max_bits);
}

bool ECDSA::verify_msg(const std::string &message, const Point &base_point, UINT private_key, const Point &public_key,
                       const std::pair<LongInt, UINT> &sign) const {
    if (base_point.get_inf() or !curve->contains(base_point))
        return false;
    UINT curve_order = curve->get_curve_order(base_point);
    if (!(base_point * curve_order).get_inf())
        return false;
    // Confirm that r and s are at least in the acceptable range.
    if (sign.second > curve_order || sign.first > curve_order)
        return false;

    LongInt hash = hash_truncated(message, curve_order);
    UINT w = Point::inverse_mod(sign.first, curve_order) % curve_order;
    UINT u1 = (hash * w) % curve_order, u2 = (sign.second * w) % curve_order;
    Point C1 = base_point * u1, C2 = public_key * u2;
    Point C = C1 + C2;
    return sign.first % curve_order == C.get_x() % curve_order;
}
