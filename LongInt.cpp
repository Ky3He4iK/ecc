//
// Created by ky3he4ik on 08/11/19.
//

#include "LongInt.h"
#include <iostream>

template<UINT bits_num>
LongInt<bits_num>::LongInt() {
//    value = new UINT[ARR_SIZE];
    FOR_IND(i)value[i] = 0;
}

template<UINT bits_num>
LongInt<bits_num>::LongInt(UINT init) {
//    value = new UINT[ARR_SIZE];
    value[LAST] = init;
}


template<UINT bits_num>
LongInt<bits_num>::LongInt(const LongInt<bits_num> &other) {
//    value = new UINT[ARR_SIZE];
    for (UINT i = 0; i < ARR_SIZE; i++)
        value[i] = (other[i]);
}

template<UINT bits_num>
LongInt<bits_num>::LongInt(const std::string &str, int radix) {
//    value = new UINT[ARR_SIZE];
    for (auto &c: str) {
        int i = ctoi(c);
        if (i != -1 && i < radix) {
            *this *= radix;
            *this += i;
        }
    }
}

template<UINT bits_num>
template<UINT old_bits_num>
LongInt<bits_num> LongInt<bits_num>::changeLen(const LongInt<old_bits_num> &other) {
    LongInt<bits_num> res;
    UINT shift = ARR_SIZE - (old_bits_num / BITS_BASE + ((old_bits_num % BITS_BASE == 0) ? 0 : 1));
    UINT shift_n = (old_bits_num / BITS_BASE + ((old_bits_num % BITS_BASE == 0) ? 0 : 1)) - ARR_SIZE;
    if (shift > shift_n) {
        for (UINT i = 0; i < ARR_SIZE; i++)
            res[i] = other[i + shift_n];
    } else
        for (UINT i = 0; i < ARR_SIZE - shift; i++) {
            res[i + shift] = other[i];
        }
    return res;
}


template<UINT bits_num>
bool
LongInt<bits_num>::mult_cmp(const LongInt<bits_num> &a, const LongInt<bits_num> &b, const LongInt<bits_num> &third) {
    LongInt o_r((UINT) 0);
    std::pair<LongInt<bits_num>, bool> res;
    bool overflow = false;
    bool ans = false;
    FOR_IND(i) {
        res = shift_overflow(a * b[i], (ARR_SIZE - i - 1) * BITS_BASE);
        overflow |= res.first + o_r < o_r || ((res.first == 0) != (b[i] == 0)) || res.second;
        if (overflow || ans)
            return true;

        o_r += res.first;
        ans |= o_r > third;
    }
    return ans || overflow;
}


template<UINT bits_num>
std::pair<LongInt<bits_num>, bool> LongInt<bits_num>::shift_overflow(const LongInt<bits_num> &a, UINT other) {
    UINT shift_b = other / BITS_BASE, shift_s = other % BITS_BASE;
    LongInt res;
    bool of = false;
    FOR_IND_REVERSE(i) {
        UINT ni = i + shift_b;
        if (ni < ARR_SIZE) {
            res[i] = (a[ni] << shift_s) | ((ni == ARR_SIZE - 1 || shift_s == 0) ? (UINT) 0 : (UINT) (res[ni + 1]
                    >> ((BITS_BASE - shift_s) % BITS_BASE)));
        } else
            res[i] = 0;
    }
    for (UINT i = 0; i < shift_b + ((shift_s == 0) ? 0 : 1); i++) {
        of |= a[i] >> ((BITS_BASE - shift_s) % BITS_BASE);
    }
    return std::make_pair(res, of);
}


template<UINT bits_num>
int LongInt<bits_num>::ctoi(char c) {
    if (c <= '9' && c >= '0')
        return c - '0';
    if (c <= 'Z' && c >= 'A')
        return c - 'A' + 10;
    if (c <= 'z' && c >= 'a')
        return c - 'a' + 10;
    return -1;
}

template<UINT bits_num>
char LongInt<bits_num>::itoc(int i) const {
    return (i < 10) ? i + '0' : i + 'A' - 10;
}


template<UINT bits_num>
std::string LongInt<bits_num>::to_string(UINT radix, bool group) const {
    std::string res;
    LongInt tmp(*this);
    if (group) {
        UINT l = 1;
        for (UINT i = 0; i < get_bits_count() || tmp != 0; i++) {
            res.push_back(itoc(tmp % radix));
            tmp /= (radix);
            if (l % 4 == 0 && i != get_bits_count() - 1)
                res.push_back('_');
            l++;
        }
    } else {
        while (tmp != 0) {
            res.push_back(itoc(tmp % radix));
            tmp /= (radix);
        }
    }
    reverse(res.begin(), res.end());
    if (res.empty())
        res = "0";
    return res;
}

