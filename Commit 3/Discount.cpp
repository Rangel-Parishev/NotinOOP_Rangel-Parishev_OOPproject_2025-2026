#include "Discount.h"
#include "Fragrance.h"
#include "Brand.h"
#include <sstream>
#include <stdexcept>
#include <algorithm>

unsigned Discount::nextDiscountId = 1;

Discount::Discount(double percent)
    : discountId(nextDiscountId++), discountPercent(percent) {
}

Discount::Discount(unsigned id, double percent)
    : discountId(id), discountPercent(percent)
{
    if (id >= nextDiscountId) nextDiscountId = id + 1;
}

unsigned Discount::getId() const { return discountId; }
double Discount::getPercent() const { return discountPercent; }

double Discount::apply(const std::vector<Fragrance>& items) const {
    double total = 0.0;
    for (const auto& f : items) total += f.getPrice();
    return total * (1.0 - discountPercent / 100.0);
}

std::string Discount::describe() const {
    return std::to_string((int)discountPercent) + "% off everything";
}

std::string Discount::serialize() const {
    std::ostringstream oss;
    oss << "PERCENT|" << discountId << "|" << discountPercent;
    return oss.str();
}

std::string Discount::getType() const { return "PERCENT"; }

std::unique_ptr<Discount> Discount::deserialize(const std::string& line) {
    std::istringstream iss(line);
    std::string type, token;
    std::getline(iss, type, '|');
    std::getline(iss, token, '|'); unsigned id = std::stoul(token);
    std::getline(iss, token, '|'); double pct = std::stod(token);

    if (type == "BONUS") {
        std::getline(iss, token);
        double bonus = std::stod(token);
        return std::make_unique<BonusDiscount>(id, pct, bonus);
    }
    if (type == "BRAND") {
        std::string brand;
        std::getline(iss, brand);
        return std::make_unique<BrandDiscount>(id, pct, brand);
    }
    return std::make_unique<Discount>(id, pct);
}

void Discount::setNextId(unsigned id) { if (id >= nextDiscountId) nextDiscountId = id + 1; }
