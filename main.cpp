#include <iostream>

#include "curve/LongInt.h"
#include "curve/EllipticCurve.h"
#include "curve/Point.h"
#include "curve/ECC.h"

using namespace std;

#define ASSERT_BOOL(cond) {if (!(cond)) { \
    r = false; \
    cout << "Err\n"; \
} else cout << "OK\n"; }
#define ASSERT_TEST(left, right, description) { \
    r &= my_assert(left, right, description); \
}

bool my_assert(const LongInt &left, const LongInt &right, const std::string &description) {
    if (right != left) {
        cout << description << "\nExpected: " << right.to_string(16) << "\t(" << right.to_string()
             << ")\n\t Got: " << left.to_string(16) << "\t(" << left.to_string() << ")\n\t\t  "
             << right.to_string() << " != " << left.to_string() << '\n';
        return false;
    }
    cout << description << ": OK\n";
    return true;
}

bool my_assert(const Point &left, const Point &right, const std::string &description) {
    if (!(right == left)) {
        cout << description << "\nExpected: " << right.to_string() << ";\tGot: " << left.to_string() << '\n';
        return false;
    }
    cout << description << ": OK\n";
    return true;
}

bool testCurvesNPoints() {
    bool r = true;

    cout << "Curve and points:\n";

    auto curve_pointer = std::make_shared<EllipticCurve>(EllipticCurve(LongInt(), LongInt(7), LongInt(211)));

    Point point(curve_pointer, LongInt(150), LongInt(22));
    cout << "Smaller curve: " << curve_pointer->to_string() << "; base point: " << point.to_string() << '\n';
    LongInt n = curve_pointer->set_curve_order(point);
    cout << "n: " << n.to_string() << '\n';

    auto pair = curve_pointer->generate_keypair(point);
    LongInt d = pair.first;
    Point Q = pair.second;
    cout << "Private: " << d.to_string() << "\nPublic: " << Q.to_string() << '\n';

    //todo: attacks?

    pair = curve_pointer->generate_keypair(point);
    LongInt d1 = pair.first;
    Point Q1 = pair.second;
    cout << "Second private: " << d1.to_string() << "\nSecond public: " << Q1.to_string() << '\n';
    ASSERT_TEST(Q + (-Q1) + Q1, Q, "-+")
    Point c(curve_pointer, LongInt(51), LongInt(19));
    ASSERT_TEST(c / 94 * 94, c, "/*")
    c = Point(curve_pointer, LongInt(22), LongInt(152));
    ASSERT_TEST(c / 104 * 104, c, "/*")
    c = Point(curve_pointer, LongInt(56), LongInt(156));
    ASSERT_TEST(c / 185 * 185, c, "/*")
    ASSERT_TEST(Q / d1 * d1, Q, "/*")


    ASSERT_TEST(curve_pointer->get_y(Q1.get_x(), (Q1.get_y() & 1) == 1), Q1.get_y(), "get y by x")
    ASSERT_TEST(curve_pointer->get_point_by_x(Q1.get_x(), (Q1.get_y() & 1) == 1), Q1, "get point by x")
    ASSERT_BOOL(curve_pointer->contains(curve_pointer->get_point_by_x(Q1.get_x(), 0)))
    ASSERT_BOOL(curve_pointer->contains(curve_pointer->get_point_by_x(Q1.get_x(), 1)))

    if (r)
        cout << "Point&curve tests are passed!\n";
    else
        cout << "There are error in some tests\n";
    return r;
}

char itoc(int i) {
    if (i < 0)
        i += 16;
    return (i < 10) ? i + '0' : i + 'A' - 10;
}

std::string bin_str_to_hex(const std::string &str) {
    std::string res;
    for (auto &c: str) {
        res.push_back(itoc((c >> 4) & 0xf));
        res.push_back(itoc(c & 0xf));
    }
    return res;
}

int main() {
    bool k = testCurvesNPoints();
    if (k)
        cout << "All tests have passed!\n";
    else
        cout << "ERROR!\n";

    ECC Bob = ECC(Curve_parameters::curve_secp256k1());
    ECC Alice = ECC(Bob.get_parameters());
    std::string msg = "Some encrypted message sent from Bob to Alice";
    cout << "msg: " << msg << '\n';

    auto sharedAlice = Alice.set_shared_secret(Bob.get_public_key());
    auto sharedBob = Bob.set_shared_secret(Alice.get_public_key());
    bool r = true;
    ASSERT_TEST(sharedAlice, sharedBob, "Shared key match")
    cout << "Shared: " << sharedAlice.to_string(16) << '\n';
    auto encrypted = Bob.encode(msg);
    cout << "Encrypted by Bob msg (in hex): " << bin_str_to_hex(encrypted) << '\n';
    auto sign = Bob.sign_msg(msg);
    cout << "Sign: (" << sign.first.to_string() << ";" << sign.second.to_string() << ")\n";

    auto decrypted = Alice.decode(encrypted);
    cout << "Decrypted by Alice: " << decrypted << '\n';
    cout << "Verify: "
         << (ECC::verify_msg(decrypted, sign, Bob.get_public_key(), Bob.get_parameters()) ? "Passed" : "Failed")
         << '\n';
    cout << "Bob data: " << Bob.serialize() << '\n';
    cout << "Alice data: " << Alice.serialize() << '\n';
    return 0;
}
