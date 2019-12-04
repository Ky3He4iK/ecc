//
// Created by ky3he4ik on 15/11/19.
//

#ifndef ECC_ELLIPTICCURVE_H
#define ECC_ELLIPTICCURVE_H

#include <random>

#include "LongInt.h"
#include "Point.h"

class Point;

// Elliptic Curves over any Field
// general Weierstrass cubic y^2 = x^3 + ax^2 + bx + c over any field
// y**2 = (x**3 + a*x + b) % p
// (4 * a**3 + 27 * b**2) % p != 0
#define LONG_INT_LEN 64
class EllipticCurve {
private:
    LongInt a, b, c, p, exp;
    std::random_device random;
public:
    EllipticCurve(const LongInt &_a, const LongInt &_b, const LongInt &_c, const LongInt &_p, const LongInt &_exp);

    bool contains(const Point &point) const;

    const LongInt &get_p() const {
        return p;
    }

    const LongInt &get_a() const {
        return a;
    }

    const LongInt &get_b() const {
        return b;
    }

    const LongInt &get_c() const {
        return c;
    }

    const LongInt &get_exp() const {
        return exp;
    }

    std::string to_string() const;

    LongInt discriminant() const;

    UINT order(const Point &point) const;

    std::pair<UINT, Point> generate_keypair(const Point &point, UINT curve_order);

    static EllipticCurve getSECP256k1();
};


#endif //ECC_ELLIPTICCURVE_H
