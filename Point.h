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
    const LongInt *p;
    LongInt x, y;
    bool is_inf = false;
    const EllipticCurve *curve;

    static std::array<LongInt, 3> extended_gcd(const LongInt &a, const LongInt &b);

    static LongInt inverse_mod(const LongInt &k, const LongInt &p);

public:
    Point(const EllipticCurve *_curve, const LongInt &_x, const LongInt &_y);

    Point(const Point &other) = default;

    bool get_inf() const {
        return is_inf;
    }

    bool on_curve() const;

    static Point inf_point(const EllipticCurve *curve);

    Point operator+(const Point &other) const;

    Point &operator+=(const Point &other);

    Point operator*(const LongInt &k) const;
    Point operator-() const;
};


#endif //ECC_POINT_H
