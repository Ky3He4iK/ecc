//
// Created by ky3he4ik on 06/12/2019.
//

#include "ECDSA.h"

#include <utility>
#include "dependences/picosha2.h"

#define LONG_INT_FROM_JSON(json_obj) (LongInt((json_obj).get<std::string>(), 16))

ECDSA::ECDSA(const std::shared_ptr<EllipticCurve> &curve, const Point &_base_point) {
    LongInt curve_order = curve->fast_curve_order(_base_point);
    private_key = LongInt::get_random(curve_order.get_bits_count()) % curve_order;
    public_key = ECDSA_public_key(
            curve->get_a(),
            curve->get_b(),
            curve->get_p(),
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

bool ECDSA::verify_msg(const std::string &message, const std::pair<LongInt, LongInt> &sign,
                       const ECDSA_public_key &public_key) {
    if (public_key.base_point.get_inf() || !(public_key.base_point * public_key.curve_order).get_inf())
        return false;
    // Confirm that r and s are at least in the acceptable range.
    if (sign.first > public_key.curve_order || sign.second > public_key.curve_order)
        return false;

    LongInt hash = picosha2::hash256_long_int(message) % public_key.curve_order;
    LongInt w = Point::inverse_mod(sign.second, public_key.curve_order);
    LongInt u1 = (hash * w) % public_key.curve_order;
    LongInt u2 = (sign.first * w) % public_key.curve_order;
    Point C = public_key.base_point * u1 + public_key.random_point * u2;
    return sign.first % public_key.curve_order == C.get_x() % public_key.curve_order;
}

ECDSA ECDSA::getSECP256k1() {
    EllipticCurve _curve = EllipticCurve::getSECP256k1();
    ECDSA res(std::make_shared<EllipticCurve>(_curve), _curve.get_base_point());
    return res;
}

ECDSA_public_key ECDSA::get_public_key() const {
    return public_key;
}

ECDSA::ECDSA(const ECDSA_private_key &_private_key, const ECDSA_public_key &_public_key) : private_key(_private_key),
                                                                                           public_key(_public_key) {}

ECDSA_private_key ECDSA::get_private_key() const {
    return private_key;
}

std::string ECDSA::serialize() const {
    nlohmann::json res = {
            {"public_key",  public_key.to_json()},
            {"private_key", private_key.to_string(16)}
    };
    return res.dump(4);
}

ECDSA ECDSA::deserialize(const std::string &data) {
    auto ecdsa = nlohmann::json::parse(data);
    auto public_key = ECDSA_public_key::deserialize(ecdsa["public_key"]);
    auto private_key = ECDSA_private_key(ecdsa["private_key"].get<std::string>(), 16);
    return ECDSA(private_key, public_key);
}

ECDSA_public_key::ECDSA_public_key() : curve_a(0), curve_b(0),
                                       base_point(Point::inf_point(nullptr)), curve_order(0),
                                       random_point(Point::inf_point(nullptr)) {}

ECDSA_public_key::ECDSA_public_key(const LongInt &_curve_a, const LongInt &_curve_b, const LongInt &_curve_p,
                                   const Point &_base_point, const LongInt &_curve_order, const Point &_random_point) :
        curve_a(_curve_a), curve_b(_curve_b), curve_p(_curve_p), base_point(_base_point), curve_order(_curve_order),
        random_point(_random_point) {}

std::string ECDSA_public_key::serialize() const {
    return to_json().dump(4);
}

ECDSA_public_key ECDSA_public_key::deserialize(const std::string &data) {
    return deserialize(nlohmann::json::parse(data));
}

ECDSA_public_key ECDSA_public_key::deserialize(const nlohmann::json &data) {
    auto curve = EllipticCurve(LONG_INT_FROM_JSON(data["curve"]["a"]),
                               LONG_INT_FROM_JSON(data["curve"]["b"]),
                               LONG_INT_FROM_JSON(data["curve"]["p"]));
    auto base_point = Point(std::make_shared<EllipticCurve>(curve), LONG_INT_FROM_JSON(data["base_point"]["x"]),
                            LONG_INT_FROM_JSON(data["base_point"]["y"]));
    auto random_point = Point(std::make_shared<EllipticCurve>(curve),
                              LONG_INT_FROM_JSON(data["random_point"]["x"]),
                              LONG_INT_FROM_JSON(data["random_point"]["y"]));
    auto curve_order = LONG_INT_FROM_JSON(data["curve_order"]);
    curve.set_curve_order(base_point, curve_order);
    return {
            curve.get_a(),
            curve.get_b(),
            curve.get_p(),
            base_point,
            curve_order,
            random_point
    };
}

nlohmann::json ECDSA_public_key::to_json() const {
    return {
            {"curve",        {
                                     {"a", curve_a.to_string(16)},
                                     {"b", curve_b.to_string(16)},
                                     {"p", curve_p.to_string(16)},
                             }},
            {"base_point",   {
                                     {"x", base_point.get_x().to_string(16)},
                                     {"y", base_point.get_y().to_string(16)},
                             }},
            {"random_point", {
                                     {"x", random_point.get_x().to_string(16)},
                                     {"y", random_point.get_y().to_string(16)},
                             }},
            {"curve_order",  curve_order.to_string(16)}
    };
}
