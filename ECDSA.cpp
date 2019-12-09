//
// Created by ky3he4ik on 06/12/2019.
//

#include "ECDSA.h"
#include "picosha2.h"

ECDSA::ECDSA(const EllipticCurve *_curve, const Point &_base_point) : curve(_curve) {
    LongInt curve_order = curve->fast_curve_order(_base_point);
    std::random_device random;
    private_key = LongInt::get_random(LONG_INT_LEN, random) % curve_order;
    public_key = ECDSA_public_key(
            curve->get_a(),
            curve->get_b(),
            _base_point,
            curve_order,
            _base_point * private_key
    );
}

// Create a digital signature for the string message using a given curve with a distinguished
// point P which generates a prime order subgroup of size n.
std::pair<LongInt, LongInt>
ECDSA::sign_msg(const std::string &message) const {
    LongInt hash = picosha2::hash256_long_int(message) % public_key.curve_order;
    LongInt r(LONG_INT_LEN);
    LongInt s(LONG_INT_LEN);
    std::random_device random;
    // Choose a randomly selected secret point kP then compute r and s.
    while (s == 0) {
        UINT k = random();
        r = (public_key.base_point * k).get_x() % public_key.curve_order;
        if (r == 0)
            continue;
        s = (Point::inverse_mod(k, public_key.curve_order) * (hash + r * private_key)) % public_key.curve_order;
    }
    return {r, s};
}

bool ECDSA::verify_msg(const std::string &message, const std::pair<LongInt, LongInt> &sign) const {
    if (public_key.base_point.get_inf() or !curve->contains(public_key.base_point))
        return false;
    if (!(public_key.base_point * public_key.curve_order).get_inf())
        return false;
    // Confirm that r and s are at least in the acceptable range.
    if (sign.first > public_key.curve_order || sign.second > public_key.curve_order)
        return false;

    LongInt hash = picosha2::hash256_long_int(message) % public_key.curve_order;
    LongInt w = Point::inverse_mod(sign.second, public_key.curve_order);
    LongInt u1 = (hash * w) % public_key.curve_order;
    LongInt u2 = (sign.first * w) % public_key.curve_order;
    Point C1 = public_key.base_point * u1;
    Point C2 = public_key.random_point * u2;
    Point C = C1 + C2;
    return sign.first % public_key.curve_order == C.get_x() % public_key.curve_order;
}

ECDSA ECDSA::getSECP256k1() {
    EllipticCurve _curve = EllipticCurve::getSECP256k1();
    LongInt x(256, "79BE667E F9DCBBAC 55A06295 CE870B07 029BFCDB 2DCE28D9 59F2815B 16F81798", 16);

    Point point = _curve.get_point_by_x(x, false);
    _curve.set_curve_order(point,
                           LongInt(256, "FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE BAAEDCE6 AF48A03B BFD25E8C D0364141"));
    ASSERT_(point.get_y() ==
            LongInt(256, "483ADA77 26A3C465 5DA4FBFC 0E1108A8 FD17B448 A6855419 9C47D08F FB10D4B8", 16),
            "Base point is wrong!");
    return ECDSA(&_curve, point);
}

ECDSA_public_key::ECDSA_public_key() : curve_a(0), curve_b(0),
                                       base_point(Point::inf_point(nullptr)), curve_order(0),
                                       random_point(Point::inf_point(nullptr)) {}

ECDSA_public_key::ECDSA_public_key(const LongInt &_curve_a, const LongInt &_curve_b, const Point &_base_point,
                                   const LongInt &_curve_order, const Point &_random_point) :
        curve_a(_curve_a), curve_b(_curve_b), base_point(_base_point), curve_order(_curve_order),
        random_point(_random_point) {}
