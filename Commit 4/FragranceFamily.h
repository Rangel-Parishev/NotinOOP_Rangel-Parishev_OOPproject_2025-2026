#pragma once
#include <string>
#include <stdexcept>

enum class FragranceFamily {
    CITRUS,
    FLORAL,
    ORIENTAL,
    WOODY,
    FRESH,
    AQUATIC,
    SPICY,
    GOURMAND,
    FRUITY,
    LEATHER
};

inline std::string fragranceFamilyToString(FragranceFamily f) {
    switch (f) {
    case FragranceFamily::CITRUS:    return "CITRUS";
    case FragranceFamily::FLORAL:    return "FLORAL";
    case FragranceFamily::ORIENTAL:  return "ORIENTAL";
    case FragranceFamily::WOODY:     return "WOODY";
    case FragranceFamily::FRESH:     return "FRESH";
    case FragranceFamily::AQUATIC:   return "AQUATIC";
    case FragranceFamily::SPICY:     return "SPICY";
    case FragranceFamily::GOURMAND:  return "GOURMAND";
    case FragranceFamily::FRUITY:    return "FRUITY";
    case FragranceFamily::LEATHER:   return "LEATHER";
    }
    return "UNKNOWN";
}

inline FragranceFamily fragranceFamilyFromString(const std::string& s) {
    if (s == "CITRUS")   return FragranceFamily::CITRUS;
    if (s == "FLORAL")   return FragranceFamily::FLORAL;
    if (s == "ORIENTAL") return FragranceFamily::ORIENTAL;
    if (s == "WOODY")    return FragranceFamily::WOODY;
    if (s == "FRESH")    return FragranceFamily::FRESH;
    if (s == "AQUATIC")  return FragranceFamily::AQUATIC;
    if (s == "SPICY")    return FragranceFamily::SPICY;
    if (s == "GOURMAND") return FragranceFamily::GOURMAND;
    if (s == "FRUITY")   return FragranceFamily::FRUITY;
    if (s == "LEATHER")  return FragranceFamily::LEATHER;
    throw std::invalid_argument("Unknown fragrance family: " + s);
}
