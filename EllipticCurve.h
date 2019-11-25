//
// Created by ky3he4ik on 15/11/19.
//

#ifndef ECC_ELLIPTICCURVE_H
#define ECC_ELLIPTICCURVE_H

#include "LongInt.h"
#include "Point.h"

class Point;

// y**2 = (x**3 + a*x + b) % p
class EllipticCurve {
private:
    LongInt p;
    LongInt a;
    LongInt b;
    LongInt n;

public:
    [[nodiscard]] const LongInt *get_p() const {
        return &p;
    }

    [[nodiscard]] const LongInt *get_a() const {
        return &a;
    }

    [[nodiscard]] const LongInt *get_b() const {
        return &b;
    }
    [[nodiscard]] const LongInt *get_n() const {
        return &n;
    }
};


#endif //ECC_ELLIPTICCURVE_H
