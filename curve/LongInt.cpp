//
// Created by ky3he4ik on 08/11/19.
//

#include "LongInt.h"

#include <utility>
#include <random>

#define num_mpz_t num.get_mpz_t()

LongInt::LongInt(int init) : num(init) {}

LongInt::LongInt(const std::string &str, int radix) : num(0) {
    for (auto &c: str) {
        int i = ctoi(c);
        if (i != -1 && i < radix)
            *this = *this * radix + i;
    }
    if (str[0] == '-')
        *this *= -1;
}

int LongInt::ctoi(char c) {
    if (c <= '9' && c >= '0')
        return c - '0';
    if (c <= 'Z' && c >= 'A')
        return c - 'A' + 10;
    if (c <= 'z' && c >= 'a')
        return c - 'a' + 10;
    return -1;
}


char LongInt::itoc(int i) {
    return (i < 10) ? i + '0' : i + 'A' - 10;
}


std::string LongInt::to_string(int radix) const {
    std::string res;
    LongInt tmp(abs());
    while (tmp != 0) {
        res.push_back(itoc((tmp % radix).to_int()));
        tmp /= radix;
    }
    if (res.empty())
        res.push_back('0');
    else if (*this < 0)
        res.push_back('-');
    reverse(res.begin(), res.end());
    return res;
}

std::string LongInt::to_bin_string() const {
    std::string res;
    LongInt tmp(abs());
    while (tmp != 0) {
        res.push_back(tmp.to_int());
        tmp >>= 8;
    }
    if (res.empty())
        res.push_back('0');
    else if (*this < 0)
        res.push_back('-');
    reverse(res.begin(), res.end());
    return res;
}

int LongInt::to_int() const {
    return mpz_get_si(num_mpz_t);
}

size_t LongInt::get_actual_bits() const {
    //todo
    LongInt tmp(*this);
    size_t res = 0;
    while (tmp != 0) {
        ++res;
        tmp >>= 1;
    }
    return res;
}

LongInt LongInt::operator+(const LongInt &other) const {
    mpz_class res;
    mpz_add(res.get_mpz_t(), num_mpz_t, other.num.get_mpz_t());
    return LongInt(res);
}

LongInt LongInt::operator-(const LongInt &other) const {
    mpz_class res;
    mpz_sub(res.get_mpz_t(), num_mpz_t, other.num.get_mpz_t());
    return LongInt(res);
}

LongInt LongInt::operator/(const LongInt &other) const {
    mpz_class res;
    mpz_fdiv_q(res.get_mpz_t(), num_mpz_t, other.num.get_mpz_t());
    return LongInt(res);
}


LongInt LongInt::operator*(const LongInt &other) const {
    mpz_class res;
    mpz_mul(res.get_mpz_t(), num_mpz_t, other.num.get_mpz_t());
    return LongInt(res);
}


LongInt LongInt::operator%(const LongInt &other) const {
    if (other == 0)
        return LongInt();
    mpz_class res;
    mpz_fdiv_r(res.get_mpz_t(), num_mpz_t, other.num.get_mpz_t());
    return LongInt(res);
}


bool LongInt::operator==(const LongInt &other) const {
    return mpz_cmp(num_mpz_t, other.num.get_mpz_t()) == 0;
}


bool LongInt::operator!=(const LongInt &other) const {
    return mpz_cmp(num_mpz_t, other.num.get_mpz_t()) != 0;
}


bool LongInt::operator>(const LongInt &other) const {
    return mpz_cmp(num_mpz_t, other.num.get_mpz_t()) > 0;
}


bool LongInt::operator>=(const LongInt &other) const {
    return mpz_cmp(num_mpz_t, other.num.get_mpz_t()) >= 0;
}


bool LongInt::operator<(const LongInt &other) const {
    return mpz_cmp(num_mpz_t, other.num.get_mpz_t()) < 0;
}


bool LongInt::operator<=(const LongInt &other) const {
    return mpz_cmp(num_mpz_t, other.num.get_mpz_t()) <= 0;
}


LongInt LongInt::operator|(const LongInt &other) const {
    mpz_class res;
    mpz_ior(res.get_mpz_t(), num_mpz_t, other.num.get_mpz_t());
    return LongInt(res);
}


LongInt LongInt::operator&(const LongInt &other) const {
    mpz_class res;
    mpz_and(res.get_mpz_t(), num_mpz_t, other.num.get_mpz_t());
    return LongInt(res);
}


LongInt LongInt::operator^(const LongInt &other) const {
    mpz_class res;
    mpz_xor(res.get_mpz_t(), num_mpz_t, other.num.get_mpz_t());
    return LongInt(res);
}

LongInt LongInt::operator<<(size_t other) const {
    mpz_class res;
    mpz_mul_2exp(res.get_mpz_t(), num_mpz_t, other);
    return LongInt(res);
}

