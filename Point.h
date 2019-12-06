//
// Created by ky3he4ik on 22/11/19.
//

#ifndef ECC_POINT_H
#define ECC_POINT_H

#include <array>

#include "EllipticCurve.h"
#include "LongInt.h"

class EllipticCurve;

class Point {
private:
    LongInt x, y;
    bool is_inf = false;
    const EllipticCurve *curve;

    static std::array<LongInt, 3> extended_gcd(const LongInt &a, const LongInt &b);

    static std::array<UINT, 3> extended_gcd(UINT a, UINT b);

    static LongInt inverse_mod(const LongInt &k, const LongInt &p);

    static LongInt inverse_mod(UINT k, const LongInt &p);

    static LongInt inverse_mod(const LongInt &k, UINT p);

    static LongInt inverse_mod(UINT k, UINT p);

public:
    Point(const EllipticCurve *_curve, const LongInt &_x, const LongInt &_y);

    Point(const Point &other) = default;

    bool get_inf() const {
        return is_inf;
    }

    bool on_curve() const;

    static Point inf_point(const EllipticCurve *curve);

    const LongInt &get_x() const;

    const LongInt &get_y() const;

    std::string to_string() const;

    Point operator+(const Point &other) const;

    Point operator*(const LongInt &k) const;

    Point operator*(UINT k) const;

    Point operator-() const;

    Point operator-(const Point &other) const;

    Point operator/(const LongInt &k) const;

    Point operator/(UINT k) const;

    Point &operator=(const Point &other) = default;

    bool operator==(const Point &other) const;
};


#endif //ECC_POINT_H
