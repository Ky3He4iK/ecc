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
#define LONG_INT_LEN 32

class EllipticCurve {
private:
    LongInt a, b, p;
    LongInt curve_order;
    std::shared_ptr<Point> base_point;

public:
    EllipticCurve(const LongInt &_a, const LongInt &_b, const LongInt &_p);

    LongInt set_curve_order(const Point &point);

    [[nodiscard]] bool contains(const Point &point) const;

    [[nodiscard]] const LongInt &get_p() const {
        return p;
    }

    [[nodiscard]] const LongInt &get_a() const {
        return a;
    }

    [[nodiscard]] const LongInt &get_b() const {
        return b;
    }

    [[nodiscard]] bool is_valid() const;

    [[nodiscard]] LongInt get_y(const LongInt &x, bool is_odd = false) const;

    [[nodiscard]] Point get_point_by_x(const LongInt &x, bool is_odd = false) const;

    [[nodiscard]] std::string to_string() const;

    [[nodiscard]] LongInt get_curve_order(const Point &base_point) const;

    [[nodiscard]] LongInt get_saved_curve_order() const;

    [[nodiscard]] LongInt fast_curve_order(const Point &base_point) const;

    void set_curve_order(const Point &base_point, const LongInt &order);

    [[nodiscard]] Point get_base_point() const;

    std::pair<LongInt, Point> generate_keypair(const Point &point);

    static EllipticCurve getSECP256k1();
};


#endif //ECC_ELLIPTICCURVE_H
