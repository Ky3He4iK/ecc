//
// Created by ky3he4ik on 06/12/2019.
//

#include "ECDSA.h"

#include <utility>
#include "picosha2.h"

ECDSA::ECDSA(std::shared_ptr<EllipticCurve> _curve, const Point &_base_point) : curve(std::move(
        _curve)) {
    LongInt curve_order = curve->fast_curve_order(_base_point);
    private_key = LongInt::get_random(curve_order.get_bits_count()) % curve_order;
    public_key = ECDSA_public_key(
            curve->get_a(),
            curve->get_b(),
            _base_point,
            curve_order,
            _base_point * private_key
    );
    std::cerr << curve->get_p().get_len() << '\n';
}

// Create a digital signature for the string message using a given curve with a distinguished
// point P which generates a prime order subgroup of size n.
std::pair<LongInt, LongInt>
ECDSA::sign_msg(const std::string &message) const {
    LongInt hash = picosha2::hash256_long_int(message) % public_key.curve_order;
    LongInt r(LONG_INT_LEN);
    LongInt s(LONG_INT_LEN);

    // Choose a randomly selected secret point kP then compute r and s.
    while (s == 0) {
        LongInt k = LongInt::get_random(public_key.curve_order.get_bits_count());
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
    ECDSA res(std::make_shared<EllipticCurve>(_curve), _curve.get_base_point());
    std::cerr << res.curve->get_p().get_len() << '\n';
    return res;
}

EllipticCurve ECDSA::get_curve() const {
    return *curve;
}

ECDSA_public_key ECDSA::get_public_key() const {
    return public_key;
}

ECDSA_public_key::ECDSA_public_key() : curve_a(0), curve_b(0),
                                       base_point(Point::inf_point(nullptr)), curve_order(0),
                                       random_point(Point::inf_point(nullptr)) {}

ECDSA_public_key::ECDSA_public_key(const LongInt &_curve_a, const LongInt &_curve_b, const Point &_base_point,
                                   const LongInt &_curve_order, const Point &_random_point) :
        curve_a(_curve_a), curve_b(_curve_b), base_point(_base_point), curve_order(_curve_order),
        random_point(_random_point) {}
