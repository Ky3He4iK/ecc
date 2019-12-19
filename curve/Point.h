//
// Created by ky3he4ik on 22/11/19.
//

#ifndef ECC_POINT_H
#define ECC_POINT_H

#include <array>
#include <string>
#include <memory>

#include "EllipticCurve.h"
#include "LongInt.h"
#include "../dependences/json.hpp"

class EllipticCurve;

class Point {
private:
    LongInt x, y;
    bool is_inf = false;
    std::shared_ptr<EllipticCurve> curve;

public:
    Point(const std::shared_ptr<EllipticCurve> &_curve, const LongInt &_x, const LongInt &_y);

    // hex_form is string like 0x hhhh hhhh .... hhhh
    // where x = 2 - compressed even
    //           3 - compressed odd
    //           4 - uncompressed form (both x and y)
    // h - and hex char
    Point(const std::shared_ptr<EllipticCurve> &_curve, const std::string &hex_form);

    Point(const Point &other) = default;

    Point();

    static Point deserialize(const nlohmann::json &json, const std::shared_ptr<EllipticCurve> &_curve);

    static std::array<LongInt, 3> extended_gcd(const LongInt &a, const LongInt &b);

    static LongInt inverse_mod(const LongInt &k, const LongInt &p);

    bool get_inf() const {
        return is_inf;
    }

    static Point inf_point(const std::shared_ptr<EllipticCurve> &curve);

    std::shared_ptr<EllipticCurve> get_curve() const;

    const LongInt &get_x() const;

    const LongInt &get_y() const;

    std::string to_human_string() const;

    std::string to_string(int bit_len = -1) const;

    Point operator+(const Point &other) const;

    Point operator*(const LongInt &k) const;

    Point operator*(int k) const;

    Point operator-() const;

    Point operator-(const Point &other) const;

    Point operator/(const LongInt &k) const;

    Point operator/(int k) const;

    Point &operator=(const Point &other);

    bool operator==(const Point &other) const;
};

#endif //ECC_POINT_H
