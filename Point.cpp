//
// Created by ky3he4ik on 22/11/19.
//

#include "Point.h"

#include <stdexcept>
#include <exception>

std::array<AS_INT, 3> Point::extended_gcd(const AS_INT &a, const AS_INT &b) {
#define GCD_STEP(v, old_v, tmp, q) { \
    (tmp) = (v); \
    (v) = (old_v) - (q) * (v); \
    (old_v) = (tmp); \
}
    AS_INT s(a.get_bits_count(), (UINT) 0), old_s(a.get_bits_count(), 1);
    AS_INT t(a.get_bits_count(), 1), old_t(a.get_bits_count(), (UINT) 0);
    AS_INT r(b), old_r(a);
    while (r != 0) {
        AS_INT q = old_r / r;
        AS_INT tmp = r;
        GCD_STEP(r, old_r, tmp, q)
        GCD_STEP(s, old_s, tmp, q)
        GCD_STEP(t, old_t, tmp, q)
    }
    return {old_r, old_s, old_t};
}

AS_INT Point::inverse_of(const AS_INT &n, const AS_INT &p) {
    auto res = extended_gcd(n, p);
    if ((n * res[1] + p * res[2]) % p != res[0])
        throw std::invalid_argument(
                std::string("(") + n.to_string() + " * " + res[1].to_string() + " + " + p.to_string() + " * " +
                res[2].to_string() + ") % " + p.to_string() + " != " + res[0].to_string());
    if (res[0] != 1)
        throw std::invalid_argument(n.to_string() + " has no multiplicative inverse modulo " + p.to_string());
    return res[1] % p;
}

Point::Point(const EllipticCurve *_curve, const AS_INT &_x, const AS_INT &_y) : curve(_curve), x(_x % *_curve->get_p()),
                                                                                y(_y % *_curve->get_p()),
                                                                                p(_curve->get_p()) {}

Point Point::operator+(const Point &other) const {
    AS_INT m = (x == other.x)
               ? ((3 * x * x + *curve->get_a()) * inverse_of(2 * y, *p)) % *p
               : ((y - other.y) * inverse_of(x - other.x, *p)) % *p;
    AS_INT xr = m * m - x - other.x;
    return {curve, xr, y + m * (xr - x)};
}

Point &Point::operator+=(const Point &other) {
    AS_INT m = (x == other.x)
               ? ((3 * x * x + *curve->get_a()) * inverse_of(2 * y, *p)) % *p
               : ((y - other.y) * inverse_of(x - other.x, *p)) % *p;
    AS_INT xr = m * m - x - other.x;
    y += m * (xr - x);
    x = xr;
    return *this;
}

Point Point::operator*(const AS_INT &other) const {
    Point res = Point(curve, AS_INT(0), AS_INT(0));
    Point pow = Point(*this);
    for (UINT i = 0; i < other.get_actual_bits(); i++) {
        if (other.get_bit(other.get_bits_count() - i - 1))
            res += pow;
        pow += pow;
    }
    return res;
}