LongInt LongInt::operator>>(size_t other) const {
    mpz_class res;
    mpz_fdiv_q_2exp(res.get_mpz_t(), num_mpz_t, other);
    return LongInt(res);
}

LongInt &LongInt::operator++() {
    *this += 1;
    return *this;
}


const LongInt LongInt::operator++(int) {
    LongInt res(*this);
    *this += 1;
    return res;
}


LongInt &LongInt::operator--() {
    *this -= 1;
    return *this;
}


const LongInt LongInt::operator--(int) {
    LongInt res(*this);
    *this -= 1;
    return res;
}

LongInt &LongInt::operator=(int other) {
    *this = LongInt(other);
    return *this;
}

LongInt LongInt::operator-() const {
    mpz_class res;
    mpz_neg(res.get_mpz_t(), num_mpz_t);
    return LongInt(res);
}

// fast pow mod (need to fast check is point on the curve on not)
// Calculate (x ** y) % z efficiently.
LongInt LongInt::pow_and_mod(const LongInt &y, const LongInt &z) const {
    mpz_class res;
    if (z == 0)
        return LongInt(0);
    mpz_powm_sec(res.get_mpz_t(), num_mpz_t, y.num.get_mpz_t(), z.num.get_mpz_t());
    return LongInt(res);
}

LongInt LongInt::get_random(size_t bits_count) {
    std::random_device random;
    LongInt res;
    for (size_t i = 0; i <= bits_count; i++)
        res = (res << 1) | (random() & 1);
    return res;
}

LongInt LongInt::abs() const {
    mpz_class res;
    mpz_abs(res.get_mpz_t(), num_mpz_t);
    return LongInt(res);
}

LongInt::LongInt(mpz_class init) : num(std::move(init)) {}


LongInt &LongInt::operator+=(const LongInt &other) {
    *this = *this + other;
    return *this;
}


LongInt &LongInt::operator-=(const LongInt &other) {
    *this = *this - other;
    return *this;
}


LongInt &LongInt::operator*=(const LongInt &other) {
    *this = *this * other;
    return *this;
}


LongInt &LongInt::operator/=(const LongInt &other) {
    *this = *this / other;
    return *this;
}


LongInt &LongInt::operator%=(const LongInt &other) {
    *this = *this % other;
    return *this;
}


LongInt &LongInt::operator|=(const LongInt &other) {
    *this = *this | other;
    return *this;
}


LongInt &LongInt::operator&=(const LongInt &other) {
    *this = *this & other;
    return *this;
}


LongInt &LongInt::operator^=(const LongInt &other) {
    *this = *this ^ other;
    return *this;
}


LongInt &LongInt::operator>>=(size_t other) {
    *this = *this >> other;
    return *this;
}


LongInt &LongInt::operator<<=(size_t other) {
    *this = *this << other;
    return *this;
}

bool LongInt::is_odd() const {
    return (*this & 1) == 1;
}

LongInt LongInt::operator+(int other) const {
    return *this + LongInt(other);
}


LongInt LongInt::operator-(int other) const {
    return *this - LongInt(other);
}


LongInt LongInt::operator*(int other) const {
    return *this * LongInt(other);
}


LongInt LongInt::operator/(int other) const {
    return *this / LongInt(other);
}


LongInt LongInt::operator%(int other) const {
    return *this % LongInt(other);
}


bool LongInt::operator==(int other) const {
    return *this == LongInt(other);
}


bool LongInt::operator!=(int other) const {
    return *this != LongInt(other);
}


bool LongInt::operator>(int other) const {
    return *this > LongInt(other);
}


bool LongInt::operator>=(int other) const {
    return *this >= LongInt(other);
}


bool LongInt::operator<(int other) const {
    return *this < LongInt(other);
}


bool LongInt::operator<=(int other) const {
    return *this <= LongInt(other);
}


LongInt LongInt::operator|(int other) const {
    return *this | LongInt(other);
}


LongInt LongInt::operator&(int other) const {
    return *this & LongInt(other);
}


LongInt LongInt::operator^(int other) const {
    return *this ^ LongInt(other);
}


LongInt &LongInt::operator+=(int other) {
    *this = *this + LongInt(other);
    return *this;
}


LongInt &LongInt::operator-=(int other) {
    *this = *this - LongInt(other);
    return *this;
}


LongInt &LongInt::operator*=(int other) {
    *this = *this * LongInt(other);
    return *this;
}


LongInt &LongInt::operator/=(int other) {
    *this = *this / LongInt(other);
    return *this;
}


LongInt &LongInt::operator%=(int other) {
    *this = *this % LongInt(other);
    return *this;
}


LongInt &LongInt::operator|=(int other) {
    *this = *this | LongInt(other);
    return *this;
}


LongInt &LongInt::operator&=(int other) {
    *this = *this & LongInt(other);
    return *this;
}


LongInt &LongInt::operator^=(int other) {
    *this = *this ^ LongInt(other);
    return *this;
}
