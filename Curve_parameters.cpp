//
// Created by ky3he4ik on 12/12/2019.
//

#include "Curve_parameters.h"


Curve_parameters::Curve_parameters() : curve_a(), curve_b(), base_point(Point::inf_point(nullptr)), curve_order() {}

Curve_parameters::Curve_parameters(const LongInt &_curve_a, const LongInt &_curve_b, const LongInt &_curve_p,
                                   const Point &_base_point, const LongInt &_curve_order) :
        curve_a(_curve_a), curve_b(_curve_b), curve_p(_curve_p), base_point(_base_point), curve_order(_curve_order) {}

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
    return {
            {"curve",       {
                                    {"a", curve_a.to_string(16)},
                                    {"b", curve_b.to_string(16)},
                                    {"p", curve_p.to_string(16)},
                            }},
            {"base_point",  {
                                    {"x", base_point.get_x().to_string(16)},
                                    {"y", base_point.get_y().to_string(16)},
                            }},
            {"curve_order", curve_order.to_string(16)}
    };
}

Curve_parameters Curve_parameters::curve_secp256k1() {
    EllipticCurve curve(LongInt(), LongInt(7),
                        LongInt("0xFFFFFFFF_FFFFFFFF_FFFFFFFF_FFFFFFFF_FFFFFFFF_FFFFFFFF_FFFFFFFE_FFFFFC2F", 16));
    return {
            curve.get_a(),
            curve.get_b(),
            curve.get_p(),
            curve.get_point_by_x(
                    LongInt("79BE667E F9DCBBAC 55A06295 CE870B07 029BFCDB 2DCE28D9 59F2815B 16F81798", 16)),
            LongInt("FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE BAAEDCE6 AF48A03B BFD25E8C D0364141", 16)
    };
}

EllipticCurve Curve_parameters::create_curve() const {
    return EllipticCurve(curve_a, curve_b, curve_p);
}

Curve_parameters::Curve_parameters(const EllipticCurve &curve,
                                   const Point &_base_point) : curve_a(curve.get_a()), curve_b(curve.get_b()),
                                                               curve_p(curve.get_p()), base_point(_base_point),
                                                               curve_order(curve.get_curve_order(_base_point)) {}
