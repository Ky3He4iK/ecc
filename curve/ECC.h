//
// Created by ky3he4ik on 12/12/2019.
//

#ifndef ECC_ECC_H
#define ECC_ECC_H

#include "EllipticCurve.h"
#include <random>
#include <string>
#include "../dependences/json.hpp"

#include "Curve_parameters.h"

typedef LongInt Private_key;
typedef Point Public_key;
typedef std::pair<LongInt, LongInt> Sign;

class ECC {
private:
    Private_key private_key;
    Public_key public_key;
    Curve_parameters parameters;
    Private_key shared_secret;

public:
    explicit ECC(const std::shared_ptr<EllipticCurve> &curve, const Point &_base_point);

    explicit ECC(const Private_key &_private_key, const Public_key &_public_key, const Curve_parameters &_parameters);

    explicit ECC(const Curve_parameters &_parameters);

    void set_keys(const Private_key &priv, const Public_key &pub);

    // ECDSA
    [[nodiscard]] Sign sign_msg(const std::string &message) const;

    static bool verify_msg(const std::string &message, const Sign &sign, const Public_key &public_key,
                           const Curve_parameters &parameters);

    // ECDH
    Private_key set_shared_secret(const Public_key &another);

    // (De)cypher using AES
    std::string encode(std::string &msg) const;

    std::string decode(std::string &msg) const;

    static std::pair<Private_key, Public_key> create_keys(const Curve_parameters &parameters);

    Public_key generatePublic(const Private_key &priv);

    [[nodiscard]] Public_key get_public_key() const;

    [[nodiscard]] Private_key get_private_key() const;

    [[nodiscard]] Curve_parameters get_parameters() const;

    [[nodiscard]] std::shared_ptr<EllipticCurve> get_curve() const;

    [[nodiscard]] std::string serialize() const;

    static ECC deserialize(const std::string &data);
};


#endif //ECC_ECC_H
