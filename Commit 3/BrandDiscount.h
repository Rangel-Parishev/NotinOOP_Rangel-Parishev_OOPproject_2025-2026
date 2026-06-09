#pragma once
#include "Discount.h"
#include "Fragrance.h"

class BrandDiscount : public Discount {
    std::string brandName;
public:
    BrandDiscount(double percent, const std::string& brandName);
    BrandDiscount(unsigned id, double percent, const std::string& brandName);

    const std::string& getBrandName() const;

    double apply(const std::vector<Fragrance>& items) const override;
    std::string describe() const override;
    std::string serialize() const override;
    std::string getType() const override;
};
