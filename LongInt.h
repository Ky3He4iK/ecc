//
// Created by ky3he4ik on 08/11/19.
//

#ifndef ELIPTIC_CURVES_CYPHER_LONGINT_H
#define ELIPTIC_CURVES_CYPHER_LONGINT_H

#include <cstdint>
#include <stdexcept>
#include <string>
#include <algorithm>

#include <vector>
#include <array>
#include <iostream>

typedef uint32_t UINT; //32bit because div and mult easier to write that way


#define BITS_BASE (sizeof(UINT) * (UINT)8)
#define ARR_SIZE (UINT)(bits_num / BITS_BASE + ((bits_num % BITS_BASE == 0) ? 0 : 1))
#define LAST (len - (UINT)1)
#define FOR_IND(i) for (UINT (i) = 0; (i) < len; (i)++)
#define FOR_IND_REVERSE(i) for (UINT (i) = LAST; (i) != ((UINT) -1); (i)--)
#define UINT_MAX ((UINT)-1)
#define UINT_0 (UINT)0
#define ASSERT_(condition, msg) { \
    if (!(condition)) \
        std::cerr << "Houston ... We've Got a Problem: assertion failed at " << __FILE__ << ":" << __LINE__ \
                  << " At func " << __FUNCTION__ << ": (" << msg << ")"; \
}

/**
 * Unsigned number with fixed @bits_num-bit length
 *
 */
class LongInt {
private:
    const UINT bits_num;
    std::vector<UINT> value;
    // array that represents this number. Big-endian (?)
    UINT len = ARR_SIZE;
    bool overflowed = false;

    void set_overflowed(bool val = true);

    static char itoc(int i);

    static int ctoi(char c);

public:
//    const UINT ARR_SIZE = ARR_SIZE;

    LongInt(UINT bits_num, const UINT *init);

    explicit LongInt(UINT bits_num);

    explicit LongInt(UINT bits_num, UINT init);

    LongInt(const LongInt &other) = default;

    explicit LongInt(UINT bits_num, const std::string &str, int radix = 10);

    [[nodiscard]] LongInt changeLen(UINT new_bits_num) const;

    [[nodiscard]] std::string to_string(UINT radix = 10, bool group = false) const;

    [[nodiscard]] UINT last_item() const;

    [[nodiscard]] bool get_bit(UINT pos) const;

    void set_bit(UINT pos, bool val = true);

    [[nodiscard]] UINT get_bits_count() const;

    [[nodiscard]] UINT get_len() const;

    [[nodiscard]] bool get_overflowed() const;

    [[nodiscard]] UINT get_actual_bits() const;

    LongInt operator+(const LongInt &other) const;

    LongInt operator+(UINT other) const;

    LongInt &operator+=(const LongInt &other);

    LongInt &operator+=(UINT other);


    friend LongInt operator+(UINT first, const LongInt &other) {
        return other + first;
    }


    LongInt operator-(const LongInt &other) const;

    LongInt operator-(UINT other) const;

    LongInt &operator-=(const LongInt &other);

    LongInt &operator-=(UINT other);


    friend LongInt operator-(UINT first, const LongInt &other) {
        return other - first;
    }


    LongInt operator/(const LongInt &other) const;

    LongInt operator/(UINT other) const;

    LongInt &operator/=(const LongInt &other);

    LongInt &operator/=(UINT other);

//    friend LongInt operator/(UINT first, const LongInt &other);


    LongInt operator*(const LongInt &other) const;

    LongInt operator*(UINT other) const;

    LongInt &operator*=(const LongInt &other);

    LongInt &operator*=(UINT other);


    friend LongInt operator*(UINT first, const LongInt &other) {
        return other * first;
    }


    LongInt operator%(const LongInt &other) const;

    UINT operator%(UINT other) const;

    LongInt &operator%=(const LongInt &other);

    LongInt &operator%=(UINT other);


    bool operator==(const LongInt &other) const;

    bool operator==(UINT other) const;


    friend bool operator==(UINT first, const LongInt &other) {
        return other == first;
    }


    bool operator!=(const LongInt &other) const;

    bool operator!=(UINT other) const;


    friend bool operator!=(UINT first, const LongInt &other) {
        return other != first;
    }

    bool operator>(const LongInt &other) const;

    bool operator>(UINT other) const;


    friend bool operator>(UINT first, const LongInt &other) {
        return other < first;
    }

    bool operator>=(const LongInt &other) const;

    bool operator>=(UINT other) const;


    friend bool operator>=(UINT first, const LongInt &other) {
        return other <= first;
    }


    bool operator<(const LongInt &other) const;

    bool operator<(UINT other) const;


    friend bool operator<(UINT first, const LongInt &other) {
        return other > first;
    }

    bool operator<=(const LongInt &other) const;

    bool operator<=(UINT other) const;


    friend bool operator<=(UINT first, const LongInt &other) {
        return other >= first;
    }


    LongInt operator|(const LongInt &other) const;

    LongInt operator|(UINT other) const;

    LongInt &operator|=(const LongInt &other);

    LongInt &operator|=(UINT other);


    friend LongInt operator|(UINT first, const LongInt &other) {
        return other | first;
    }


    LongInt operator&(const LongInt &other) const;

    LongInt operator&(UINT other) const;

    LongInt &operator&=(const LongInt &other);

    LongInt &operator&=(UINT other);


    friend LongInt operator&(UINT first, const LongInt &other) {
        return other & first;
    }


    LongInt operator^(const LongInt &other) const;

    LongInt operator^(UINT other) const;

    LongInt &operator^=(const LongInt &other);

    LongInt &operator^=(UINT other);


    friend LongInt operator^(UINT first, const LongInt &other) {
        return other ^ first;
    }


    LongInt operator<<(UINT other) const;

    LongInt operator>>(UINT other) const;

    LongInt operator<<=(UINT other);

    LongInt operator>>=(UINT other);

    LongInt operator~() const;

    LongInt &operator++();

    const LongInt operator++(int);

    LongInt &operator--();

    const LongInt operator--(int);

    LongInt &operator=(const LongInt &other);

    LongInt &operator=(UINT other);

    LongInt &operator=(int other);

    LongInt operator-() const;

    UINT &operator[](UINT index);

    UINT operator[](UINT index) const;
};

#endif //ELIPTIC_CURVES_CYPHER_LONGINT_H
