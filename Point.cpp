//
// Created by ky3he4ik on 22/11/19.
//

#include "Point.h"

#include <stdexcept>
#include <exception>
#include <utility>

#define DO_ASSERT_ON_CURVE 0
#define ASSERT_ON_CURVE(point) { \
    if (DO_ASSERT_ON_CURVE) \
        ASSERT_(curve == nullptr || (point).curve->contains(point), "Point not on curve!"); \
}

#define _POINT_GCD_STEP(v, old_v, tmp, q) { \
    (tmp) = (v); \
    (v) = (old_v) - (q) * (v); \
    (old_v) = (tmp); \
}

std::array<LongInt, 3> Point::extended_gcd(const LongInt &a, const LongInt &b) {
    if (a == 0)
        return {b, LongInt(LONG_INT_LEN), LongInt(LongInt(LONG_INT_LEN, 1))};

    LongInt s(a.get_bits_count(), UINT_0), old_s(a.get_bits_count(), 1);
    LongInt t(a.get_bits_count(), 1), old_t(a.get_bits_count(), UINT_0);
    LongInt r(b), old_r(a);
    while (r != UINT_0) {
//        std::cerr << old_s.to_string() << ' ' << s.to_string() << '\n' << old_t.to_string() << ' ' << t.to_string()
//                  << '\n' << old_r.to_string() << ' ' << r.to_string() << "\n\n";
        LongInt q = old_r / r;
        LongInt tmp = r;
        _POINT_GCD_STEP(r, old_r, tmp, q)
        _POINT_GCD_STEP(s, old_s, tmp, q)
        _POINT_GCD_STEP(t, old_t, tmp, q)
    }
    return {old_r, old_s, old_t};
}

std::array<UINT, 3> Point::extended_gcd(UINT a, UINT b) {
    if (a == 0)
        return {b, 0, 1};

    UINT s(0), old_s(1);
    UINT t(1), old_t(UINT_0);
    UINT r(b), old_r(a);
    while (r != UINT_0) {
        UINT q = old_r / r;
        UINT tmp;
        _POINT_GCD_STEP(r, old_r, tmp, q)
        _POINT_GCD_STEP(s, old_s, tmp, q)
        _POINT_GCD_STEP(t, old_t, tmp, q)
    }
    return {old_r, old_s, old_t};
}

Point::Point(std::shared_ptr<EllipticCurve> _curve, const LongInt &_x, const LongInt &_y) : curve(std::move(_curve)),
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

    LongInt m(x);
    if (x == other.x) {
        if (y != other.y)
            return inf_point(curve);
        if (y == 0)
            return inf_point(curve);
        m = ((3 * x * x + curve->get_a() * x + curve->get_b()) * inverse_mod(y << 1, curve->get_p())) % curve->get_p();
    } else
        m = ((other.y - y) * inverse_mod((other.x - x) % curve->get_p(), curve->get_p())) % curve->get_p();
    LongInt xr = (m * m - x - other.x - curve->get_a()) % curve->get_p();
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
    if (k % curve->get_p() == UINT_0) {
        res.is_inf = true;
    } else if (k < UINT_0)
        return (-*this) * (-k);
    else {
        Point pow = Point(*this);
        UINT actual_bits = k.get_actual_bits();
        UINT min = k.get_bits_count() - actual_bits;
        for (UINT i = k.get_bits_count() - 1; i != 0 && i >= min; i--) {
            if (k.get_bit(i))
                res = res + pow;
            pow = pow + pow;
        }
        if (k.get_bit(0))
            res = res + pow;
    }
    ASSERT_ON_CURVE(res)
    return res;
}