template<UINT bits_num>
UINT LongInt<bits_num>::last_item() const {
    return value[LAST];
}

template<UINT bits_num>
bool LongInt<bits_num>::get_bit(UINT pos) const {
    return (value[pos / BITS_BASE] >> ((BITS_BASE - (pos % BITS_BASE) - 1) % BITS_BASE)) & 1;
}


template<UINT bits_num>
void LongInt<bits_num>::set_bit(UINT pos, bool val) {
    UINT mask = 0;
    for (UINT i = 0; i < BITS_BASE; i++) {
        mask <<= (UINT) 1;
        if (i != pos % BITS_BASE)
            mask |= (UINT) 1;
    }
    value[pos / BITS_BASE] &= mask;
    value[pos / BITS_BASE] |= (val << pos % BITS_BASE);
}

template<UINT bits_num>
UINT LongInt<bits_num>::get_bits_count() const {
    return ARR_SIZE * BITS_BASE;
}


template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator+(const LongInt<bits_num> &other) const {
    LongInt res;
    FOR_IND_REVERSE(i) {
        res[i] = value[i] + other[i];
        if (i < ARR_SIZE - 1 && res[i + 1] < value[i + 1])
            res[i]++;
    }
    return res;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator+(UINT other) const {
    LongInt res;
    FOR_IND_REVERSE(i) {
        res[i] = value[i];
        if (i == ARR_SIZE - 1)
            res[i] += other;
        else if (res[i + 1] < value[i + 1])
            res[i]++;
    }
    return res;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator+=(const LongInt<bits_num> &other) {
    LongInt<bits_num> res = *this + other;
    FOR_IND(i)value[i] = res[i];
    return *this;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator+=(UINT other) {
    LongInt<bits_num> res = *this + other;
    FOR_IND(i)value[i] = res[i];
    return *this;
}


template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator-(const LongInt<bits_num> &other) const {
    LongInt res;
    FOR_IND_REVERSE(i) {
        res[i] = value[i] - other[i];
        if (i < ARR_SIZE - 1 && res[i + 1] > value[i + 1])
            res[i]--;
    }
    return res;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator-(UINT other) const {
    LongInt res;
    FOR_IND_REVERSE(i) {
        res[i] = value[i];
        if (i == ARR_SIZE - 1)
            res[i] -= other;
        else if (res[i + 1] > value[i + 1])
            res[i]--;
    }
    return res;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator-=(const LongInt<bits_num> &other) {
    LongInt<bits_num> res = *this - other;
    FOR_IND(i)value[i] = res[i];
    return *this;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator-=(UINT other) {
    LongInt<bits_num> res = *this - other;
    FOR_IND(i)value[i] = res[i];
    return *this;
}


template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator/(const LongInt<bits_num> &other) {
    LongInt ub((UINT) 0);
    ub--;
    LongInt lb((UINT) 0);
    if (*this < other)
        return LongInt<bits_num>((UINT) 0);

    // algorithm from wiki doesn't work correctly. Using binary search instead
    while (ub - lb > 1 && ub > lb) {
        LongInt mid = ((ub - lb) >> 1) + lb;
//        bool dbg = ARR_SIZE > 4 && ((*this)[4] == 73944566) && ((*this)[2] == 1261236803);
        if (mult_cmp(other, mid, *this)) {
            ub = mid;
        } else
            lb = mid;
    }
    return lb;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator/(UINT other) const {
    LongInt res(*this);
    res /= other;
    return res;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator/=(const LongInt<bits_num> &other) {
    if (other == 0)
        throw std::invalid_argument("Cannot divide by zero");

    LongInt<bits_num> res = *this / other;
    FOR_IND(i)value[i] = res[i];
    return *this;
}


template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator/=(UINT other) {
    uint64_t buf = 0;
    FOR_IND(i) {
        buf = (buf << BITS_BASE) + value[i];
        value[i] = buf / other;
        buf = buf % other;
    }
    // not the best solution but it's easy to write and it works!
    return *this;
}


template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator*(const LongInt<bits_num> &other) const {
    LongInt<bits_num> res;
    FOR_IND(i) {
        res += (other * value[i]) << (bits_num - (i + 1) * BITS_BASE);
    }
    return res;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator*(UINT other) const {
    LongInt res;
    uint64_t buf = 0;
    FOR_IND_REVERSE(i) {
        buf = value[i] * (uint64_t) other + buf;
        res[i] = buf;
        buf >>= BITS_BASE;
    }
    return res;
}


template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator*=(const LongInt<bits_num> &other) {
    LongInt<bits_num> res = *this * other;
    FOR_IND(i)value[i] = res[i];
    return *this;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator*=(UINT other) {
    uint64_t buf = 0;
    FOR_IND_REVERSE(i) {
        buf = value[i] * (uint64_t) other + buf;
        value[i] = buf;
        buf >>= BITS_BASE;
    }
    return *this;
}


template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator%(const LongInt<bits_num> &other) const {
    if (other == 0)
        throw std::invalid_argument("Cannot divide by zero");

    LongInt remainder;
    // algorithm from wiki
    for (UINT i = get_bits_count() - 1; i < get_bits_count(); i--) {
        remainder <<= 1;
        remainder |= get_bit(get_bits_count() - i - 1);
//        res.set_bit(i, remainder >= other);
        if (remainder >= other) {
            remainder -= other;
        }
    }
    return remainder;
//    LongInt a(*this);
//    LongInt d = a / other;
//    return *this - other * d;
}

template<UINT bits_num>
UINT LongInt<bits_num>::operator%(UINT other) const {
    uint64_t buf = 0;
    FOR_IND(i) {
        buf = ((buf << BITS_BASE) + value[i]) % other;
    }
    return (UINT) buf;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator%=(const LongInt<bits_num> &other) {
    if (other == 0)
        throw std::invalid_argument("Cannot divide by zero");
    LongInt<bits_num> res = *this % other;
    FOR_IND(i)value[i] = res[i];
    return *this;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator%=(UINT other) {
    uint64_t buf = 0;
    FOR_IND(i) {
        buf = ((buf << BITS_BASE) + value[i]) % other;
        value[i] = 0;
    }
    value[ARR_SIZE - 1] = buf;
    return this;
}

template<UINT bits_num>
bool LongInt<bits_num>::operator==(const LongInt<bits_num> &other) const {
    FOR_IND(i)
        if (value[i] != other[i])
            return false;
    return true;
}

template<UINT bits_num>
bool LongInt<bits_num>::operator==(UINT other) const {
    for (UINT i = 0; i < ARR_SIZE - 1; i++)
        if (value[i] != 0)
            return false;
    return value[ARR_SIZE - 1] == other;
}


template<UINT bits_num>
bool LongInt<bits_num>::operator!=(const LongInt<bits_num> &other) const {
    FOR_IND(i)
        if (value[i] != other[i])
            return true;
    return false;
}

template<UINT bits_num>
bool LongInt<bits_num>::operator!=(UINT other) const {
    for (UINT i = 0; i < ARR_SIZE - 1; i++)
        if (value[i] != 0)
            return true;
    return value[ARR_SIZE - 1] != other;
}


template<UINT bits_num>
bool LongInt<bits_num>::operator>(const LongInt<bits_num> &other) const {
    FOR_IND(i)
        if (value[i] > other[i])
            return true;
        else if (value[i] < other[i])
            return false;
    return false;
}

template<UINT bits_num>
bool LongInt<bits_num>::operator>(UINT other) const {
    for (UINT i = 0; i < ARR_SIZE - 1; i++)
        if (value[i])
            return true;
    return value[ARR_SIZE - 1] > other;
}


template<UINT bits_num>
bool LongInt<bits_num>::operator>=(const LongInt<bits_num> &other) const {
    FOR_IND(i)
        if (value[i] > other[i])
            return true;
        else if (value[i] < other[i])
            return false;
    return value[ARR_SIZE - 1] == other[ARR_SIZE - 1];
}

template<UINT bits_num>
bool LongInt<bits_num>::operator>=(UINT other) const {
    for (UINT i = 0; i < ARR_SIZE - 1; i++)
        if (value[i])
            return true;
    return value[ARR_SIZE - 1] >= other;
}


template<UINT bits_num>
bool LongInt<bits_num>::operator<(const LongInt<bits_num> &other) const {
    FOR_IND(i)
        if (value[i] < other[i])
            return true;
        else if (value[i] > other[i])
            return false;
    return false;
}

template<UINT bits_num>
bool LongInt<bits_num>::operator<(UINT other) const {
    for (UINT i = 0; i < ARR_SIZE - 1; i++)
        if (value[i])
            return false;
    return value[ARR_SIZE - 1] < other;
}


template<UINT bits_num>
bool LongInt<bits_num>::operator<=(const LongInt<bits_num> &other) const {
    FOR_IND(i)
        if (value[i] < other[i])
            return true;
        else if (value[i] > other[i])
            return false;
    return last_item() <= other.last_item();
}

template<UINT bits_num>
bool LongInt<bits_num>::operator<=(UINT other) const {
    for (UINT i = 0; i < ARR_SIZE - 1; i++)
        if (value[i])
            return false;
    return value[ARR_SIZE - 1] <= other;
}


template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator|(const LongInt<bits_num> &other) const {
    LongInt<bits_num> res;
    FOR_IND(i)res[i] = value[i] | other[i];
    return res;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator|(UINT other) const {
    LongInt<bits_num> res(*this);
    res[ARR_SIZE - 1] |= other;
    return res;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator|=(const LongInt<bits_num> &other) {
    FOR_IND(i) value[i] |= other[i];
    return *this;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator|=(UINT other) {
    value[LAST] |= other;
    return *this;
}


template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator&(const LongInt<bits_num> &other) const {
    LongInt<bits_num> res;
    FOR_IND(i)res[i] = value[i] & other[i];
    return res;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator&(UINT other) const {
    LongInt<bits_num> res;
    res[ARR_SIZE - 1] = value[ARR_SIZE - 1] & other;
    return res;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator&=(const LongInt<bits_num> &other) {
    FOR_IND(i)value[i] &= other[i];
    return *this;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator&=(UINT other) {
    FOR_IND(i)value[i] = 0;
    value[LAST] &= other;
    return *this;
}


template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator^(const LongInt<bits_num> &other) const {
    LongInt<bits_num> res;
    FOR_IND(i)res[i] = value[i] ^ other[i];
    return res;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator^(UINT other) const {
    LongInt<bits_num> res(*this);
    res[ARR_SIZE - 1] ^= other;
    return res;
}


template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator^=(const LongInt<bits_num> &other) {
    FOR_IND(i)value[i] ^= other[i];
    return *this;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator^=(UINT other) {
    value[LAST] ^= other;
    return *this;
}


template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator<<(UINT other) const {
    LongInt<bits_num> res(*this);
    res <<= other;
    return res;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator<<=(UINT other) {
    UINT shift_b = other / BITS_BASE, shift_s = other % BITS_BASE;
    FOR_IND(i) {
        UINT ni = i + shift_b;
        if (ni < ARR_SIZE) {
            value[i] = (value[ni] << shift_s) | ((ni == ARR_SIZE - 1 || shift_s == 0) ? (UINT) 0 : (UINT) (value[ni + 1]
                    >> ((BITS_BASE - shift_s) % BITS_BASE)));
        } else
            value[i] = 0;
    }
    return *this;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator>>(UINT other) const {
    LongInt<bits_num> res(*this);
    res >>= other;
    return res;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator>>=(UINT other) {
    UINT shift_b = other / BITS_BASE, shift_s = other % BITS_BASE;
    FOR_IND_REVERSE(i) {
        UINT ni = i - shift_b;
        if (ni <= i) {
            value[i] = (value[ni] >> shift_s) |
                       ((ni == 0 || shift_s == 0) ? 0 : (value[ni - 1] << ((BITS_BASE - shift_s) % BITS_BASE)));
        } else
            value[i] = 0;
    }
    return *this;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator~() const {
    LongInt<bits_num> res;
    FOR_IND(i)res[i] = ~value[i];
    return res;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator++() {
    *this += 1;
    return *this;
}

template<UINT bits_num>
const LongInt<bits_num> LongInt<bits_num>::operator++(int) {
    LongInt<bits_num> res(*this);
    *this += 1;
    return res;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator--() {
    *this -= 1;
    return *this;
}

template<UINT bits_num>
const LongInt<bits_num> LongInt<bits_num>::operator--(int) {
    LongInt<bits_num> res(*this);
    *this -= 1;
    return res;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator=(UINT other) {
    FOR_IND(i)value[i] = 0;
    value[LAST] = other;
    return *this;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator=(int other) {
    FOR_IND(i)value[i] = 0;
    for (UINT ps = sizeof(other) / BITS_BASE; ps != 0; ps--) {
        value[ps] = ((unsigned) other >> (BITS_BASE * ps)) & ((UINT) -1);
    }
    value[LAST] = (unsigned) other & ((UINT) -1);
    return *this;
}

template<UINT bits_num>
UINT &LongInt<bits_num>::operator[](UINT index) {
    return this->value[index];
}

template<UINT bits_num>
UINT LongInt<bits_num>::operator[](UINT index) const {
    return this->value[index];
}
