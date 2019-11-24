//
// Created by ky3he4ik on 15/11/19.
//

#ifndef ECC_ELLIPTICCURVE_H
#define ECC_ELLIPTICCURVE_H

#include "LongInt.h"

typedef LongInt AS_INT;


// y**2 = (x**3 + a*x + b) % p
class EllipticCurve {
private:
    AS_INT p;
    AS_INT a;
    AS_INT b;

public:
    [[nodiscard]] const AS_INT *get_p() const {
        return &p;
    }

    [[nodiscard]] const AS_INT *get_a() const {
        return &a;
    }

    [[nodiscard]] const AS_INT *get_b() const {
        return &b;
    }
};


#endif //ECC_ELLIPTICCURVE_H
