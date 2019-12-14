//
// Created by ky3he4ik on 22/11/19.
//


#include "Point.h"

#include <stdexcept>

#define DO_ASSERT_ON_CURVE 1
#define ASSERT_ON_CURVE(point) { \
    if (DO_ASSERT_ON_CURVE) \
        ASSERT_(curve == nullptr || (point).curve->contains(point), "Point not on curve!"); \
}


std::array<LongInt, 3> Point::extended_gcd(const LongInt &a, const LongInt &b) {
    if (a == 0)
        return {b, LongInt(), LongInt(1)};

#define _POINT_GCD_STEP(v, old_v, tmp, q) { \
    (tmp) = (v); \
    (v) = (old_v) - (q) * (v); \
    (old_v) = (tmp); \
}

    LongInt s, old_s(1);
    LongInt t(1), old_t;
    LongInt r(b), old_r(a);
    while (r != 0) {
        LongInt q = old_r / r;
        LongInt tmp;
        _POINT_GCD_STEP(r, old_r, tmp, q)
        _POINT_GCD_STEP(s, old_s, tmp, q)
        _POINT_GCD_STEP(t, old_t, tmp, q)
    }
    return {old_r, old_s, old_t};
}

Point::Point(const std::shared_ptr<EllipticCurve> &_curve, const LongInt &_x, const LongInt &_y) : curve(_curve),
                                                                                                   x(_x), y(_y) {
    if (curve) {
        x %= curve->get_p();
        y %= curve->get_p();
    }
}


// Returns the result of this + other according to the group law.
Point Point::operator+(const Point &other) const {
    ASSERT_ON_CURVE(*this)
    ASSERT_ON_CURVE(other)
    if (is_inf)
        return other;
    if (other.is_inf)
        return *this;

    LongInt m;
    if (x == other.x) {
        if (y != other.y)
            return inf_point(curve);
        if (y == 0)
            return inf_point(curve);
        m = ((x * 3 * x + curve->get_a()) * inverse_mod(y << 1, curve->get_p())) % curve->get_p();
    } else
        m = ((other.y - y) * inverse_mod((other.x - x) % curve->get_p(), curve->get_p())) % curve->get_p();
    LongInt xr = (m * m - x - other.x) % curve->get_p();
//    Point res(curve, xr, (curve->get_p() * 2 - y - m * (xr - x)) % curve->get_p());
    Point res(curve, xr, (m * (x - xr) - y) % curve->get_p());

    ASSERT_ON_CURVE(res)
    return res;
}


// Returns k * point computed using the double and point_add algorithm.
Point Point::operator*(const LongInt &k) const {
    if (get_inf() || k == 0)
        return *this;
    ASSERT_ON_CURVE(*this)
    Point res = Point::inf_point(curve);
    if (k < 0)
        return (-*this) * (-k);
    else {
        Point pow = Point(*this);
        LongInt tmp(k);
        while (tmp != 0) {
            if ((tmp & 1) == 1)
                res = res + pow;
            pow = pow + pow;
            tmp >>= 1;
        }
    }
    ASSERT_ON_CURVE(res)
    return res;
}

// Returns k * point computed using the double and point_add algorithm.
Point Point::operator*(int k) const {
    if (get_inf() || k == 0)
        return *this;
    ASSERT_ON_CURVE(*this)
    Point res = Point::inf_point(curve);
    if (k < 0)
        return (-*this) * (-k);
    else {
        Point pow = Point(*this);
        LongInt tmp(k);
        while (tmp != 0) {
            if ((tmp & 1) == 1)
                res = res + pow;
            pow = pow + pow;
            tmp >>= 1;
        }
    }
    ASSERT_ON_CURVE(res)
    return res;
}

/*
 *  Returns the inverse of k modulo p.
    This function returns the only integer x such that (x * k) % p == 1.
    k must be non-zero and p must be a prime.
 * */
Point Point::inf_point(const std::shared_ptr<EllipticCurve> &curve) {
    Point res(curve, LongInt(), LongInt());
    res.is_inf = true;
    return res;
}


// Returns the inverse of k modulo p.
//    This function returns the only integer x such that (x * k) % p == 1.
//    k must be non-zero and p must be a prime
LongInt Point::inverse_mod(const LongInt &k, const LongInt &p) {
    if (k == 0)
        throw std::invalid_argument("Division by zero");
    if (k < 0)
        return p - inverse_mod(-k, p);

    auto gcd_x_y = extended_gcd(k, p);
    ASSERT_(gcd_x_y[0] == 1, "GCD is not 1")
    ASSERT_((k * gcd_x_y[1]) % p == 1, "(k * x[1]) % p != 1")

    return gcd_x_y[1] % p;
}

// Returns -point
Point Point::operator-() const {
    ASSERT_ON_CURVE(*this)
    if (is_inf)
        return Point::inf_point(curve);
    if (curve) {
        Point res = {curve, x, (curve->get_p() - y) % curve->get_p()};
        ASSERT_ON_CURVE(res)
        return res;
    } else {
        Point res = {curve, x, -y};
        return res;
    }
}


Point Point::operator-(const Point &other) const {
    ASSERT_ON_CURVE(*this)
    if (other.is_inf)
        return *this;
    return *this + (-other);
}

// Returns true if the given point lies on the elliptic curve
bool Point::on_curve() const {
    return is_inf || curve == nullptr ||
           ((y * y - x * x * x - curve->get_a() * x - curve->get_b()) % curve->get_p() == 0);
}

std::string Point::to_string() const {
    if (is_inf)
        return "Infinity";
    return std::string("(") + x.to_string() + "; " + y.to_string() + ")";
}

bool Point::operator==(const Point &other) const {
    if (is_inf || other.is_inf)
        return is_inf == other.is_inf;
    return x == other.x && y == other.y;
}

const LongInt &Point::get_x() const {
    return x;
}

const LongInt &Point::get_y() const {
    return y;
}

Point Point::operator/(const LongInt &k) const {
    return *this *inverse_mod(LongInt(k), curve->get_saved_curve_order());
}

Point Point::operator/(int k) const {
    return *this * inverse_mod(LongInt(k), curve->get_saved_curve_order());
}

Point::Point() : is_inf(true), curve(nullptr) {}

Point &Point::operator=(const Point &other) {
    if (this == &other)
        return *this;
    x = other.x;
    y = other.y;
    is_inf = other.is_inf;
    curve = other.curve;
    return *this;
}

Point::Point(const std::shared_ptr<EllipticCurve> &_curve, const std::string &hex_form): curve(_curve) {
    int start = hex_form[0] != '0';
    switch (hex_form[start]) {
        case '2':
            x = LongInt(hex_form.substr(2), 16);
            y = curve->get_y(x, false);
            break;
        case '3':
            x = LongInt(hex_form.substr(2), 16);
            y = curve->get_y(x, true);
            break;
        case '4':
            x = LongInt(hex_form.substr(2, hex_form.size() / 2 + 1), 16);
            y = LongInt(hex_form.substr(hex_form.size() / 2 + 1), 16);
            break;
        default:
            x = 0;
            y = 0;
            is_inf = true;
            return;
    }
}
