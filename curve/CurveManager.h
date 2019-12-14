//
// Created by ky3he4ik on 14/12/2019.
//

#ifndef ECC_CURVEMANAGER_H
#define ECC_CURVEMANAGER_H

#include "Curve_parameters.h"
#include "../dependences/json.hpp"

#include <vector>
#include <map>
#include <string>

class CurveManager {
private:
    std::map<std::string, std::pair<Curve_parameters, int> > loaded_curves;
    std::vector<std::string> curve_names;

    CurveManager();

public:
    static CurveManager &getInstance();

    [[nodiscard]] std::pair<Curve_parameters, int> getCurves(const std::string &str) const;

    [[nodiscard]] std::vector<std::string> getCurveNames() const;

};


#endif //ECC_CURVEMANAGER_H
