//
// Created by ky3he4ik on 08/11/19.
//

#include "LongInt.h"
#include <iostream>

#define CHECK_SIZES(b) { \
    if (bits_num != (b).bits_num) \
        throw std::invalid_argument( \
            std::string("Dimensions didn't match! (") + LongInt(BITS_BASE, bits_num).to_string() + " != " + \
                    LongInt(BITS_BASE, (b).bits_num).to_string()); \
}

LongInt::LongInt(UINT _bits_num, const UINT *init) : bits_num(_bits_num) {
//        value = new UINT[ARR_SIZE];
    value.resize(ARR_SIZE);
    FOR_IND(i)value[i] = init[i];
}

LongInt::LongInt(UINT _bits_num) : bits_num(_bits_num) {
    value.resize(ARR_SIZE);
    FOR_IND(i)value[i] = 0;
}


LongInt::LongInt(UINT _bits_num, UINT init) : bits_num(_bits_num) {
    value.resize(ARR_SIZE);
    value[LAST] = init;
}


LongInt::LongInt(UINT _bits_num, const std::string &str, int radix) : bits_num(_bits_num) {
    sign = str[0] != '-';
    value.resize(ARR_SIZE);
    for (auto &c: str) {
        int i = ctoi(c);
        if (i != -1 && i < radix)
            *this = *this * radix + i;
    }
}

