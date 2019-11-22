//
// Created by ky3he4ik on 22/11/19.
//

#ifndef ECC_POINT_H
#define ECC_POINT_H


#include "EllipticCurve.h"
#include "LongInt.h"

class Point {
    Point(EllipticCurve *curve, LongInt x, LongInt y);

};


#endif //ECC_POINT_H
