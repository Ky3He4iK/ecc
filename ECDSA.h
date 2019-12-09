//
// Created by ky3he4ik on 06/12/2019.
//

#ifndef ECC_ECDSA_H
#define ECC_ECDSA_H

#include "EllipticCurve.h"
#include <random>
#include <string>

class ECDSA {
private:
    const EllipticCurve *curve;
    const Point base_point;
    UINT curve_order;
    std::pair<UINT, Point> keypair{0, Point::inf_point(nullptr)};

public:
    explicit ECDSA(const EllipticCurve *_curve, const Point &_base_point);

    [[nodiscard]] std::pair<LongInt, UINT> sign_msg(const std::string &message) const;

    [[nodiscard]] bool verify_msg(const std::string &message, const std::pair<LongInt, UINT> &sign) const;

    static LongInt hash_truncated(const std::string &str, UINT num);

    static ECDSA getSECP256k1();
};

#endif //ECC_ECDSA_H