Point Point::operator*(UINT k) const {
    if (get_inf() || k == 0)
        return *this;
    ASSERT_ON_CURVE(*this)
    Point res = Point::inf_point(curve);
    if (k % curve->get_p() == UINT_0) {
        res.is_inf = true;
    } else if (k < UINT_0)
        return (-*this) * (-k);
    else {
        Point pow = Point(*this);
        while (k != 0) {
            if (k & 1)
                res = res + pow;
            pow = pow + pow;
            k >>= 1;
        }
//        if (k & 1)
//            res = res + pow;
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
    Point res(curve, LongInt(0), LongInt(0));
    res.is_inf = true;
    return res;
}


// Returns the inverse of k modulo p.
//    This function returns the only integer x such that (x * k) % p == 1.
//    k must be non-zero and p must be a prime
LongInt Point::inverse_mod(const LongInt &k, const LongInt &p) {
    if (k == UINT_0)
        throw std::invalid_argument("Division by zero");
    if (k < UINT_0)
        return p - inverse_mod(-k, p);

    auto gcd_x_y = extended_gcd(k, p);
    ASSERT_(gcd_x_y[0] == 1, "GCD is not 1")
    ASSERT_((k.changeLen(k.get_bits_count() << 1) * gcd_x_y[1]) % p == 1, "(k * x[1]) % p != 1")

    return gcd_x_y[1] % p;
}

LongInt Point::inverse_mod(UINT k, const LongInt &p) {
    if (k == UINT_0)
        throw std::invalid_argument("Division by zero");
    if (p > UINT_MAX)
        return inverse_mod(LongInt(LONG_INT_LEN, k), p);
    auto gcd_x_y = extended_gcd(k, p.last_item());
    bool sign = gcd_x_y[1] > (UINT_MAX >> 1);
    if (sign)
        gcd_x_y[1] = -gcd_x_y[1];
    LongInt res(LONG_INT_LEN, gcd_x_y[1] % p.last_item());
    if (sign)
        res = -res;
    ASSERT_(gcd_x_y[0] == 1, "GCD is not 1")
    ASSERT_((k * res) % p.last_item() == 1, "(k * x[1]) % p != 1")
    return res;
}

LongInt Point::inverse_mod(const LongInt &k, UINT p) {
    if (k == UINT_0)
        throw std::invalid_argument("Division by zero");
    if (k < UINT_0)
        return p - inverse_mod(-k, p);
    auto gcd_x_y = extended_gcd(k % p, p);
    bool sign = gcd_x_y[1] > (UINT_MAX >> 1);
    if (sign)
        gcd_x_y[1] = -gcd_x_y[1];
    LongInt res(LONG_INT_LEN, gcd_x_y[1] % p);
    if (sign)
        res = -res;
    ASSERT_(gcd_x_y[0] == 1, "GCD is not 1")
    ASSERT_((k * res) % p == 1, "(k * x[1]) % p != 1")
    return res;
}

LongInt Point::inverse_mod(UINT k, UINT p) {
    if (k == UINT_0)
        throw std::invalid_argument("Division by zero");
    auto gcd_x_y = extended_gcd(k, p);
    bool sign = gcd_x_y[1] > (UINT_MAX >> 1);
    if (sign)
        gcd_x_y[1] = -gcd_x_y[1];
    LongInt res(LONG_INT_LEN, gcd_x_y[1] % p);
    if (sign)
        res = -res;
    ASSERT_(gcd_x_y[0] == 1, "GCD is not 1")
    ASSERT_((k * res) % p == 1, "(k * x[1]) % p != 1")
    return res;
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
           ((y * y - x * x * x - curve->get_a() * x - curve->get_b()) % curve->get_p() == UINT_0);
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
    return *this * inverse_mod(k, curve->get_curve_order(curve->get_base_point()));
}

Point Point::operator/(UINT k) const {
    return *this * inverse_mod(k, curve->get_curve_order(curve->get_base_point()));
}

Point::Point() : is_inf(true), curve(nullptr) {}

Point &Point::operator=(const Point &other) {
    if (*this == other)
        return *this;
    x = other.x;
    y = other.y;
    is_inf = other.is_inf;
    curve = other.curve;
    return *this;
}
