//
// Created by ky3he4ik on 06/12/2019.
//

#include "ECDSA.h"

#include <utility>
#include "dependences/picosha2.h"
#include "Curve_parameters.h"

ECDSA::ECDSA(const std::shared_ptr<EllipticCurve> &curve, const Point &_base_point) {
    LongInt curve_order = curve->fast_curve_order(_base_point);
    while (private_key == 0 || private_key >= curve_order)
        private_key = LongInt::get_random(curve_order.get_actual_bits()) % curve_order;
    public_key = ECDSA_public_key(_base_point * private_key);
    parameters = Curve_parameters(
            curve->get_a(),
            curve->get_b(),
            curve->get_p(),
            _base_point,
            curve_order
    );
}

ECDSA::ECDSA(const Curve_parameters &_parameters) : parameters(_parameters) {
    while (private_key == 0 || private_key >= parameters.curve_order)
        private_key = LongInt::get_random(parameters.curve_order.get_actual_bits()) % parameters.curve_order;
    public_key = ECDSA_public_key(parameters.base_point * private_key);
}

// Create a digital signature for the string message using a given curve with a distinguished
// point P which generates a prime order subgroup of size n.
std::pair<LongInt, LongInt>
ECDSA::sign_msg(const std::string &message) const {
    LongInt hash = picosha2::hash256_long_int(message) % parameters.curve_order;
    LongInt r;
    LongInt s;

    // Choose a randomly selected secret point dP then compute r and s.
    while (s == 0) {
        LongInt d;
        while (d == 0 || d >= parameters.curve_order)
            d = LongInt::get_random(parameters.curve_order.get_actual_bits()) % parameters.curve_order;
        r = (parameters.base_point * d).get_x() % parameters.curve_order;
        if (r == 0)
            continue;
        s = (Point::inverse_mod(d, parameters.curve_order) * (hash + r * private_key)) % parameters.curve_order;
    }
    return {r, s};
}

bool ECDSA::verify_msg(const std::string &message, const std::pair<LongInt, LongInt> &sign,
                       const ECDSA_public_key &public_key, const Curve_parameters &parameters) {
    if (parameters.base_point.get_inf() || !(parameters.base_point * parameters.curve_order).get_inf())
        return false;
    // Confirm that r and s are at least in the acceptable range.
    if (sign.first > parameters.curve_order || sign.second > parameters.curve_order)
        return false;

    LongInt hash = picosha2::hash256_long_int(message) % parameters.curve_order;
    LongInt w = Point::inverse_mod(sign.second, parameters.curve_order);
    LongInt u1 = (hash * w) % parameters.curve_order;
    LongInt u2 = (sign.first * w) % parameters.curve_order;
    Point C = parameters.base_point * u1 + public_key * u2;
    return sign.first % parameters.curve_order == C.get_x() % parameters.curve_order;
}

ECDSA ECDSA::getSECP256k1() {
    auto parameters = Curve_parameters::curve_secp256k1();
    return ECDSA(parameters);
}

ECDSA_public_key ECDSA::get_public_key() const {
    return public_key;
}

ECDSA::ECDSA(const ECDSA_private_key &_private_key, const ECDSA_public_key &_public_key,
             const Curve_parameters &_parameters) : private_key(_private_key), public_key(_public_key),
                                                    parameters(_parameters) {}

ECDSA_private_key ECDSA::get_private_key() const {
    return private_key;
}

Curve_parameters ECDSA::get_parameters() const {
    return parameters;
}

std::string ECDSA::serialize() const {
    nlohmann::json res = {
            {"public_key",  {
                                    {"x", public_key.get_x().to_string(16)},
                                    {"y", public_key.get_y().to_string(16)}
                            }},
            {"private_key", private_key.to_string(16)},
            {"parameters",  parameters.to_json()}
    };
    return res.dump(4);
}

ECDSA ECDSA::deserialize(const std::string &data) {
    auto ecdsa = nlohmann::json::parse(data);
    auto parametes = Curve_parameters::deserialize(ecdsa["parameters"]);
    auto public_key = Point(std::make_shared<EllipticCurve>(parametes.create_curve()),
                            LONG_INT_FROM_JSON(ecdsa["public_key"]["x"]),
                            LONG_INT_FROM_JSON(ecdsa["public_key"]["y"]));
    auto private_key = ECDSA_private_key(ecdsa["private_key"].get<std::string>(), 16);
    return ECDSA(private_key, public_key, parametes);
}
