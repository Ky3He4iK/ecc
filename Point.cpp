//
// Created by ky3he4ik on 22/11/19.
//

#include "Point.h"

#include <stdexcept>
#include <exception>

#define ASSERT_ON_CURVE(point) ASSERT_((point).on_curve(), "Point not on curve!");

std::array<LongInt, 3> Point::extended_gcd(const LongInt &a, const LongInt &b) {
#define GCD_STEP(v, old_v, tmp, q) { \
    (tmp) = (v); \
    (v) = (old_v) - (q) * (v); \
    (old_v) = (tmp); \
}

    LongInt s(a.get_bits_count(), (UINT) 0), old_s(a.get_bits_count(), 1);
    LongInt t(a.get_bits_count(), 1), old_t(a.get_bits_count(), (UINT) 0);
    LongInt r(b), old_r(a);
    while (r != (UINT) 0) {
        LongInt q = old_r / r;
        LongInt tmp = r;
        GCD_STEP(r, old_r, tmp, q)
        GCD_STEP(s, old_s, tmp, q)
        GCD_STEP(t, old_t, tmp, q)
    }
    return {old_r, old_s, old_t};
}

Point::Point(const EllipticCurve *_curve, const LongInt &_x, const LongInt &_y) : curve(_curve),
                                                                                  x(_x % *_curve->get_p()),
                                                                                  y(_y % *_curve->get_p()),
                                                                                  p(_curve->get_p()) {}

Point Point::operator+(const Point &other) const {
    Point res(*this);
    res += other;
    return res;
}

// Returns the result of this + other according to the group law.
Point &Point::operator+=(const Point &other) {
    ASSERT_ON_CURVE(*this)
    ASSERT_ON_CURVE(other)
    if (is_inf) {
        x = other.x;
        y = other.y;
        is_inf = other.is_inf;
    } else if (!other.is_inf) {
        LongInt m(x);
        if (x == other.x) {
            if (y == other.y)
                m = ((3 * x * x + *curve->get_a()) * inverse_mod(2 * y, *p)) % *p;
            else
                is_inf = true;
        } else
            m = ((y - other.y) * inverse_mod(x - other.x, *p)) % *p;
        LongInt xr = m * m - x - other.x;
        y = (*curve->get_p() * 2 - y - m * (xr - x) % *curve->get_p()) % *curve->get_p();
        x = xr;
    }
    ASSERT_ON_CURVE(*this)
    return *this;
}


// Returns k * point computed using the double and point_add algorithm.
Point Point::operator*(const LongInt &k) const {
    ASSERT_ON_CURVE(*this)
    Point res = Point(curve, LongInt(0), LongInt(0));
    if (k % *curve->get_n() == UINT_0) {
        res.is_inf = true;
    } else if (k < UINT_0)
        return (-*this) * (-k);
    else {
        Point pow = Point(*this);
        for (UINT i = 0; i < k.get_bits_count(); i++) {
            if (k.get_bit(k.get_bits_count() - i - 1))
                res += pow;
            pow += pow;
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
Point Point::inf_point(const EllipticCurve *curve) {
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
    ASSERT_((k * gcd_x_y[1]) % p == 1, "(k * x[1]) % p != 1")

    return gcd_x_y[1] % p;
}

// Returns -point
Point Point::operator-() const {
    ASSERT_ON_CURVE(*this)
    if (is_inf)
        return Point::inf_point(curve);
    Point res = {curve, x, (*curve->get_p() - y) % *curve->get_p()};
    ASSERT_ON_CURVE(*this)
    return res;
}

// Returns true if the given point lies on the elliptic curve
bool Point::on_curve() const {
    return is_inf | ((y * y - x * x * x - *curve->get_a() * x - *curve->get_b()) % *curve->get_p() == UINT_0);
}
