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

typedef uint32_t UINT; //32bit because div and mult easier to write that way


#define BITS_BASE (sizeof(UINT) * (UINT)8)
#define ARR_SIZE (UINT)(bits_num / BITS_BASE + ((bits_num % BITS_BASE == 0) ? 0 : 1))
#define LAST (len - (UINT)1)
#define FOR_IND(i) for (UINT (i) = 0; (i) < len; (i)++)
#define FOR_IND_REVERSE(i) for (UINT (i) = LAST; (i) != ((UINT) -1); (i)--)
#define UINT_MAX ((UINT)-1)

//template<UINT bits_num>  class LongInt;

/**
 * Unsigned number with fixed @bits_num-bit length
 *
 */

template<UINT bits_num> //bits in the number
class LongInt {
private:
    std::array<UINT, ARR_SIZE> value{0};
    // array that represents this number. Big-endian (?)
    UINT len = ARR_SIZE;

    static char itoc(int i);

    static int ctoi(char c);

    static bool mult_cmp(const LongInt<bits_num> &a, const LongInt<bits_num> &b, const LongInt<bits_num> &third);

    static std::pair<LongInt<bits_num>, bool> shift_overflow(const LongInt<bits_num> &a, UINT other);

public:
//    const UINT ARR_SIZE = ARR_SIZE;

    explicit LongInt(const UINT init[ARR_SIZE]) {
//        value = new UINT[ARR_SIZE];
        FOR_IND(i)value[i] = init[i];
    }

    LongInt();

    explicit LongInt(UINT init);

    LongInt(const LongInt<bits_num> &other);

    explicit LongInt(const std::string &str, int radix = 10);

//    template<UINT _, UINT old_bits_num>
    template<UINT old_bits_num>
    static LongInt<bits_num> changeLen(const LongInt<old_bits_num> &other);

    std::string to_string(UINT radix = 10, bool group = false) const;

    UINT last_item() const;

    bool get_bit(UINT pos) const;

    void set_bit(UINT pos, bool val = true);

    UINT get_bits_count() const;

    UINT get_len() const;

    LongInt<bits_num> operator+(const LongInt<bits_num> &other) const;

    LongInt<bits_num> operator+(UINT other) const;

    LongInt<bits_num> &operator+=(const LongInt<bits_num> &other);

    LongInt<bits_num> &operator+=(UINT other);

    template<UINT _>
    friend LongInt<bits_num> operator+(UINT first, const LongInt<bits_num> &other);


    LongInt<bits_num> operator-(const LongInt<bits_num> &other) const;

    LongInt<bits_num> operator-(UINT other) const;

    LongInt<bits_num> &operator-=(const LongInt<bits_num> &other);

    LongInt<bits_num> &operator-=(UINT other);

    template<UINT _>
    friend LongInt<bits_num> operator-(UINT first, const LongInt<bits_num> &other);


    LongInt<bits_num> operator/(const LongInt<bits_num> &other);

    LongInt<bits_num> operator/(UINT other) const;

    LongInt<bits_num> &operator/=(const LongInt<bits_num> &other);

    LongInt<bits_num> &operator/=(UINT other);

//    friend LongInt<bits_num> operator/(UINT first, const LongInt<bits_num> &other);


    LongInt<bits_num> operator*(const LongInt<bits_num> &other) const;

    LongInt<bits_num> operator*(UINT other) const;

    LongInt<bits_num> &operator*=(const LongInt<bits_num> &other);

    LongInt<bits_num> &operator*=(UINT other);

    template<UINT _>
    friend LongInt<bits_num> operator*(UINT first, const LongInt<bits_num> &other);


    LongInt<bits_num> operator%(const LongInt<bits_num> &other) const;

    UINT operator%(UINT other) const;

    LongInt<bits_num> operator%=(const LongInt<bits_num> &other);

    LongInt<bits_num> operator%=(UINT other);


    bool operator==(const LongInt<bits_num> &other) const;

    bool operator==(UINT other) const;

    template<UINT _>
    friend bool operator==(UINT first, const LongInt<bits_num> &other);


    bool operator!=(const LongInt<bits_num> &other) const;

    bool operator!=(UINT other) const;

    template<UINT _>
    friend bool operator!=(UINT first, const LongInt<bits_num> &other);

    bool operator>(const LongInt<bits_num> &other) const;

    bool operator>(UINT other) const;

