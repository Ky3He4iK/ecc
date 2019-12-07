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

public:
    ECDSA(const EllipticCurve *_curve);

    [[nodiscard]] std::pair<UINT, Point> generate_keypair(const Point &base_point) const;

    [[nodiscard]] std::pair<LongInt, UINT>
    sign_msg(const std::string &message, const Point &base_point, UINT private_key, const Point &public_key) const;

    [[nodiscard]] bool
    verify_msg(const std::string &message, const Point &base_point, UINT private_key, const Point &public_key,
               const std::pair<LongInt, UINT> &sign) const;

    static LongInt hash_truncated(const std::string &str, UINT num);
};

#endif //ECC_ECDSA_H
