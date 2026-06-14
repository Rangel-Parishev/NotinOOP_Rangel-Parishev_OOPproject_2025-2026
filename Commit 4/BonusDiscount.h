#pragma once
#include "Discount.h"
#include "Fragrance.h"

class BonusDiscount : public Discount {
    double bonus;
public:
    BonusDiscount(double percent, double bonus);
    BonusDiscount(unsigned id, double percent, double bonus);

    double bonus_amount() const { return bonus; }

    double apply(const std::vector<Fragrance>& items) const override;
    std::string describe() const override;
    std::string serialize() const override;
    std::string getType() const override;
};