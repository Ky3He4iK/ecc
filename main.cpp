#include <iostream>

#include "LongInt.h"
#include "EllipticCurve.h"
#include "Point.h"

using namespace std;

#define ASSERT_BOOL(cond) {if (!(cond)) return false; cout << "OK\n"; }
#define ASSERT_TEST(left, right, description) { \
    r &= assert(left, right, description); \
}

bool assert(const LongInt &left, const LongInt &right, const std::string &description) {
    if (right != left) {
        cout << description << "\nExpected: " << right.to_string(16, true) << "\t(" << right.to_string()
             << ")\n\t Got: " << left.to_string(16, true) << "\t(" << left.to_string() << ")\n\t\t  "
             << right.to_string() << " != " << left.to_string() << '\n';
        return false;
    }
    cout << description << ": OK\n";
    return true;
}

bool assert(const Point &left, const Point &right, const std::string &description) {
    if (!(right == left)) {
        cout << description << "\nExpected: " << right.to_string() << ";\tGot: " << left.to_string() << '\n';
        return false;
    }
    cout << description << ": OK\n";
    return true;
}

bool testLongInt() {
    bool r = true;
    UINT arra[] = {0, 1, 2};
    UINT arrb[] = {0, 2, 4};
    UINT arrc[] = {0, 3, 6};
    LongInt a(96, arra);
    LongInt b(96, arrb);
    LongInt c(96, arrc);
    ASSERT_TEST(a * 2, b, "*2")
    ASSERT_TEST(a + a, b, "+")
    ASSERT_TEST(c - b, a, "-")
    ASSERT_TEST((a << 1), b, "<<")
    cout << "Checkpoint 1\n";
    ASSERT_TEST((a ^ b), c, "^")
    ASSERT_TEST((a | b), c, "|")
    ASSERT_TEST((a & b), LongInt(96, (UINT)
            0), "&")
    ASSERT_BOOL(a < c)
    cout << "Checkpoint 2\n";
    ASSERT_BOOL(c > b)
    ASSERT_BOOL(a != b)
    ASSERT_TEST(LongInt(96, "0x0000 0001 0000 0002", 16), a, "from str")
    ASSERT_TEST(LongInt(96, "42_9496_7298_bjhgfccv", 10), a, "from str")
    cout << "Checkpoint 3\n";
    ASSERT_BOOL(a.to_string(10) == "4294967298")
    ASSERT_TEST(1 * a, a, "*")
    ASSERT_TEST(LongInt(32, "1100", 2) / LongInt(32, "100", 2), LongInt(32, "3"), "/")
    ASSERT_TEST(LongInt(32, 16) / LongInt(32, 2), LongInt(32, 8), "/")

    cout << "Random tests:\n";
    LongInt r1(192, "13860852903647110680");
    LongInt r2(192, "7163078174578821018");

    ASSERT_BOOL(!r2.get_bit(14))
    ASSERT_BOOL(!r2.get_bit(128))
    ASSERT_BOOL(r2.get_bit(129))
    ASSERT_BOOL(r2.get_bit(130))
    ASSERT_BOOL(!r2.get_bit(131))

    cout << "Arithmetic\n";
    ASSERT_TEST(r1 + 4294967295, LongInt(192, "13860852907942077975"), "+")
    ASSERT_TEST(r1 + r2, LongInt(192, "21023931078225931698"), "+")
    ASSERT_TEST(1 + r1, LongInt(192, "13860852903647110681"), "+")
    ASSERT_TEST(r1 - r2, LongInt(192, "6697774729068289662"), "-")
    ASSERT_TEST(r1 / 45, LongInt(192, "308018953414380237"), "/")
    ASSERT_TEST(r1 * r2, LongInt(192, "99286372915162096498107574836556272240"), "*")
    ASSERT_TEST(r1 * 47, LongInt(192, "651460086471414201960"), "*")
    LongInt s1 = r1.changeLen(66);
    LongInt s2 = r2.changeLen(66);
    ASSERT_TEST(s1 % s2, LongInt(66, "6697774729068289662"), "%")
    ASSERT_TEST(s1 / s2, LongInt(66, "1"), "/")
    ASSERT_TEST(s1 * 4 / s2, LongInt(66, "7"), "*/")
    ASSERT_TEST(LongInt(192, r1 % 45), LongInt(30, "15").changeLen(192), "%")
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
    ASSERT_TEST(r1 | r2, LongInt(192, "16391974441348753306"), "|")
    ASSERT_TEST(r1 | 42, LongInt(192, "13860852903647110714"), "|")
    ASSERT_TEST(r1 & r2, LongInt(192, "4631956636877178392"), "&")
    ASSERT_TEST(r1 & 42, LongInt(192, "8"), "&")
    ASSERT_TEST(r1 ^ r2, LongInt(192, "11760017804471574914"), "^")
    ASSERT_TEST(r1 ^ 42, LongInt(192, "13860852903647110706"), "^")
    ASSERT_TEST(r1 >> 8, LongInt(192, "54143956654871526"), ">>")
    ASSERT_TEST(r1 << 37, LongInt(192, "1905021117306591343855754280960"), "<<")

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

    r1 = LongInt(192, "13860852903647110680");
    r2 = LongInt(192, "7163078174578821018");

    cout << "Extra-long tests:\n";
    LongInt l1(256, "257333454774842625920237308813574237994");
    LongInt l2(256, "337997643905302403911919781617103751364");

    ASSERT_TEST(LongInt(256, "26959946667150639794667015087019630673637144422540572481103610249215") + 1,
                LongInt(256, "26959946667150639794667015087019630673637144422540572481103610249216"), "+1")
    ASSERT_TEST(LongInt(256, "115792089237316195423570985008687907853269984665640564039457584007913129639935") + 1,
                LongInt(256, "0"), "+1")
    ASSERT_TEST(LongInt(256, "26959946667150639794667015087019630673637144422540572481103610249216") - 1,
                LongInt(256, "26959946667150639794667015087019630673637144422540572481103610249215"), "-1")
    ASSERT_TEST(LongInt(256, "115792089237316195423570985008687907853269984665640564039457584007913129639936"),
                LongInt(256, "0"), "of")
    ASSERT_TEST(
            LongInt(256, "115792089237316195423570985008687907853269984665640564039457584007913129639935") >> BITS_BASE,
            LongInt(256, "26959946667150639794667015087019630673637144422540572481103610249215"), "+1")
    ASSERT_TEST(l1 + l2, LongInt(256, "595331098680145029832157090430677989358"), "+")
    ASSERT_TEST(1 + l1, LongInt(256, "257333454774842625920237308813574237995"), "1+")
    ASSERT_TEST(l2 - l1, LongInt(256, "80664189130459777991682472803529513370"), "-")
    ASSERT_TEST(l1 * l2, LongInt(256, "86978101411908498471595237453121472878336237366277074434698761705205138123816"),
                "*")
    ASSERT_TEST(l1 * 47, LongInt(256, "12094672374417603418251153514237989185718"), "*")
    ASSERT_TEST(l1 % l1, LongInt(256, "0"), "%")
    ASSERT_TEST(l1 % l2, LongInt(256, "257333454774842625920237308813574237994"), "%")
    ASSERT_TEST(l2 % l1, LongInt(256, "80664189130459777991682472803529513370"), "%")
    ASSERT_TEST(LongInt(256, l1 % 45), LongInt(32, "24").changeLen(256), "%")
    ASSERT_TEST(l1 / 45, LongInt(256, "5718521217218725020449717973634983066"), "/")
    ASSERT_TEST(l1 / l2, LongInt(256, "0"), "/")
    ASSERT_TEST(l2 / l1, LongInt(256, "1"), "/")
    ASSERT_TEST(l1 * 4 / l2, LongInt(256, "3"), "* /")
    ASSERT_TEST(l1 / LongInt(256, "26959946667150639794667015087019630673637144422540572481103610249215"),
                LongInt(256, "0"), "/")
    ASSERT_TEST(
            LongInt(256, "26959946667150639794667015087019630673637144422540572481103610249215") / LongInt(256, "1"),
            LongInt(256, "26959946667150639794667015087019630673637144422540572481103610249215"), "/")
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
    ASSERT_TEST(l1 | l2, LongInt(256, "340116192306808449001680668437468901358"), "|")
    ASSERT_TEST(l1 | 42, LongInt(256, "257333454774842625920237308813574237994"), "|")
    ASSERT_TEST(l1 & l2, LongInt(256, "255214906373336580830476421993209088000"), "&")
    ASSERT_TEST(l1 & 42, LongInt(256, "42"), "&")
    ASSERT_TEST(l1 ^ l2, LongInt(256, "84901285933471868171204246444259813358"), "^")
    ASSERT_TEST(l1 ^ 42, LongInt(256, "257333454774842625920237308813574237952"), "^")
    ASSERT_TEST(l1 >> 8, LongInt(256, "1005208807714229007500926987553024367"), ">>")
    ASSERT_TEST(l1 << 37, LongInt(256, "35367640717588611899973796669227325217675220615168"), "<<")

    cout << "Combine len:\n";
    ASSERT_TEST(l1 / r1.changeLen(256), LongInt(256, "18565484863282276648"), "/")
    cout << l1.to_string() << ' ' << r1.to_string() << ' ' << r2.to_string() << '\n';
    cout << l1.to_string(16, true) << ' ' << r1.to_string(16, true) << ' ' << r2.to_string(16, true) << '\n';
    ASSERT_TEST((l1 * r2.changeLen(256)) / r1.changeLen(256),
                LongInt(256, "132986019424650742707037798676240676234"), "* /")

    LongInt ll1(512,
                "6238162682458071501550601419107179902265820124508191469615123332044395304616134585420987485345987351121513745903552771477682002784196138250718972903116667");
    LongInt ll2(512,
                "601878907032733300537224155868479379715586974160464875191130221445025214725227030143567584586666338417751100136667304419942937584554666204915760222789298");
    ASSERT_TEST(ll1.changeLen(2048) * ll2.changeLen(2048), LongInt(2048,
                                                                   "3754618537210247802566121260460371099236390818479251663063529991850135472888709563703389675152639014575356816975895608352120156452719547448590356478393981151277454088836920431897360207456515841528723783171289952071265444118872554041416583699137970002861243351947894865714785111644651268408155338304253029766"),
                "*")
    ASSERT_TEST(ll1 / ll2, LongInt(512, "10"), "/")
    ASSERT_TEST(ll1 % ll2, LongInt(512,
                                   "219373612130738496178359860422386105109950382903542717703821117594143157363864283985311639479323966944002744536879727278252626938649476201561370675223687"),
                "%")
    ASSERT_TEST(ll1 - ll2, LongInt(512,
                                   "5636283775425338201013377263238700522550233150347726594423993110599370089890907555277419900759321012703762645766885467057739065199641472045803212680327369"),
                "-")
    ASSERT_TEST(ll1 >> 0, ll1, ">> 0")
    ASSERT_TEST(ll1 << 0, ll1, "<< 0")

    ASSERT_TEST(LongInt(32, "1024") / LongInt(64, "2"), LongInt(32, "512"), "combi-121")
    ASSERT_TEST(LongInt(64, "1024") / LongInt(32, "2"), LongInt(32, "512"), "combi-211")
    ASSERT_TEST(LongInt(32, "1024") / LongInt(32, "2"), LongInt(64, "512"), "combi-112")
    ASSERT_BOOL(LongInt(32, "1024") / LongInt(64, "2") != LongInt(32, "513"))
    ASSERT_BOOL(LongInt(32, "1024") / LongInt(32, "2") != LongInt(64, "513"))

    return r;
}