LongInt LongInt::changeLen(UINT new_bits_num) const {
    LongInt res(new_bits_num);
    UINT n_len = res.get_len();
    for (UINT i = 0; i < std::min(n_len, len); i++)
        if (n_len < len)
            res[i] = value[i + len - n_len];
        else
            res[i + n_len - len] = value[i];
    res.sign = sign;
    return res;
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


std::string LongInt::to_string(UINT radix, bool group) const {
    std::string res;
    LongInt tmp(*this);
    UINT l = 1;
    while (tmp != UINT_0) {
        res.push_back(itoc(tmp % radix));
        tmp /= (radix);
        if (group && l++ % 4 == 0)
            res.push_back('_');
    }
    if (!res.empty() && res.back() == '_')
        res.pop_back();
    if (!sign && !res.empty())
        res.push_back('-');
    reverse(res.begin(), res.end());
    if (res.empty())
        res = "0";
    return res;
}


UINT LongInt::last_item() const {
    return value[LAST];
}


bool LongInt::get_bit(UINT pos) const {
    return (value[pos / BITS_BASE] >> ((BITS_BASE - (pos % BITS_BASE) - 1) % BITS_BASE)) & (UINT) 1;
}


UINT LongInt::get_len() const {
    return len;
}


void LongInt::set_bit(UINT pos, bool val) {
    UINT mask = UINT_MAX - ((UINT) 1 << (pos % BITS_BASE));
    value[pos / BITS_BASE] = (value[pos / BITS_BASE] & mask) | (UINT) (val << (pos % BITS_BASE));
}


UINT LongInt::get_bits_count() const {
    return len * BITS_BASE;
}

UINT LongInt::get_actual_bits() const {
    for (UINT i = 0; i < get_bits_count(); i++)
        if (get_bit(i))
            return get_bits_count() - i;

    return 0;
}

LongInt LongInt::operator+(const LongInt &other) const {
    LongInt res(*this);
    res += other;
    return res;
}


LongInt LongInt::operator+(UINT other) const {
    LongInt res(*this);
    res += other;
    return res;
}


LongInt &LongInt::operator+=(const LongInt &other) {
    CHECK_SIZES(other)
    if (sign) {
        if (!other.sign)
            return *this -= -other.sign;
    } else if (!other.sign) {
        *this = other - -(*this);
        return *this;
    }

    uint64_t buf = 0;
    FOR_IND_REVERSE(i) {
        buf = buf + other[i] + (uint64_t) value[i];
        value[i] = buf;
        buf >>= BITS_BASE;
    }
    return *this;
}


LongInt &LongInt::operator+=(UINT other) {
    uint64_t buf = 0;
    FOR_IND_REVERSE(i) {
        buf = buf + (uint64_t) value[i];
        if (i == len - 1)
            buf += other;
        value[i] = buf;
        buf >>= BITS_BASE;
    }
    return *this;
}


LongInt LongInt::operator-(const LongInt &other) const {
    LongInt res(*this);
    res -= other;
    return res;
}


LongInt LongInt::operator-(UINT other) const {
    LongInt res(*this);
    res -= other;
    return res;
}


LongInt &LongInt::operator-=(const LongInt &other) {
    if (other > *this)
        return *this = -(other - *this);

    CHECK_SIZES(other)
    if (sign) {
        if (!other.sign)
            return *this += -other.sign;
    } else if (!other.sign) {
        *this = -(*this + other);
        return *this;
    }

    uint64_t buf = 0;
    FOR_IND_REVERSE(i) {
        buf = (uint64_t) value[i] - other[i] - buf;
        value[i] = buf;
        buf = (buf >> BITS_BASE) & (UINT) 1;
    }
    return *this;
}


LongInt &LongInt::operator-=(UINT other) {
    if (other > *this)
        return *this = -(other - *this);

    uint64_t buf = 0;
    FOR_IND_REVERSE(i) {
        buf = (uint64_t) value[i] - buf;
        if (i == len - 1)
            buf -= other;
        value[i] = buf;
        buf = (buf >> BITS_BASE) & (UINT) 1;
    }
    return *this;
}


LongInt LongInt::operator/(const LongInt &other) const {
    CHECK_SIZES(other)
    if (other == UINT_0)
        throw std::invalid_argument("Cannot divide by zero");

    LongInt remainder(bits_num);
    LongInt res(bits_num);
    // algorithm from wiki
    for (UINT i = get_bits_count() - 1; i < get_bits_count(); i--) {
        remainder = (remainder << 1) | get_bit(get_bits_count() - i - 1);
        res = (res << 1) | (remainder >= other);
//        res.set_bit(i, remainder >= other);
        if (remainder >= other)
            remainder -= other;
    }
    res.sign = sign == other.sign;
    return res;
}


LongInt LongInt::operator/(UINT other) const {
    LongInt res(*this);
    res /= other;
    return res;
}


LongInt &LongInt::operator/=(const LongInt &other) {
    *this = *this / other;
    return *this;
}


LongInt &LongInt::operator/=(UINT other) {
    uint64_t buf = 0;
    FOR_IND(i) {
        buf = (buf << BITS_BASE) + value[i];
        value[i] = buf / other;
        buf %= other;
    }
    // not the best solution but it's easy to write and it works!
    return *this;
}


LongInt LongInt::operator*(const LongInt &other) const {
    LongInt res(bits_num);
    FOR_IND(i)res += (other * value[i]) << (get_bits_count() - (i + 1) * BITS_BASE);
    res.sign = sign == other.sign;
    return res;
}


LongInt LongInt::operator*(UINT other) const {
    LongInt res(*this);
    return res *= other;
}


LongInt &LongInt::operator*=(const LongInt &other) {
    CHECK_SIZES(other)
    return *this = *this * other;
}


LongInt &LongInt::operator*=(UINT other) {
    uint64_t buf = 0;
    FOR_IND_REVERSE(i) {
        buf = value[i] * (uint64_t) other + buf;
        value[i] = buf;
        buf >>= BITS_BASE;
    }
    return *this;
}


LongInt LongInt::operator%(const LongInt &other) const {
    CHECK_SIZES(other)
    if (other == UINT_0)
        return *this;
    if (other > *this)
        return LongInt(*this);

    LongInt remainder(bits_num);
    // algorithm from wiki
    for (UINT i = get_bits_count() - 1; i < get_bits_count(); i--) {
        remainder = (remainder << 1) | get_bit(get_bits_count() - i - 1);
//        res.set_bit(i, remainder >= other);
        if (remainder >= other)
            remainder -= other;
    }
    remainder.sign = sign;
    return remainder;
}


UINT LongInt::operator%(UINT other) const {
    if (other == UINT_0)
        return last_item();
    uint64_t buf = 0;
    FOR_IND(i)buf = ((buf << BITS_BASE) + value[i]) % other;
    return (UINT) buf;
}


LongInt &LongInt::operator%=(const LongInt &other) {
    return *this = *this % other;
}


LongInt &LongInt::operator%=(UINT other) {
    uint64_t buf = 0;
    FOR_IND(i) {
        buf = ((buf << BITS_BASE) + value[i]) % other;
        value[i] = 0;
    }
    value[LAST] = buf;
    return *this;
}


bool LongInt::operator==(const LongInt &other) const {
    CHECK_SIZES(other)
    if (sign != other.sign) {
        if (other != 0 && *this != 0)
            return false;
        return (other == 0) == (*this == 0);
    }
    FOR_IND(i) {
        if (value[i] != other[i])
            return false;
    }
    return true;
}


bool LongInt::operator==(UINT other) const {
    for (UINT i = 0; i < len - 1; i++)
        if (value[i] != 0)
            return false;
    return value[len - 1] == other;
}


bool LongInt::operator!=(const LongInt &other) const {
    return !(*this == other);
}


bool LongInt::operator!=(UINT other) const {
    return !(*this == other);
}


bool LongInt::operator>(const LongInt &other) const {
    if (sign != other.sign) {
        if (other != 0 && *this != 0)
            return sign;
        if (sign)
            return (other == 0) != (*this == 0);
        return false;
    }
    if (!sign)
        return -(*this) < -other;
    FOR_IND(i) {
        if (value[i] > other[i])
            return true;
        else if (value[i] < other[i])
            return false;
    }
    return false;
}


bool LongInt::operator>(UINT other) const {
    if (!sign)
        return false;
    for (UINT i = 0; i < len - 1; i++)
        if (value[i])
            return true;
    return value[len - 1] > other;
}


bool LongInt::operator>=(const LongInt &other) const {
    return *this == other || *this > other;
}


bool LongInt::operator>=(UINT other) const {
    return *this == other || *this > other;
}


bool LongInt::operator<(const LongInt &other) const {
    if (sign != other.sign) {
        if (other != 0 && *this != 0)
            return !sign;
        if (!sign)
            return (other == 0) != (*this == 0);
        return false;
    }
    if (!sign)
        return -(*this) > -other;
    FOR_IND(i) {
        if (value[i] < other[i])
            return true;
        else if (value[i] > other[i])
            return false;
    }
    return false;
}


bool LongInt::operator<(UINT other) const {
    if (!sign)
        return *this == 0 && other != 0;
    for (UINT i = 0; i < len - 1; i++)
        if (value[i])
            return false;
    return value[len - 1] < other;
}


bool LongInt::operator<=(const LongInt &other) const {
    return *this == other || *this < other;
}


bool LongInt::operator<=(UINT other) const {
    return *this == other || *this < other;
}


LongInt LongInt::operator|(const LongInt &other) const {
    LongInt res(*this);
    return res |= other;
}


LongInt LongInt::operator|(UINT other) const {
    LongInt res(*this);
    res[LAST] |= other;
    return res;
}


LongInt &LongInt::operator|=(const LongInt &other) {
    CHECK_SIZES(other)
    FOR_IND(i) value[i] |= other[i];
    return *this;
}


LongInt &LongInt::operator|=(UINT other) {
    value[LAST] |= other;
    return *this;
}


LongInt LongInt::operator&(const LongInt &other) const {
    LongInt res(*this);
    return res &= other;
}


LongInt LongInt::operator&(UINT other) const {
    LongInt res(bits_num);
    res[LAST] = value[LAST] & other;
    return res;
}


LongInt &LongInt::operator&=(const LongInt &other) {
    CHECK_SIZES(other)
    FOR_IND(i)value[i] &= other[i];
    return *this;
}


LongInt &LongInt::operator&=(UINT other) {
    FOR_IND(i)value[i] = 0;
    value[LAST] &= other;
    return *this;
}


LongInt LongInt::operator^(const LongInt &other) const {
    LongInt res(*this);
    return res ^= other;
}


LongInt LongInt::operator^(UINT other) const {
    LongInt res(*this);
    res[LAST] ^= other;
    return res;
}


LongInt &LongInt::operator^=(const LongInt &other) {
    CHECK_SIZES(other)
    FOR_IND(i)value[i] ^= other[i];
    return *this;
}


LongInt &LongInt::operator^=(UINT other) {
    value[LAST] ^= other;
    return *this;
}


LongInt LongInt::operator<<(UINT other) const {
    LongInt res(*this);
    return res <<= other;
}


LongInt LongInt::operator<<=(UINT other) {
    UINT shift_b = other / BITS_BASE, shift_s = other % BITS_BASE;
    FOR_IND(i) {
        UINT ni = i + shift_b;
        if (ni < len) {
            value[i] = value[ni] << shift_s;
            if (ni != len - 1 && shift_s != 0)
                value[i] |= value[ni + 1] >> (BITS_BASE - shift_s);
        } else
            value[i] = 0;
    }
    return *this;
}


LongInt LongInt::operator>>(UINT other) const {
    LongInt res(*this);
    res >>= other;
    return res;
}


LongInt LongInt::operator>>=(UINT other) {
    UINT shift_b = other / BITS_BASE, shift_s = other % BITS_BASE;
    FOR_IND_REVERSE(i) {
        UINT ni = i - shift_b;
        if (ni <= i) {
            value[i] = value[ni] >> shift_s;
            if (ni != 0 && shift_s != 0)
                value[i] |= value[ni - 1] << (BITS_BASE - shift_s);
        } else
            value[i] = 0;
    }
    return *this;
}


LongInt LongInt::operator~() const {
    LongInt res(bits_num);
    FOR_IND(i)res[i] = ~value[i];
    return res;
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


LongInt &LongInt::operator=(const LongInt &other) {
    CHECK_SIZES(other)
    FOR_IND(i)value[i] = other[i];
    return *this;
}


LongInt &LongInt::operator=(UINT other) {
    FOR_IND(i)value[i] = 0;
    value[LAST] = other;
    sign = true;
    return *this;
}


LongInt &LongInt::operator=(int other) {
    FOR_IND(i)value[i] = 0;
    for (UINT ps = sizeof(other) / (BITS_BASE); ps != 0; ps--)
        value[ps] = ((unsigned) other >> (BITS_BASE * ps)) & ((UINT) -1);
    value[LAST] = (unsigned) other & ((UINT) -1);
    sign = other > 0;
    return *this;
}


UINT &LongInt::operator[](UINT index) {
    return this->value[index];
}


UINT LongInt::operator[](UINT index) const {
    return this->value[index];
}


LongInt LongInt::operator-() const {
    LongInt res(*this);
    res.sign = !sign;
    return res;
}

// fast pow mod (need to fast check is point on the curve on not)
// Calculate (x ** y) % z efficiently.
LongInt LongInt::fast_pow_mod(const LongInt &y, const LongInt &z) const {
    LongInt res(y.get_bits_count(), 1);
    LongInt tmp(*this);
    for (UINT i = 0; i < y.get_bits_count(); i++) {
        if (y.get_bit(y.get_bits_count() - i - 1))
            res = (res * tmp) % z;
        tmp = (tmp * tmp) % z;
    }
    return res;
}
