#pragma once
#include <string>
#include <stdexcept>

enum class Brand {
    CHANEL,
    DIOR,
    GUCCI,
    VERSACE,
    PRADA,
    LOUIS_VUITTON,
    HERMES,
    BURBERRY,
    TOM_FORD,
    YSL
};

inline std::string brandToString(Brand b) {
    switch (b) {
    case Brand::CHANEL:       return "CHANEL";
    case Brand::DIOR:         return "DIOR";
    case Brand::GUCCI:        return "GUCCI";
    case Brand::VERSACE:      return "VERSACE";
    case Brand::PRADA:        return "PRADA";
    case Brand::LOUIS_VUITTON:return "LOUIS_VUITTON";
    case Brand::HERMES:       return "HERMES";
    case Brand::BURBERRY:     return "BURBERRY";
    case Brand::TOM_FORD:     return "TOM_FORD";
    case Brand::YSL:          return "YSL";
    }
    return "UNKNOWN";
}

inline Brand brandFromString(const std::string& s) {
    if (s == "CHANEL")        return Brand::CHANEL;
    if (s == "DIOR")          return Brand::DIOR;
    if (s == "GUCCI")         return Brand::GUCCI;
    if (s == "VERSACE")       return Brand::VERSACE;
    if (s == "PRADA")         return Brand::PRADA;
    if (s == "LOUIS_VUITTON") return Brand::LOUIS_VUITTON;
    if (s == "HERMES")        return Brand::HERMES;
    if (s == "BURBERRY")      return Brand::BURBERRY;
    if (s == "TOM_FORD")      return Brand::TOM_FORD;
    if (s == "YSL")           return Brand::YSL;
    throw std::invalid_argument("Unknown brand: " + s);
}
