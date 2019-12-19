//
// Created by ky3he4ik on 08/11/19.
//

#ifndef ELIPTIC_CURVES_CYPHER_LONGINT_H
#define ELIPTIC_CURVES_CYPHER_LONGINT_H

#include <string>
#include <gmpxx.h>
#include <iostream>

#define ASSERT_(condition, msg) { \
    if (!(condition)) \
        std::cerr << "Houston ... We've Got a Problem: assertion failed at " << __FILE__ << ":" << __LINE__ \
                  << " At func " << __FUNCTION__ << ": (" << msg << ")" << '\n'; \
}

class LongInt {
private:
    mpz_class num;

    explicit LongInt(mpz_class init);

    static char itoc(int i);

    static int ctoi(char c);

public:
    explicit LongInt(int init = 0);

    LongInt(const LongInt &other) = default;

    explicit LongInt(const std::string &str, int radix = 10);

    [[nodiscard]] std::string to_string(int radix = 10) const;

    [[nodiscard]] std::string to_bin_string() const;

    [[nodiscard]] int to_int() const;

    [[nodiscard]] size_t get_actual_bits() const;

    [[nodiscard]] LongInt pow_and_mod(const LongInt &y, const LongInt &z) const;

    static LongInt get_random(size_t bits_count);

    [[nodiscard]] LongInt abs() const;

    [[nodiscard]] bool is_odd() const;

    LongInt operator<<(size_t other) const;

    LongInt operator>>(size_t other) const;

    LongInt &operator++();

    const LongInt operator++(int);

    LongInt &operator--();

    const LongInt operator--(int);

    LongInt &operator=(const LongInt &other) = default;

    LongInt &operator=(int other);


    LongInt &operator>>=(size_t other);

    LongInt &operator<<=(size_t other);

    LongInt operator+(int other) {
        return *this + LongInt(other);
    };


    LongInt operator-(int other) {
        return *this - LongInt(other);
    };


    LongInt operator*(int other) {
        return *this * LongInt(other);
    };


    LongInt operator/(int other) {
        return *this / LongInt(other);
    };


    LongInt operator%(int other) {
        return *this % LongInt(other);
    };


    LongInt operator|(int other) {
        return *this | LongInt(other);
    };


    LongInt operator&(int other) {
        return *this & LongInt(other);
    };


    LongInt operator^(int other) {
        return *this ^ LongInt(other);
    };

    LongInt operator-() const;


    LongInt operator+(const LongInt &other) const;


    LongInt operator-(const LongInt &other) const;


    LongInt operator*(const LongInt &other) const;


    LongInt operator/(const LongInt &other) const;


    LongInt operator%(const LongInt &other) const;


    bool operator==(const LongInt &other) const;


    bool operator!=(const LongInt &other) const;


    bool operator>(const LongInt &other) const;


    bool operator>=(const LongInt &other) const;


    bool operator<(const LongInt &other) const;


    bool operator<=(const LongInt &other) const;


    LongInt operator|(const LongInt &other) const;


    LongInt operator&(const LongInt &other) const;


    LongInt operator^(const LongInt &other) const;


    LongInt &operator+=(const LongInt &other);


    LongInt &operator-=(const LongInt &other);


    LongInt &operator*=(const LongInt &other);


    LongInt &operator/=(const LongInt &other);


    LongInt &operator%=(const LongInt &other);


    LongInt &operator|=(const LongInt &other);


    LongInt &operator&=(const LongInt &other);


    LongInt &operator^=(const LongInt &other);


    LongInt operator+(int other) const;


    LongInt operator-(int other) const;


    LongInt operator*(int other) const;


    LongInt operator/(int other) const;


    LongInt operator%(int other) const;


    bool operator==(int other) const;


    bool operator!=(int other) const;


    bool operator>(int other) const;


    bool operator>=(int other) const;


    bool operator<(int other) const;


    bool operator<=(int other) const;


    LongInt operator|(int other) const;


    LongInt operator&(int other) const;


    LongInt operator^(int other) const;


    LongInt &operator+=(int other);


    LongInt &operator-=(int other);


    LongInt &operator*=(int other);


    LongInt &operator/=(int other);


    LongInt &operator%=(int other);


    LongInt &operator|=(int other);


    LongInt &operator&=(int other);


    LongInt &operator^=(int other);

};

//#include "LongInt.cpp"
#endif //ELIPTIC_CURVES_CYPHER_LONGINT_H
