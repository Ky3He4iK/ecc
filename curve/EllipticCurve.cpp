//
// Created by ky3he4ik on 15/11/19.
//

#include "EllipticCurve.h"

EllipticCurve::EllipticCurve(const LongInt &_a, const LongInt &_b,
                             const LongInt &_p) : a(_a), b(_b), p(_p) {
    if (!is_valid())
        std::cerr << "Warning: this curve is invalid!\n";
}

// Compute the order of a point on the curve. Slow version. Todo: Schoof's algorithm
// Порядок точки
LongInt EllipticCurve::set_curve_order(const Point &point) {
    base_point = std::make_shared<Point>(point);
    Point q(point);
    curve_order = 1;
    //Add P to Q repeatedly until obtaining the identity (point at infinity).
    while (!q.get_inf()) {
        q = point + q;
        ++curve_order;
//        std::cerr << curve_order << ' ' << q.to_string() << '\n';
    }
    return curve_order;
}

std::string EllipticCurve::to_string() const {
    std::string res("y^2 = x^3");
    if (a != 0) {
        if (a > 0)
            res += std::string(" + ");
        res += a.to_string() + "*x";
    }
    if (b != 0) {
        if (b > 0)
            res += std::string(" + ");
        res += b.to_string();
    }
    if (p != 0)
        res += " over field " + p.to_string();
    return res;
}

bool EllipticCurve::contains(const Point &point) const {
    return point.get_inf() || (
            point.get_y().pow_and_mod(LongInt(2), p).abs() ==
            (point.get_x().pow_and_mod(LongInt(3), p).abs() +
             (point.get_x() * a).abs() + b.abs()) % p);
}

// for each X there is two Y on curve, odd and even.
LongInt EllipticCurve::get_y(const LongInt &x, bool is_odd) const {
    LongInt _a = (x.pow_and_mod(LongInt(3), p) + 7) % p; // a = ( ( ( (x^3) mod n ) ax^2 + bx + c ) mod n )
    LongInt y(_a.pow_and_mod((p + 1) / 4, p)); // y = a^{(n+1)//4} mod n
    if (y.is_odd() == is_odd)
        return y;
    return -y % p;  // invert Yint()
}


// for each X there is two Y on curve, odd and even.
Point EllipticCurve::get_point_by_x(const LongInt &x, bool is_odd) const {
    return Point(std::make_shared<EllipticCurve>(*this), x % p, get_y(x, is_odd));
}

LongInt EllipticCurve::get_curve_order(const Point &_base_point) const {
    if (base_point && _base_point == *base_point)
        return curve_order;
    if (p == 0)
        return LongInt();
    Point q(_base_point);
    LongInt order(1);
    //Add P to Q repeatedly until obtaining the identity (point at infinity).
    while (!q.get_inf()) {
        q = _base_point + q;
        ++order;
//        std::cerr << curve_order << ' ' << q.to_string() << '\n';
    }
    return order;
}

void EllipticCurve::set_curve_order(const Point &_base_point, const LongInt &_curve_order) {
    base_point = std::make_shared<Point>(_base_point);
    curve_order = _curve_order;
}

bool EllipticCurve::is_valid() const {
    return (a.pow_and_mod(LongInt(3), p) * 4 + b * b * 27) % p != 0;
}

LongInt EllipticCurve::get_saved_curve_order() const {
    return curve_order;
}
