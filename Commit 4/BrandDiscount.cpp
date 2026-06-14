#include "BrandDiscount.h"
#include "Brand.h"
#include <sstream>

BrandDiscount::BrandDiscount(double percent, const std::string& bname)
    : Discount(percent), brandName(bname) {
}

BrandDiscount::BrandDiscount(unsigned id, double percent, const std::string& bname)
    : Discount(id, percent), brandName(bname) {
}

const std::string& BrandDiscount::getBrandName() const { return brandName; }

double BrandDiscount::apply(const std::vector<Fragrance>& items) const {
    double total = 0.0;
    for (const auto& f : items) {
        double price = f.getPrice();
        if (brandToString(f.getBrand()) == brandName)
            price *= (1.0 - discountPercent / 100.0);
        total += price;
    }
    return total;
}

std::string BrandDiscount::describe() const {
    std::ostringstream oss;
    oss << (int)discountPercent << "% off " << brandName << " items";
    return oss.str();
}

std::string BrandDiscount::serialize() const {
    std::ostringstream oss;
    oss << "BRAND|" << discountId << "|" << discountPercent << "|" << brandName;
    return oss.str();
}

std::string BrandDiscount::getType() const { return "BRAND"; }