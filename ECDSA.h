//
// Created by ky3he4ik on 06/12/2019.
//

#ifndef ECC_ECDSA_H
#define ECC_ECDSA_H

#include "EllipticCurve.h"
#include <random>
#include <string>

typedef LongInt ECDSA_private_key;

struct ECDSA_public_key {
    ECDSA_public_key();

    ECDSA_public_key(const LongInt &_curve_a, const LongInt &_curve_b, const Point &_base_point,
                     const LongInt &_curve_order, const Point &_random_point);

    ECDSA_public_key &operator=(const ECDSA_public_key &other) = default;

    LongInt curve_a;
    LongInt curve_b;
    Point base_point;
    LongInt curve_order;
    Point random_point;
};

class ECDSA {
private:
    const EllipticCurve *curve;
    ECDSA_private_key private_key;
    ECDSA_public_key public_key;

public:
    explicit ECDSA(const EllipticCurve *_curve, const Point &_base_point);

    [[nodiscard]] std::pair<LongInt, LongInt> sign_msg(const std::string &message) const;

    [[nodiscard]] bool verify_msg(const std::string &message, const std::pair<LongInt, LongInt> &sign) const;

    static ECDSA getSECP256k1();
};

#endif //ECC_ECDSA_H
