#include <iostream>

#include "LongInt.h"
#include "LongInt.cpp"

using namespace std;

#define ASSERT_BOOL(cond) {if (!(cond)) return false; cout << "OK\n"; }
#define ASSERT(left, right, description) { \
    r &= assert(left, right, description); \
}

template<UINT n>
bool assert(const LongInt<n> &left, const LongInt<n> &right, const std::string &description) {
    if (right != left) {
        cout << description << "\nExpected: " << right.to_string(16, true) << "\t(" << right.to_string()
             << ")\n\t Got: " << left.to_string(16, true) << "\t(" << left.to_string() << ")\n\t\t  "
             << right.to_string() << " != " << left.to_string() << '\n';
        return false;
    }
    cout << description << ": OK\n";
    return true;
}

bool test() {
    bool r = true;
    UINT arra[] = {0, 1, 2};
    UINT arrb[] = {0, 2, 4};
    UINT arrc[] = {0, 3, 6};
    LongInt<96> a(arra);
    LongInt<96> b(arrb);
    LongInt<96> c(arrc);
    ASSERT(a * 2, b, "*2")
    ASSERT(a + a, b, "+")
    ASSERT(c - b, a, "-")
    ASSERT((a << 1), b, "<<")
    cout << "Checkpoint 1\n";
    ASSERT((a ^ b), c, "^")
    ASSERT((a | b), c, "|")
    ASSERT((a & b), LongInt<96>((UINT)
            0), "&")
    ASSERT_BOOL(a < c)
    cout << "Checkpoint 2\n";
    ASSERT_BOOL(c > b)
    ASSERT_BOOL(a != b)
    ASSERT(LongInt<96>("0x0000 0001 0000 0002", 16), a, "from str")
    ASSERT(LongInt<96>("42_9496_7298_bjhgfccv", 10), a, "from str")
    cout << "Checkpoint 3\n";
    ASSERT_BOOL(a.to_string(10) == "4294967298")
    ASSERT(1 * a, a, "*")
    ASSERT(LongInt<32>("1100", 2) / LongInt<32>("100", 2), LongInt<32>("3"), "/")
    ASSERT(LongInt<32>(16) / LongInt<32>(2), LongInt<32>(8), "/")

    cout << "Random tests:\n";
    LongInt<192> r1("13860852903647110680");
    LongInt<192> r2("7163078174578821018");

    ASSERT_BOOL(!r2.get_bit(14))
    ASSERT_BOOL(!r2.get_bit(128))
    ASSERT_BOOL(r2.get_bit(129))
    ASSERT_BOOL(r2.get_bit(130))
    ASSERT_BOOL(!r2.get_bit(131))

    cout << "Arithmetic\n";
    ASSERT(r1 + 4294967295, LongInt<192>("13860852907942077975"), "+")
    ASSERT(r1 + r2, LongInt<192>("21023931078225931698"), "+")
    ASSERT(1 + r1, LongInt<192>("13860852903647110681"), "+")
    ASSERT(r1 - r2, LongInt<192>("6697774729068289662"), "-")
    ASSERT(r1 / 45, LongInt<192>("308018953414380237"), "/")
    ASSERT(r1 * r2, LongInt<192>("99286372915162096498107574836556272240"), "*")
    ASSERT(r1 * 47, LongInt<192>("651460086471414201960"), "*")
    LongInt<66> s1 = LongInt<66>::changeLen(r1);
    LongInt<66> s2 = LongInt<66>::changeLen(r2);
    ASSERT(s1 % s2, LongInt<66>("6697774729068289662"), "%")
    ASSERT(s1 / s2, LongInt<66>("1"), "/")
    ASSERT(s1 * 4 / s2, LongInt<66>("7"), "*/")
    ASSERT(LongInt<192>(r1 % 45), LongInt<192>::changeLen(LongInt<30>("15")), "%")
    ASSERT_BOOL(r1 % 45 == 15)

    cout << "logic\n";
    ASSERT_BOOL(r1 == r1)
    ASSERT_BOOL(!(r1 == 42))
    ASSERT_BOOL(r1 != r2)
    ASSERT_BOOL(r1 != 42)
    ASSERT_BOOL(r1 > r2)
    ASSERT_BOOL(r1 > 42)
    ASSERT_BOOL(r1 >= r2)
    ASSERT_BOOL(r1 >= 42)
    ASSERT_BOOL(!(r1 < 42))
    ASSERT_BOOL(!(r1 <= 42))
    cout << "bin\n";
    ASSERT(r1 | r2, LongInt<192>("16391974441348753306"), "|")
    ASSERT(r1 | 42, LongInt<192>("13860852903647110714"), "|")
    ASSERT(r1 & r2, LongInt<192>("4631956636877178392"), "|")
    ASSERT(r1 & 42, LongInt<192>("8"), "&")
    ASSERT(r1 ^ r2, LongInt<192>("11760017804471574914"), "^")
    ASSERT(r1 ^ 42, LongInt<192>("13860852903647110706"), "^")
    ASSERT(r1 >> 8, LongInt<192>("54143956654871526"), ">>")
    ASSERT(r1 << 37, LongInt<192>("1905021117306591343855754280960"), "<<")

    //check for compile-time errors
    r1++;
    ++r1;
    r1 += 1;
    r1 += r2;
    r1--;
    --r1;
    r1 -= 1;
    r1 -= r2;
    r1 /= 45;
    r1 /= r2;
    r1 *= 1;
    r1 *= r2;
    r1 |= 1;
    r1 |= r2;
    r1 &= 1;
    r1 &= r2;
    r1 ^= 1;
    r1 ^= r2;
    r1 <<= 1;
    r1 >>= 1;
    ~r1;

    r1 = LongInt<192>("13860852903647110680");
    r2 = LongInt<192>("7163078174578821018");

    cout << "Extra-long tests:\n";
    LongInt<256> l1("257333454774842625920237308813574237994");
    LongInt<256> l2("337997643905302403911919781617103751364");

    ASSERT(LongInt<256>("26959946667150639794667015087019630673637144422540572481103610249215") + 1,
           LongInt<256>("26959946667150639794667015087019630673637144422540572481103610249216"), "+1")
    ASSERT(LongInt<256>("115792089237316195423570985008687907853269984665640564039457584007913129639935") + 1,
           LongInt<256>("0"), "+1")
    ASSERT(LongInt<256>("26959946667150639794667015087019630673637144422540572481103610249216") - 1,
           LongInt<256>("26959946667150639794667015087019630673637144422540572481103610249215"), "-1")
    ASSERT(LongInt<256>("115792089237316195423570985008687907853269984665640564039457584007913129639936"),
           LongInt<256>("0"), "of")
    ASSERT(LongInt<256>("115792089237316195423570985008687907853269984665640564039457584007913129639935") >> BITS_BASE,
           LongInt<256>("26959946667150639794667015087019630673637144422540572481103610249215"), "+1")
    ASSERT(l1 + l2, LongInt<256>("595331098680145029832157090430677989358"), "+")
    ASSERT(1 + l1, LongInt<256>("257333454774842625920237308813574237995"), "1+")
    ASSERT(l2 - l1, LongInt<256>("80664189130459777991682472803529513370"), "-")
    ASSERT(l1 * l2, LongInt<256>("86978101411908498471595237453121472878336237366277074434698761705205138123816"), "*")
    ASSERT(l1 * 47, LongInt<256>("12094672374417603418251153514237989185718"), "*")
    ASSERT(l1 % l1, LongInt<256>("0"), "%")
    ASSERT(l1 % l2, LongInt<256>("257333454774842625920237308813574237994"), "%")
    ASSERT(l2 % l1, LongInt<256>("80664189130459777991682472803529513370"), "%")
    ASSERT(LongInt<256>(l1 % 45), LongInt<256>::changeLen(LongInt<30>("24")), "%")
    ASSERT(l1 / 45, LongInt<256>("5718521217218725020449717973634983066"), "/")
    ASSERT(l1 / l2, LongInt<256>("0"), "/")
    ASSERT(l2 / l1, LongInt<256>("1"), "/")
    ASSERT(l1 * 4 / l2, LongInt<256>("3"), "*/")
    ASSERT(l1 / LongInt<256>("26959946667150639794667015087019630673637144422540572481103610249215"),
           LongInt<256>("0"), "/")
    ASSERT(LongInt<256>("26959946667150639794667015087019630673637144422540572481103610249215") / LongInt<256>("1"),
           LongInt<256>("26959946667150639794667015087019630673637144422540572481103610249215"), "/")
    ASSERT_BOOL(l1 % 45 == 24)


    cout << "logic\n";
    ASSERT_BOOL(l1 == l1)
    ASSERT_BOOL(!(l1 == 42))
    ASSERT_BOOL(l1 != l2)
    ASSERT_BOOL(l1 != 42)
    ASSERT_BOOL(l1 < l2)
    ASSERT_BOOL(l1 > 42)
    ASSERT_BOOL(l1 <= l2)
    ASSERT_BOOL(l1 >= 42)
    ASSERT_BOOL(!(l1 < 42))
    ASSERT_BOOL(!(l1 <= 42))
    cout << "bin\n";
    ASSERT(l1 | l2, LongInt<256>("340116192306808449001680668437468901358"), "|")
    ASSERT(l1 | 42, LongInt<256>("257333454774842625920237308813574237994"), "|")
    ASSERT(l1 & l2, LongInt<256>("255214906373336580830476421993209088000"), "&")
    ASSERT(l1 & 42, LongInt<256>("42"), "&")
    ASSERT(l1 ^ l2, LongInt<256>("84901285933471868171204246444259813358"), "^")
    ASSERT(l1 ^ 42, LongInt<256>("257333454774842625920237308813574237952"), "^")
    ASSERT(l1 >> 8, LongInt<256>("1005208807714229007500926987553024367"), ">>")
    ASSERT(l1 << 37, LongInt<256>("35367640717588611899973796669227325217675220615168"), "<<")

    cout << "Combine len:\n";
    ASSERT(l1 / LongInt<256>::changeLen(r1), LongInt<256>("18565484863282276648"), "/")
    cout << l1.to_string() << ' ' << r1.to_string() << ' ' << r2.to_string() << '\n';
    cout << l1.to_string(16, true) << ' ' << r1.to_string(16, true) << ' ' << r2.to_string(16, true) << '\n';
    ASSERT((l1 * LongInt<256>::changeLen(r2)) / LongInt<256>::changeLen(r1),
           LongInt<256>("132986019424650742707037798676240676234"), "*/")
    LongInt<18>::changeLen(LongInt<128>("1"));
    return r;
}

int main() {
    UINT arr[2] = {1, 2};
    UINT arrl[4] = {0, 0, 1, 2};
    LongInt<64> a(123);
    cout << a.to_string(10) << '\t' << a.to_string(16) << '\n';
    LongInt<64> b(arr);
    cout << b.to_string(10) << '\t' << b.to_string(16) << '\n';
    cout << (b * 2).to_string(10) << '\n';

    if (test())
        cout << "All tests have passed!\n";
    else
        cout << "ERROR!\n";

    LongInt<128> bl(arrl);
    LongInt<128> c = bl * 2;
    LongInt<128> d = bl * c;
    cout << '\n' << d.to_string(16) << '\n';
    cout << d.to_string(10) << '\n';
    cout << "36893488181778841608\n";
    return 0;

}
