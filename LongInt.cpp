//
// Created by ky3he4ik on 08/11/19.
//

#include "LongInt.h"
#include <iostream>

#define FIX_SIZES(b) { \
    auto ol = (b).get_actual_bits(); \
    if (ol != UINT_MAX && ol > get_bits_count()) \
        (*this) = changeLen(ol); \
}

#define FIX_SIZE_UINT { \
    if (len == 0) { \
        value.resize(1); \
        len = 1; \
    } \
};

#define PRE_FOR_DIFF_SIZES_THIS(other, i) \
    if ((other).len < len) \
        for (UINT (i) = 0; (i) < len - (other).len; (i)++)

#define PRE_FOR_DIFF_SIZES_OTHER(other, i) \
    if ((other).len > len) \
        for (UINT (i) = 0; (i) < (other).len - len; (i)++)

#define FOR_DIFF_SIZES(other, t_it, l_it) \
    UINT start_ind = 0;\
    if ((other).len > len)\
        start_ind = (other).len - len;\
    for (UINT (t_it) = start_ind + len - (other).len, (l_it) = start_ind; (l_it) < (other).len; (l_it)++, (t_it)++)

#define FOR_DIFF_SIZES_REVERSE(other, t_it, l_it) \
    UINT start_ind = 0;\
    if ((other).len > len)\
        start_ind = (other).len - len;\
    for (UINT (t_it) = len - 1, (l_it) = (other).len - 1; (l_it) < (other).len && (l_it) >= start_ind; (l_it)--, (t_it)--)


LongInt::LongInt(UINT _bits_num, const UINT *init) : bits_num(_bits_num) {
//        value = new UINT[ARR_SIZE];
    value.resize(ARR_SIZE);
    if (init)
        FOR_IND(i) {
            value[i] = init[i];
        }
}

LongInt::LongInt(UINT _bits_num) : bits_num(_bits_num) {
    value.resize(ARR_SIZE);
    FOR_IND(i)value[i] = 0;
}


LongInt::LongInt(UINT _bits_num, UINT init) : bits_num(_bits_num) {
    value.resize(ARR_SIZE);
    value[LAST] = init;
}


LongInt::LongInt(const std::string &str, int radix): bits_num(0) {
    sign = str[0] != '-';
    for (auto &c: str) {
        int i = ctoi(c);
        if (i != -1 && i < radix)
            *this = *this * radix + i;
    }
}

LongInt LongInt::changeLen(UINT new_bits_num) const {
    if (new_bits_num == get_bits_count())
        return *this;
    LongInt res(new_bits_num);
    UINT n_len = res.get_len();
    for (UINT i = 0; i < std::min(n_len, len); i++)
        if (n_len < len)
            res[i] = value[i + len - n_len];
        else
            res[i + n_len - len] = get(i);
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
        res.push_back(itoc(tmp.abs() % radix));
        tmp /= (radix);
        if (group && l++ % 4 == 0)
            res.push_back('_');
    }
    if (res.empty())
        res.push_back('0');
    else {
        if (res.back() == '_')
            res.pop_back();
        if (!sign)
            res.push_back('-');
    }
    reverse(res.begin(), res.end());
    return res;
}


UINT LongInt::last_item() const {
    return get(LAST);
}


bool LongInt::get_bit(UINT pos) const {
    if (len == 0)
        return 0;
    return (value[pos / BITS_BASE] >> ((BITS_BASE - (pos % BITS_BASE) - 1) % BITS_BASE)) & (UINT) 1;
}


UINT LongInt::get_len() const {
    return len;
}


void LongInt::set_bit(UINT pos, bool val) {
    UINT mask = UINT_MAX - ((UINT) 1 << (BITS_BASE - pos % BITS_BASE - 1));
    value[pos / BITS_BASE] = (value[pos / BITS_BASE] & mask) | (UINT) (val << (BITS_BASE - pos % BITS_BASE - 1));
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
    FIX_SIZES(other)
    if (sign) {
        if (!other.sign)
            return *this -= -other.sign;
    } else if (other.sign) {
        *this = other - -(*this);
        return *this;
    }

    uint64_t buf = 0;
    FOR_DIFF_SIZES_REVERSE(other, i, j) {
        buf += other[j] + (uint64_t) value[i];
        value[i] = buf;
        buf >>= BITS_BASE;
    }
    if (len > other.len)
        for (UINT i = len - other.len - 1; i < len - other.len; i--) {
            buf += value[i];
            value[i] = buf;
            buf >>= BITS_BASE;
        }
    return *this;
}


