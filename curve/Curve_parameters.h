//
// Created by ky3he4ik on 12/12/2019.
//

#ifndef ECC_CURVE_PARAMETERS_H
#define ECC_CURVE_PARAMETERS_H

#include "LongInt.h"
#include "Point.h"
#include "../dependences/json.hpp"
#include "EllipticCurve.h"

#define LONG_INT_FROM_JSON(json_obj) (LongInt((json_obj).get<std::string>(), 16))

struct Curve_parameters {
    Curve_parameters();

    Curve_parameters(const LongInt &_curve_a, const LongInt &_curve_b, const LongInt &_curve_p,
                     const Point &_base_point, const LongInt &_curve_order);

    Curve_parameters(const EllipticCurve &_curve, const Point &_base_point);

    Curve_parameters &operator=(const Curve_parameters &other) = default;

    [[nodiscard]] std::string serialize() const;

    [[nodiscard]] nlohmann::json to_json() const;

    [[nodiscard]] EllipticCurve create_curve() const;

    int bitLen() const;

    static Curve_parameters deserialize(const std::string &data);

    static Curve_parameters deserialize(const nlohmann::json &data);

    static Curve_parameters curve_secp256k1();

    LongInt curve_a;
    LongInt curve_b;
    LongInt curve_p;
    Point base_point;
    LongInt curve_order;
};


#endif //ECC_CURVE_PARAMETERS_H
