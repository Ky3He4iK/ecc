//
// Created by ky3he4ik on 22/11/19.
//

#ifndef ECC_POINT_H
#define ECC_POINT_H

#include <array>

#include "EllipticCurve.h"
#include "LongInt.h"

class Point {
private:
    const AS_INT *p;
    AS_INT x, y;
    const EllipticCurve *curve;

    static std::array<AS_INT, 3> extended_gcd(const AS_INT &a, const AS_INT &b);

    static AS_INT inverse_of(const AS_INT &n, const AS_INT &p);

public:
    Point(const EllipticCurve *_curve, const AS_INT &_x, const AS_INT &_y);
    Point(const Point &other) = default;

    Point operator+(const Point &other) const;
    Point &operator+=(const Point &other);

    Point operator*(const AS_INT &other) const;

};


#endif //ECC_POINT_H
