//
// Created by ky3he4ik on 12/12/2019.
//

#include "Curve_parameters.h"

#include <algorithm>

Curve_parameters::Curve_parameters() : curve_a(), curve_b(), base_point(Point::inf_point(nullptr)), curve_order() {}

Curve_parameters::Curve_parameters(const LongInt &_curve_a, const LongInt &_curve_b, const LongInt &_curve_p,
                                   const Point &_base_point, const LongInt &_curve_order) :
        curve_a(_curve_a), curve_b(_curve_b), curve_p(_curve_p), base_point(_base_point), curve_order(_curve_order) {}

Curve_parameters::Curve_parameters(const std::string &_curve_a, const std::string &_curve_b,
                                   const std::string &_curve_p, const std::string &_base_point,
                                   const std::string &_curve_order) : curve_a(_curve_a, 16), curve_b(_curve_a, 16),
                                                                      curve_p(_curve_p, 16),
                                                                      curve_order(_curve_order, 16) {
    auto curve = EllipticCurve(curve_a, curve_b, curve_p);
    base_point = Point(std::make_shared<EllipticCurve>(curve), _base_point);
    curve.set_curve_order(base_point, curve_order);
}

std::string Curve_parameters::serialize() const {
    return to_json().dump(4);
}

Curve_parameters Curve_parameters::deserialize(const std::string &data) {
    return deserialize(nlohmann::json::parse(data));
}

Curve_parameters Curve_parameters::deserialize(const nlohmann::json &data) {
    auto curve = std::make_shared<EllipticCurve>(EllipticCurve(LONG_INT_FROM_JSON(data["curve"]["a"]),
                                                               LONG_INT_FROM_JSON(data["curve"]["b"]),
                                                               LONG_INT_FROM_JSON(data["curve"]["p"])));
    auto base_point = Point(curve, LONG_INT_FROM_JSON(data["base_point"]["x"]),
                            LONG_INT_FROM_JSON(data["base_point"]["y"]));
    auto curve_order = LONG_INT_FROM_JSON(data["curve_order"]);
    curve->set_curve_order(base_point, curve_order);
    return {
            curve->get_a(),
            curve->get_b(),
            curve->get_p(),
            base_point,
            curve_order
    };
}

nlohmann::json Curve_parameters::to_json() const {
    nlohmann::json res;
    res["curve"]["a"] = curve_a.to_string(16);
    res["curve"]["b"] = curve_b.to_string(16);
    res["curve"]["p"] = curve_p.to_string(16);
    res["curve_order"] = curve_order.to_string(16);
    res["base_point"]["x"] = base_point.get_x().to_string(16);
    res["base_point"]["y"] = base_point.get_y().to_string(16);
    return res;
}

Curve_parameters Curve_parameters::curve_secp256k1() {
    auto j = R"({
  "curve": {
    "a": "0",
    "b": "7",
    "p": "FFFFFFFF_FFFFFFFF_FFFFFFFF_FFFFFFFF_FFFFFFFF_FFFFFFFF_FFFFFFFE_FFFFFC2F"
  },
  "base_point": {
    "x": "79BE667E F9DCBBAC 55A06295 CE870B07 029BFCDB 2DCE28D9 59F2815B 16F81798",
    "y": "483ADA77 26A3C465 5DA4FBFC 0E1108A8 FD17B448 A6855419 9C47D08F FB10D4B8"
  },
  "curve_order": "FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE BAAEDCE6 AF48A03B BFD25E8C D0364141"
}
)"_json;
    return deserialize(j);
}

EllipticCurve Curve_parameters::create_curve() const {
    return EllipticCurve(curve_a, curve_b, curve_p);
}

Curve_parameters::Curve_parameters(const EllipticCurve &curve,
                                   const Point &_base_point) : curve_a(curve.get_a()), curve_b(curve.get_b()),
                                                               curve_p(curve.get_p()), base_point(_base_point),
                                                               curve_order(curve.get_curve_order(_base_point)) {}

int Curve_parameters::bitLen() const {
    return std::max({curve_a.get_actual_bits(), curve_b.get_actual_bits(), curve_p.get_actual_bits(),
                     curve_order.get_actual_bits(), base_point.get_x().get_actual_bits(),
                     base_point.get_y().get_actual_bits()
                    });
}
