//
// Created by ky3he4ik on 12/12/2019.
//

#include "ECC.h"
#include "dependences/picosha2.h"
#include "dependences/AES.h"

ECC::ECC(const std::shared_ptr<EllipticCurve> &curve, const Point &_base_point) {
    parameters = Curve_parameters(*curve, _base_point);
    auto keys = create_keys(parameters);
    private_key = keys.first;
    public_key = keys.second;
}

ECC::ECC(const Private_key &_private_key, const Public_key &_public_key,
         const Curve_parameters &_parameters) : private_key(_private_key), public_key(_public_key),
                                                parameters(_parameters) {}

ECC::ECC(const Curve_parameters &_parameters) : parameters(_parameters) {
    auto keys = create_keys(parameters);
    private_key = keys.first;
    public_key = keys.second;
}

std::pair<Private_key, Public_key> ECC::create_keys(const Curve_parameters &parameters) {
    std::pair<Private_key, Public_key> res;
    while (res.first == 0 || res.first >= parameters.curve_order)
        res.first = LongInt::get_random(parameters.curve_order.get_actual_bits()) % parameters.curve_order;
    res.second = Public_key(parameters.base_point * res.first);
    return res;
}

Public_key ECC::get_public_key() const {
    return public_key;
}

Private_key ECC::get_private_key() const {
    return private_key;
}

Curve_parameters ECC::get_parameters() const {
    return parameters;
}

Sign ECC::sign_msg(const std::string &message) const {
    LongInt hash = picosha2::hash256_long_int(message) % parameters.curve_order;
    LongInt r;
    LongInt s;

    // Choose a randomly selected secret point dP then compute r and s.
    while (s == 0) {
        LongInt d;
        while (d == 0 || d >= parameters.curve_order)
            d = LongInt::get_random(parameters.curve_order.get_actual_bits()) % parameters.curve_order;
        r = (parameters.base_point * d).get_x() % parameters.curve_order;
        if (r == 0)
            continue;
        s = (Point::inverse_mod(d, parameters.curve_order) * (hash + r * private_key)) % parameters.curve_order;
    }
    return {r, s};
}

bool ECC::verify_msg(const std::string &message, const Sign &sign, const Public_key &public_key,
                     const Curve_parameters &parameters) {
    if (parameters.base_point.get_inf() || !(parameters.base_point * parameters.curve_order).get_inf())
        return false;
    // Confirm that r and s are at least in the acceptable range.
    if (sign.first > parameters.curve_order || sign.second > parameters.curve_order)
        return false;

    LongInt hash = picosha2::hash256_long_int(message) % parameters.curve_order;
    LongInt w = Point::inverse_mod(sign.second, parameters.curve_order);
    LongInt u1 = (hash * w) % parameters.curve_order;
    LongInt u2 = (sign.first * w) % parameters.curve_order;
    Point C = parameters.base_point * u1 + public_key * u2;
    return sign.first % parameters.curve_order == C.get_x() % parameters.curve_order;
}

std::string ECC::serialize() const {
    nlohmann::json res = {
            {"public_key",  {
                                    {"x", public_key.get_x().to_string(16)},
                                    {"y", public_key.get_y().to_string(16)}
                            }},
            {"private_key", private_key.to_string(16)},
            {"parameters",  parameters.to_json()}
    };
    return res.dump(4);
}

ECC ECC::deserialize(const std::string &data) {
    auto ecc = nlohmann::json::parse(data);
    auto parametes = Curve_parameters::deserialize(ecc["parameters"]);
    auto public_key = Public_key(std::make_shared<EllipticCurve>(parametes.create_curve()),
                                 LONG_INT_FROM_JSON(ecc["public_key"]["x"]),
                                 LONG_INT_FROM_JSON(ecc["public_key"]["y"]));
    auto private_key = Private_key(ecc["private_key"].get<std::string>(), 16);
    return ECC(private_key, public_key, parametes);
}

std::string ECC::encode(std::string &msg) const {
    if (shared_secret == 0) {
        std::cerr << "Warning: shared secret is invalid!\n";
        return "";
    }
    AES aes(256);
    unsigned int len = 0;
    std::string res((char *) aes.EncryptECB((unsigned char *) msg.c_str(), msg.length(),
                                            (unsigned char *) shared_secret.to_bin_string().c_str(), len));
    return res;
}

std::string ECC::decode(std::string &msg) const {
    if (shared_secret == 0) {
        std::cerr << "Warning: shared secret is invalid!\n";
        return "";
    }
    AES aes(256);
    unsigned int len = 0;
    std::string res((char *) aes.DecryptECB((unsigned char *) msg.c_str(), msg.length(),
                                            (unsigned char *) shared_secret.to_bin_string().c_str()));
    return res;
}

Private_key ECC::set_shared_secret(const Public_key &another) {
    shared_secret = (another * private_key).get_x();
    return shared_secret;
}