LongInt &LongInt::operator+=(UINT other) {
    FIX_SIZE_UINT
    uint64_t buf = other;
    FOR_IND_REVERSE(i) {
        buf += value[i];
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
    FIX_SIZES(other)
    if (other > *this)
        return *this = -(other - *this);

    if (sign) {
        if (!other.sign)
            return *this += -other;
    } else if (!other.sign) {
        *this = -(*this + other);
        return *this;
    }

    uint64_t buf = 0;
    FOR_DIFF_SIZES_REVERSE(other, i, j) {
        buf = (uint64_t) value[i] - other[j] - buf;
        value[i] = buf;
        buf = (buf >> BITS_BASE) & (UINT) 1;
    }

    if (other.len < len) \

        for (UINT i = len - other.len - 1; i < len - other.len; i--) {
            buf = (uint64_t) value[i] - buf;
            value[i] = buf;
            buf = (buf >> BITS_BASE) & (UINT) 1;
        }
    return *this;
}


LongInt &LongInt::operator-=(UINT other) {
    FIX_SIZE_UINT
    if (other > *this)
        return *this = -(other - *this);

    uint64_t buf = other;
    FOR_IND_REVERSE(i) {
        buf = (uint64_t) value[i] - buf;
        value[i] = buf;
        buf = (buf >> BITS_BASE) & (UINT) 1;
    }
    return *this;
}


LongInt LongInt::operator/(const LongInt &other) const {
    if (other == UINT_0)
        throw std::invalid_argument("Cannot divide by zero");

    LongInt otherAbs = other.abs();
    if (*this < other)
        return LongInt(get_bits_count());
    auto bits = std::max(bits_num, other.get_bits_count());
    LongInt remainder(bits);
    LongInt res(bits);
    // algorithm from wiki
    for (UINT i = get_bits_count() - 1; i < get_bits_count(); i--) {
        remainder = (remainder << 1) | get_bit(get_bits_count() - i - 1);
        res <<= 1;
//        res.set_bit(i, remainder >= other);
        if (remainder >= otherAbs) {
            remainder -= otherAbs;
            res.set_bit(res.get_bits_count() - 1);
        }
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
    FIX_SIZE_UINT
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
    LongInt res(std::max(get_bits_count(), other.get_bits_count()));
    FOR_IND(i) {
        res += ((other * get(i)).shift_left_no_overflow(get_bits_count() - (i + 1) * BITS_BASE)).abs();
    }
    res.sign = sign == other.sign;
    return res;
}


LongInt LongInt::operator*(UINT other) const {
    LongInt res(*this);
    return res *= other;
}


LongInt &LongInt::operator*=(const LongInt &other) {
    FIX_SIZES(other)
    return *this = *this * other;
}


LongInt &LongInt::operator*=(UINT other) {
    FIX_SIZE_UINT
    uint64_t buf = 0;
    FOR_IND_REVERSE(i) {
        buf = value[i] * (uint64_t) other + buf;
        value[i] = buf;
        buf >>= BITS_BASE;
    }
    if (buf != 0) {
        *this = changeLen(get_bits_count() + BITS_BASE);
        value[0] = buf;
    }
    return *this;
}


LongInt LongInt::operator%(const LongInt &other) const {
    if (other == UINT_0)
        return *this;
    if (*this == 0)
        return LongInt(bits_num);
    if (other.abs() > this->abs()) {
        if (sign == other.sign)
            return *this;
        else
            return (*this) + other;
    }

    LongInt remainder(bits_num);
    // algorithm from wiki
    for (UINT i = get_bits_count() - 1; i < get_bits_count(); i--) {
        remainder = (remainder << 1) | get_bit(get_bits_count() - i - 1);
//        res.set_bit(i, remainder >= other);
        if (remainder >= other)
            remainder -= other;
    }
    if (sign != other.sign && remainder != 0)
        remainder = other.abs() - remainder.abs();
    remainder.sign = other.sign;
    return remainder;
}


UINT LongInt::operator%(UINT other) const {
    if (other == UINT_0)
        return last_item();
    uint64_t buf = 0;
    FOR_IND(i)buf = ((buf << BITS_BASE) + get(i)) % other;
    if (!sign && buf != 0)
        return other - (UINT) buf;
    return (UINT) buf;
}


LongInt &LongInt::operator%=(const LongInt &other) {
    return *this = *this % other;
}


LongInt &LongInt::operator%=(UINT other) {
    FIX_SIZE_UINT
    uint64_t buf = 0;
    FOR_IND(i) {
        buf = ((buf << BITS_BASE) + value[i]) % other;
        value[i] = 0;
    }
    value[LAST] = buf;
    if (!sign && buf != 0)
        return *this = other - *this;
    return *this;
}


bool LongInt::operator==(const LongInt &other) const {
    if (sign != other.sign) {
        if (other != 0 && *this != 0)
            return false;
        return (other == 0) == (*this == 0);
    }
    PRE_FOR_DIFF_SIZES_THIS(other, i)
            if (value[i])
                return false;
    PRE_FOR_DIFF_SIZES_OTHER(other, i)
            if (other[i])
                return false;
    FOR_DIFF_SIZES(other, i, j)if (get(i) != other[j])
            return false;
    return true;
}


bool LongInt::operator==(UINT other) const {
    if (len == 0)
        return other == 0;
    for (UINT i = 0; i < len - 1; i++)
        if (get(i) != 0)
            return false;
    return last_item() == other;
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
    PRE_FOR_DIFF_SIZES_THIS(other, i)
            if (value[i])
                return true;
    PRE_FOR_DIFF_SIZES_OTHER(other, i)
            if (other[i])
                return false;
    FOR_DIFF_SIZES(other, i, j)
        if (get(i) > other[j])
            return true;
        else if (get(i) < other[j])
            return false;
    return false;
}


bool LongInt::operator>(UINT other) const {
    if (!sign)
        return false;
    if (len == 0)
        return false;
    for (UINT i = 0; i < len - 1; i++)
        if (get(i))
            return true;
    return last_item() > other;
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
    PRE_FOR_DIFF_SIZES_THIS(other, i)if (value[i])
                return false;
    PRE_FOR_DIFF_SIZES_OTHER(other, i)if (other[i])
                return true;
    FOR_DIFF_SIZES(other, i, j)
        if (get(i) > other[j])
            return false;
        else if (get(i) < other[j])
            return true;
    return false;
}


bool LongInt::operator<(UINT other) const {
    if (!sign)
        return other != 0 || *this != 0;
    if (len == 0)
        return other != 0;
    for (UINT i = 0; i < len - 1; i++)
        if (get(i))
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
    FIX_SIZES(other)
    FOR_DIFF_SIZES(other, i, j)value[i] |= other[j];
    return *this;
}


LongInt &LongInt::operator|=(UINT other) {
    FIX_SIZE_UINT
    value[LAST] |= other;
    return *this;
}


LongInt LongInt::operator&(const LongInt &other) const {
    LongInt res(*this);
    return res &= other;
}


LongInt LongInt::operator&(UINT other) const {
    LongInt res(*this);
    return res &= other;
}


LongInt &LongInt::operator&=(const LongInt &other) {
    FIX_SIZES(other)
    PRE_FOR_DIFF_SIZES_THIS(other, i)value[i] = 0;
    FOR_DIFF_SIZES(other, i, j)value[i] &= other[j];
    return *this;
}


LongInt &LongInt::operator&=(UINT other) {
    FIX_SIZE_UINT
    for (UINT i = 0; i < len - 1; i++)
        value[i] = 0;
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
    FIX_SIZES(other)
    FOR_DIFF_SIZES(other, i, j)value[i] ^= other[j];
    return *this;
}


LongInt &LongInt::operator^=(UINT other) {
    FIX_SIZE_UINT
    value[LAST] ^= other;
    return *this;
}


LongInt LongInt::operator<<(UINT other) const {
    LongInt res(*this);
    return res <<= other;
}


LongInt LongInt::operator<<=(UINT other) {
    FIX_SIZE_UINT
    UINT shift_b = other / BITS_BASE, shift_s = other % BITS_BASE;
    FOR_IND(i) {
        UINT ni = i + shift_b;
        if (ni < len) {
            value[i] = get(ni) << shift_s;
            if (ni != len - 1 && shift_s != 0)
                value[i] |= get(ni + 1) >> (BITS_BASE - shift_s);
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
    FIX_SIZE_UINT
    UINT shift_b = other / BITS_BASE, shift_s = other % BITS_BASE;
    FOR_IND_REVERSE(i) {
        UINT ni = i - shift_b;
        if (ni <= i) {
            value[i] = get(ni) >> shift_s;
            if (ni != 0 && shift_s != 0)
                value[i] |= get(ni - 1) << (BITS_BASE - shift_s);
        } else
            value[i] = 0;
    }
    return *this;
}


LongInt LongInt::operator~() const {
    LongInt res(bits_num);
    FOR_IND(i)res[i] = ~get(i);
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

LongInt &LongInt::operator=(UINT other) {
    FIX_SIZE_UINT
    FOR_IND(i)value[i] = 0;
    value[LAST] = other;
    sign = true;
    return *this;
}


LongInt &LongInt::operator=(int other) {
    FIX_SIZE_UINT
    FOR_IND(i)value[i] = 0;
    for (UINT ps = sizeof(other) / (BITS_BASE); ps != 0; ps--)
        value[ps] = ((unsigned) other >> (BITS_BASE * ps)) & UINT_MAX;
    value[LAST] = (unsigned) other & UINT_MAX;
    sign = other > 0;
    return *this;
}


UINT &LongInt::operator[](UINT index) {
    return this->value[index];
}


UINT LongInt::operator[](UINT index) const {
    if (index >= len)
        return 0;
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
    LongInt tmp(changeLen(get_bits_count() << 1));
    for (UINT i = 0; i < y.get_bits_count(); i++) {
        if (y.get_bit(y.get_bits_count() - i - 1))
            res = (res * tmp) % z;
        tmp = (tmp * tmp) % z;
    }
    return res.changeLen(z.get_bits_count());
}

// fast pow mod (need to fast check is point on the curve on not)
// Calculate (x ** y) % z efficiently.
LongInt LongInt::fast_pow_mod(UINT y, const LongInt &z) const {
    LongInt res(z.get_bits_count() << 1, 1);
    LongInt tmp(changeLen(get_bits_count() << 1));
    while (y) {
        if (y & 1)
            res = (res * tmp) % z;
        tmp = (tmp * tmp) % z;
        y >>= 1;
    }
    return res.changeLen(z.get_bits_count());
}

// fast pow mod (need to fast check is point on the curve on not)
// Calculate (x ** y) % z efficiently.
UINT LongInt::fast_pow_mod(const LongInt &y, UINT z) const {
    UINT res = 1;
    UINT tmp = last_item();
    for (UINT i = 0; i < y.get_bits_count(); i++) {
        if (y.get_bit(y.get_bits_count() - i - 1))
            res = (res * tmp) % z;
        tmp = (tmp * tmp) % z;
    }
    return res;
}

// fast pow mod (need to fast check is point on the curve on not)
// Calculate (x ** y) % z efficiently.
UINT LongInt::fast_pow_mod(UINT y, UINT z) const {
    UINT res = 1;
    UINT tmp = last_item();
    while (y) {
        if (y & 1)
            res = (res * tmp) % z;
        tmp = (tmp * tmp) % z;
        y >>= 1;
    }
    return res;
}

LongInt LongInt::get_random(UINT bits_num) {
    static std::random_device random;
    LongInt res(bits_num);
    for (UINT i = 0; i < bits_num; i++)
        res.set_bit(i, random() & 1);
    return res;
}

LongInt LongInt::abs() const {
    return (this->sign) ? *this : -(*this);
}

UINT LongInt::get(UINT ind) const {
    if (ind >= len)
        return 0;
    return value[ind];
}

LongInt LongInt::shift_left_no_overflow(UINT other) const {
    UINT shift_b = other / BITS_BASE, shift_s = other % BITS_BASE;
    LongInt res(0);
    UINT a_b = get_actual_bits() + other;
    if (a_b > get_bits_count())
        res = changeLen(a_b);
    else
        res = *this;

    return res <<= other;
}
