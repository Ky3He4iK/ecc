//
// Created by ky3he4ik on 06/12/2019.
//

#include "ECDSA.h"
#include "picosha2.h"

ECDSA::ECDSA(const EllipticCurve *_curve, const Point &_base_point) : curve(_curve), base_point(_base_point) {
    curve_order = curve->get_curve_order(base_point);
    std::random_device random;
    UINT d = LongInt::get_random(LONG_INT_LEN, random) % curve->get_curve_order(base_point);
    Point Q = base_point * LongInt(LONG_INT_LEN, d);
    keypair = {d, Q};
}

// Create a digital signature for the string message using a given curve with a distinguished
// point P which generates a prime order subgroup of size n.
std::pair<LongInt, UINT>
ECDSA::sign_msg(const std::string &message) const {
    LongInt hash = hash_truncated(message, curve_order);
    UINT r = 0;
    LongInt res(LONG_INT_LEN);
    std::random_device random;
    // Choose a randomly selected secret point kP then compute r and s.
    while (r == 0 || res == 0) {
        UINT k = random();
        r = (base_point * k).get_x() % curve_order;
        res = (Point::inverse_mod(k, curve_order) * (hash + r * keypair.first)) % curve_order;
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

bool ECDSA::verify_msg(const std::string &message, const std::pair<LongInt, UINT> &sign) const {
    if (base_point.get_inf() or !curve->contains(base_point))
        return false;
    if (!(base_point * curve_order).get_inf())
        return false;
    // Confirm that r and s are at least in the acceptable range.
    if (sign.second > curve_order || sign.first > curve_order)
        return false;

    LongInt hash = hash_truncated(message, curve_order);
    UINT w = Point::inverse_mod(sign.first, curve_order) % curve_order;
    UINT u1 = (hash * w) % curve_order, u2 = (sign.second * w) % curve_order;
    Point C1 = base_point * u1, C2 = keypair.second * u2;
    Point C = C1 + C2;
    return sign.first % curve_order == C.get_x() % curve_order;
}

ECDSA ECDSA::getSECP256k1() {
    EllipticCurve _curve = EllipticCurve::getSECP256k1();
    LongInt x(256, "79BE667E F9DCBBAC 55A06295 CE870B07 029BFCDB 2DCE28D9 59F2815B 16F81798", 16);
    LongInt y = _curve.get_y(x, false);
    ASSERT_(y == LongInt(256, "483ADA77 26A3C465 5DA4FBFC 0E1108A8 FD17B448 A6855419 9C47D08F FB10D4B8", 16),
            "Base point is wrong!");
    // 483ADA77 26A3C465 5DA4FBFC 0E1108A8 FD17B448 A6855419 9C47D08F FB10D4B8 even
    // ACF64D7C AB766D8E 4CE4329C 8D30AB15 2A0E59D8 24A399B2 F8230843 35A6D40E right
    // 5309B283 54899271 B31BCD63 72CF54EA D5F1A627 DB5C664D 07DCF7BB CA592821 odd
    return ECDSA(&_curve, Point(&_curve, x, y));
}