bool testCurvesNPoints() {
    bool r = true;

    cout << "Curve and points:\n";
    EllipticCurve curve(LongInt(LONG_INT_LEN), LongInt(LONG_INT_LEN), LongInt(LONG_INT_LEN, 7),
                        LongInt(LONG_INT_LEN, 211), LongInt(LONG_INT_LEN, 1));
    Point point(&curve, LongInt(LONG_INT_LEN, 150), LongInt(LONG_INT_LEN, 22));
    cout << "Smaller curve: " << curve.to_string() << "; generator point: " << point.to_string() << '\n';
    curve.set_curve_order(point);
    UINT n = curve.get_order();
    cout << "n: " << n << '\n';

    auto pair = curve.generate_keypair(point);
    UINT d = pair.first;
    Point Q = pair.second;
    cout << "Private: " << d << "\nPublic: " << Q.to_string() << '\n';

    //todo: attacks?

    pair = curve.generate_keypair(point);
    UINT d1 = pair.first;
    Point Q1 = pair.second;
    cout << "Second private: " << d1 << "\nSecond public: " << Q1.to_string() << '\n';
    Point c(&curve, LongInt(LONG_INT_LEN, 51), LongInt(LONG_INT_LEN, 19));
    ASSERT_TEST(Q + (-Q1) + Q1, Q, "-+")
    ASSERT_TEST(c / 94 * 94, c, "/*")
    c = Point(&curve, LongInt(LONG_INT_LEN, 22), LongInt(LONG_INT_LEN, 152));
    ASSERT_TEST(c / 104 * 104, c, "/*")
    ASSERT_TEST(Q / d1 * d1, Q, "/*")


    ASSERT_TEST(curve.get_y(Q1.get_x(), Q1.get_y().last_item() & 1), Q1.get_y(), "get y by x")
    ASSERT_TEST(curve.get_point_by_x(Q1.get_x(), Q1.get_y().last_item() & 1), Q1, "get point by x")
    ASSERT_BOOL(curve.contains(curve.get_point_by_x(Q1.get_x(), 0)))
    ASSERT_BOOL(curve.contains(curve.get_point_by_x(Q1.get_x(), 1)))

    if (r)
        cout << "Point&curve tests are passed!\n";
    else
        cout << "There are error in some tests\n";
    return r;
}

int main() {
    UINT arr[2] = {1, 2};
    UINT arrl[4] = {0, 0, 1, 2};
    LongInt a(64, 123);
    cout << a.to_string(10) << '\t' << a.to_string(16) << '\n';
    LongInt b(64, arr);
    cout << b.to_string(10) << '\t' << b.to_string(16) << '\n';
    cout << (b * 2).to_string(10) << '\n';

    bool k = testCurvesNPoints();
    if (!testLongInt())
        k = false;
    if (k)
        cout << "All tests have passed!\n";
    else
        cout << "ERROR!\n";

    LongInt bl(128, arrl);
    LongInt c = bl * 2;
    LongInt d = bl * c;
    cout << '\n' << d.to_string(16) << '\n';
    cout << d.to_string(10) << '\n';
    cout << "36893488181778841608\n";
    return 0;

}
