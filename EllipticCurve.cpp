//
// Created by ky3he4ik on 15/11/19.
//

#include "EllipticCurve.h"

EllipticCurve::EllipticCurve(const LongInt &_a, const LongInt &_b, const LongInt &_c, const LongInt &_p,
                             const LongInt &_exp) : a(_a), b(_b), c(_c), p(_p), exp(_exp), random() {}

std::string EllipticCurve::to_string() const {
    std::string res("y^2 = x^3");
    if (a != 0) {
        if (a > 0)
            res += std::string(" + ");
        res += a.to_string() + "*x^2";
    }
    if (b != 0) {
        if (b > 0)
            res += std::string(" + ");
        res += b.to_string() + "*x";
    }
    if (c != 0) {
        if (c > 0)
            res += std::string(" + ");
        res += c.to_string();
    }
    if (p != 0) {
        res += " over field " + p.to_string();
        if (exp != 1) {
            res += "^" + exp.to_string();
        }
    }
    return res;
}

// Compute the discriminant.
LongInt EllipticCurve::discriminant() const {
    return a * a * (b * b - (a * c) << 2) + (b << 2) * (9 * a * c - 2 * b * b) - 27 * c * c;
}

// Compute the order of a point on the curve.
// Порядок точки
UINT EllipticCurve::order(const Point &point) const {
    if (p == 0)
        return 0;
    Point q(point);
    UINT order_p = 1;
    //Add P to Q repeatedly until obtaining the identity (point at infinity).
    while (!q.get_inf()) {
        q = point + q;
        ++order_p;
    }
    return order_p;
}

/*
 #Add P to itself k times.
    def mult(self, P, k):
        if P.is_infinite():
            return P
        elif k == 0:
            return Point.atInfinity()
        elif k < 0:
            return self.mult(self.invert(P), -k)
        else:
            #Convert k to a bitstring and use peasant multiplication to compute the product quickly.
            b = bin(k)[2:]
            return self.repeat_additions(P, b, 1)

    #Add efficiently by repeatedly doubling the given point, and adding the result to a running
    #total when, after the ith doubling, the ith digit in the bitstring b is a one.
    def repeat_additions(self, P, b, n):
        if b == '0':
            return Point.atInfinity()
        elif b == '1':
            return P
        elif b[-1] == '0':
            return self.repeat_additions(self.double(P), b[:-1], n+1)
        elif b[-1] == '1':
            return self.add(P, self.repeat_additions(self.double(P), b[:-1], n+1))
 */

bool EllipticCurve::contains(const Point &point) const {
//    auto c1 = point.get_y() * point.get_y();
//    auto c2 = point.get_x() * point.get_x() * point.get_x();
//    auto c3 = a * point.get_x() * point.get_x();
//    auto c4 = b * point.get_x();
//    auto c5 = c2 + c3 + c4 + c;
//    auto c6 = c1 - c5;
//    auto s1 = c1.to_string();
//    auto s2 = c2.to_string();
//    auto s3 = c3.to_string();
//    auto s4 = c4.to_string();
//    auto s5 = c5.to_string();
//    auto s6 = c6.to_string();
    return point.get_inf() || (
            point.get_y().fast_pow_mod(LongInt(LONG_INT_LEN, 2), p).abs() ==
            (point.get_x().fast_pow_mod(LongInt(LONG_INT_LEN, 3), p).abs() +
             (point.get_x().fast_pow_mod(LongInt(LONG_INT_LEN, 2), p) * a).abs() + (point.get_x() * b).abs() + c
            ) % p

    );
}

EllipticCurve EllipticCurve::getSECP256k1() {
    return EllipticCurve(LongInt(LONG_INT_LEN), LongInt(LONG_INT_LEN), LongInt(LONG_INT_LEN, 7),
                         LongInt(LONG_INT_LEN, "0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f",
                                 16),
                         LongInt(LONG_INT_LEN, 1));
}

/*
 * Generate a keypair using the point P of order n on the given curve. The private key is a
 * positive integer d smaller than n, and the public key is Q = dP.
 */
std::pair<UINT, Point> EllipticCurve::generate_keypair(const Point &point) {
    UINT d = LongInt::get_random(LONG_INT_LEN, random) % order(point);
    Point Q = point * LongInt(LONG_INT_LEN, d);
    return {d, Q};
}