    template<UINT _>
    friend bool operator>(UINT first, const LongInt<bits_num> &other);

    bool operator>=(const LongInt<bits_num> &other) const;

    bool operator>=(UINT other) const;

    template<UINT _>
    friend bool operator>=(UINT first, const LongInt<bits_num> &other);


    bool operator<(const LongInt<bits_num> &other) const;

    bool operator<(UINT other) const;

    template<UINT _>
    friend bool operator<(UINT first, const LongInt<bits_num> &other);

    bool operator<=(const LongInt<bits_num> &other) const;

    bool operator<=(UINT other) const;

    template<UINT _>
    friend LongInt<bits_num> operator<=(UINT first, const LongInt<bits_num> &other);


    LongInt<bits_num> operator|(const LongInt<bits_num> &other) const;

    LongInt<bits_num> operator|(UINT other) const;

    LongInt<bits_num> &operator|=(const LongInt<bits_num> &other);

    LongInt<bits_num> &operator|=(UINT other);

    template<UINT _>
    friend LongInt<bits_num> operator|(UINT first, const LongInt<bits_num> &other);


    LongInt<bits_num> operator&(const LongInt<bits_num> &other) const;

    LongInt<bits_num> operator&(UINT other) const;

    LongInt<bits_num> &operator&=(const LongInt<bits_num> &other);

    LongInt<bits_num> &operator&=(UINT other);

    template<UINT _>
    friend LongInt<bits_num> operator&(UINT first, const LongInt<bits_num> &other);


    LongInt<bits_num> operator^(const LongInt<bits_num> &other) const;

    LongInt<bits_num> operator^(UINT other) const;

    LongInt<bits_num> &operator^=(const LongInt<bits_num> &other);

    LongInt<bits_num> &operator^=(UINT other);

    template<UINT _>
    friend LongInt<bits_num> operator^(UINT first, const LongInt<bits_num> &other);


    LongInt<bits_num> operator<<(UINT other) const;

    LongInt<bits_num> operator>>(UINT other) const;

    LongInt<bits_num> operator<<=(UINT other);

    LongInt<bits_num> operator>>=(UINT other);

    LongInt<bits_num> operator~() const;

    LongInt<bits_num> &operator++();

    const LongInt<bits_num> operator++(int);

    LongInt<bits_num> &operator--();

    const LongInt<bits_num> operator--(int);

    LongInt<bits_num> &operator=(const LongInt<bits_num> &other) = default;

    LongInt<bits_num> &operator=(UINT other);

    LongInt<bits_num> &operator=(int other);

    LongInt<bits_num> operator-() const;

    UINT &operator[](UINT index);

    UINT operator[](UINT index) const;
};

template<UINT bits_num>
LongInt<bits_num> operator+(UINT first, const LongInt<bits_num> &other) {
    return other + first;
}

template<UINT bits_num>
LongInt<bits_num> operator-(UINT first, const LongInt<bits_num> &other) {
    return LongInt<bits_num>(first) - other;
}

template<UINT bits_num>
LongInt<bits_num> operator*(UINT first, const LongInt<bits_num> &other) {
    return other * first;
}


template<UINT bits_num>
bool operator==(UINT first, const LongInt<bits_num> &other) {
    return other == first;
}

template<UINT bits_num>
bool operator!=(UINT first, const LongInt<bits_num> &other) {
    return other != first;
}


template<UINT bits_num>
bool operator>(UINT first, const LongInt<bits_num> &other) {
    return other <= first;
}

template<UINT bits_num>
bool operator>=(UINT first, const LongInt<bits_num> &other) {
    return other <= first;
}

template<UINT bits_num>
bool operator<(UINT first, const LongInt<bits_num> &other) {
    return other >= first;
}

template<UINT bits_num>
bool operator<=(UINT first, const LongInt<bits_num> &other) {
    return other >= first;
}

template<UINT bits_num>
LongInt<bits_num> operator|(UINT first, const LongInt<bits_num> &other) {
    return other | first;
}

template<UINT bits_num>
LongInt<bits_num> operator&(UINT first, const LongInt<bits_num> &other) {
    return other & first;
}

template<UINT bits_num>
LongInt<bits_num> operator^(UINT first, const LongInt<bits_num> &other) {
    return other ^ first;
}

#endif //ELIPTIC_CURVES_CYPHER_LONGINT_H
