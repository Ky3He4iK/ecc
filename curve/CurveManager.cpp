//
// Created by ky3he4ik on 14/12/2019.
//

#include "CurveManager.h"

CurveManager::CurveManager() {
    nlohmann::json curves_json = R"({
  "secp256k1": {
    "curve": {
      "a": "0",
      "b": "7",
      "p": "FFFFFFFF_FFFFFFFF_FFFFFFFF_FFFFFFFF_FFFFFFFF_FFFFFFFF_FFFFFFFE_FFFFFC2F"
    },
    "base_point": {
      "x": "79BE667E F9DCBBAC 55A06295 CE870B07 029BFCDB 2DCE28D9 59F2815B 16F81798",
      "y": "483ADA77 26A3C465 5DA4FBFC 0E1108A8 FD17B448 A6855419 9C47D08F FB10D4B8"
    },
    "curve_order": "FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE BAAEDCE6 AF48A03B BFD25E8C D0364141"
  }
})"_json;
    for (auto &it: curves_json.items()) {
        curve_names.emplace_back(it.key());
        auto param = Curve_parameters::deserialize(it.value());
        loaded_curves[it.key()] = {param, param.bitLen()};
    }
}

CurveManager &CurveManager::getInstance() {
    static CurveManager curveManager;
    return curveManager;
}

std::pair<Curve_parameters, int> CurveManager::getCurves(const std::string &str) const {
    auto res = loaded_curves.find(str);
    if (res != loaded_curves.end())
        return res->second;
    return {{}, 0};
}

std::vector<std::string> CurveManager::getCurveNames() const {
    return curve_names;
}
