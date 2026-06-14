#include "BonusDiscount.h"
#include <sstream>

BonusDiscount::BonusDiscount(double percent, double bonus)
    : Discount(percent), bonus(bonus) {
}

BonusDiscount::BonusDiscount(unsigned id, double percent, double bonus)
    : Discount(id, percent), bonus(bonus) {
}

double BonusDiscount::apply(const std::vector<Fragrance>& items) const {
    double total = 0.0;
    for (const auto& f : items) total += f.getPrice();
    double afterPercent = total * (1.0 - discountPercent / 100.0);
    double afterBonus = afterPercent - bonus;
    return std::max(afterBonus, 0.0);
}

std::string BonusDiscount::describe() const {
    std::ostringstream oss;
    oss << (int)discountPercent << "% off + $" << bonus << " bonus";
    return oss.str();
}

std::string BonusDiscount::serialize() const {
    std::ostringstream oss;
    oss << "BONUS|" << discountId << "|" << discountPercent << "|" << bonus;
    return oss.str();
}

std::string BonusDiscount::getType() const { return "BONUS"; }
