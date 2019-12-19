//
// Created by ky3he4ik on 12/12/2019.
//

#include "ECC.h"
#include "../dependences/picosha2.h"
#include "../dependences/AES.h"

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

void ECC::set_private_key(const Private_key &key) {
    private_key = key;
    public_key = parameters.base_point * private_key;
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
    nlohmann::json res;
    res["public_key"]["x"] = public_key.get_x().to_string(16);
    res["public_key"]["y"] = public_key.get_y().to_string(16);
    res["private_key"] = private_key.to_string(16);
    res["parameters"] = parameters.to_json();
    return res.dump(4);
}

ECC ECC::deserialize(const std::string &data) {
    auto ecc = nlohmann::json::parse(data);
    auto parametes = Curve_parameters::deserialize(ecc["parameters"]);
    if (ecc.contains("public_key") && ecc.contains("private_key")) {
        auto public_key = Public_key(std::make_shared<EllipticCurve>(parametes.create_curve()),
                                     LONG_INT_FROM_JSON(ecc["public_key"]["x"]),
                                     LONG_INT_FROM_JSON(ecc["public_key"]["y"]));
        auto private_key = Private_key(ecc["private_key"].get<std::string>(), 16);
        return ECC(private_key, public_key, parametes);
    } else
        return ECC(parametes);
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
    return bin_str_to_hex(res);
}

std::string ECC::decode(std::string &msg) const {
    if (shared_secret == 0) {
        std::cerr << "Warning: shared secret is invalid!\n";
        return "";
    }
    AES aes(256);
    auto raw = bin_str_from_hex(msg);
    std::string res((char *) aes.DecryptECB((unsigned char *) raw.c_str(), raw.length(),
                                            (unsigned char *) shared_secret.to_bin_string().c_str()));
    return res;
}

Private_key ECC::set_shared_secret(const Public_key &another) {
    if (another.get_inf() || public_key.get_inf())
        return Private_key();
    shared_secret = (another * private_key).get_x();
    return shared_secret;
}

void ECC::set_keys(const Private_key &priv, const Public_key &pub) {
    private_key = priv;
    public_key = Point(public_key.get_curve(), pub.get_x(), pub.get_y());
}

std::shared_ptr<EllipticCurve> ECC::get_curve() const {
    return public_key.get_curve();
}

Public_key ECC::generatePublic(const Private_key &priv) {
    private_key = priv;
    public_key = parameters.base_point * priv;
    return public_key;
}

std::string ECC::bin_str_to_hex(const std::string &str) {
    std::string res;
    for (auto &c: str) {
        res.push_back(itoc((c >> 4) & 0xf));
        res.push_back(itoc(c & 0xf));
    }
    return res;
}

std::string ECC::bin_str_from_hex(const std::string &str) {
    std::string res;
    for (int i = 0; i < str.size();) {
        int a = ctoi(str[i++]);
        while (a == -1 && i < str.size())
            a = ctoi(str[i++]);
        if (i == str.size())
            break;
        int b = ctoi(str[i++]);
        while (b == -1 && i < str.size())
            b = ctoi(str[i++]);
        if (i == str.size() && b == -1)
            break;
        res.push_back((a << 4) + b);
    }
    return res;
}

char ECC::itoc(int i) {
    if (i < 0)
        i += 16;
    return (i < 10) ? i + '0' : i + 'A' - 10;
}

int ECC::ctoi(char c) {
    return ((c >= '0' && c <= '9') ? c - '0' :
            ((c <= 'Z' && c >= 'A') ? c - 'A' + 10 :
             ((c <= 'z' && c >= 'a') ? c - 'a' + 10 : -1)));
}