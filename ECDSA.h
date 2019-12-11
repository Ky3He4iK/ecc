//
// Created by ky3he4ik on 06/12/2019.
//

#ifndef ECC_ECDSA_H
#define ECC_ECDSA_H

#include "EllipticCurve.h"
#include <random>
#include <string>
#include "dependences/json.hpp"

typedef LongInt ECDSA_private_key;

struct ECDSA_public_key {
    ECDSA_public_key();

    ECDSA_public_key(const LongInt &_curve_a, const LongInt &_curve_b, const LongInt &_curve_p,
                     const Point &_base_point, const LongInt &_curve_order, const Point &_random_point);

    ECDSA_public_key &operator=(const ECDSA_public_key &other) = default;

    [[nodiscard]] std::string serialize() const;

    [[nodiscard]] nlohmann::json to_json() const;

    static ECDSA_public_key deserialize(const std::string &data);

    static ECDSA_public_key deserialize(const nlohmann::json &data);

    LongInt curve_a;
    LongInt curve_b;
    LongInt curve_p;
    Point base_point;
    LongInt curve_order;
    Point random_point;
};

class ECDSA {
private:
    ECDSA_private_key private_key;
    ECDSA_public_key public_key;

public:
    explicit ECDSA(const std::shared_ptr<EllipticCurve> &curve, const Point &_base_point);

    explicit ECDSA(const ECDSA_private_key &_private_key, const ECDSA_public_key &_public_key);

    [[nodiscard]] std::pair<LongInt, LongInt> sign_msg(const std::string &message) const;

    static bool verify_msg(const std::string &message, const std::pair<LongInt, LongInt> &sign,
                           const ECDSA_public_key &public_key);

    static ECDSA getSECP256k1();

    [[nodiscard]] ECDSA_public_key get_public_key() const;

    [[nodiscard]] ECDSA_private_key get_private_key() const;

    [[nodiscard]] std::string serialize() const;

    static ECDSA deserialize(const std::string &data);
};

#endif //ECC_ECDSA_H
