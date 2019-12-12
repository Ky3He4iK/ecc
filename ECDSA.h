//
// Created by ky3he4ik on 06/12/2019.
//

#ifndef ECC_ECDSA_H
#define ECC_ECDSA_H

#include "EllipticCurve.h"
#include <random>
#include <string>
#include "dependences/json.hpp"

#include "Curve_parameters.h"

typedef LongInt ECDSA_private_key;
typedef Point ECDSA_public_key;

class ECDSA {
private:
    ECDSA_private_key private_key;
    ECDSA_public_key public_key;
    Curve_parameters parameters;

public:
    explicit ECDSA(const std::shared_ptr<EllipticCurve> &curve, const Point &_base_point);

    explicit ECDSA(const ECDSA_private_key &_private_key, const ECDSA_public_key &_public_key,
                   const Curve_parameters &_parameters);

    explicit ECDSA(const Curve_parameters &_parameters);

    [[nodiscard]] std::pair<LongInt, LongInt> sign_msg(const std::string &message) const;

    static bool verify_msg(const std::string &message, const std::pair<LongInt, LongInt> &sign,
                           const ECDSA_public_key &public_key, const Curve_parameters &_parameters);

    static ECDSA getSECP256k1();

    [[nodiscard]] ECDSA_public_key get_public_key() const;

    [[nodiscard]] ECDSA_private_key get_private_key() const;

    [[nodiscard]] Curve_parameters get_parameters() const;

    [[nodiscard]] std::string serialize() const;

    static ECDSA deserialize(const std::string &data);
};

#endif //ECC_ECDSA_H
