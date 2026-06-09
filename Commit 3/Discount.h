#pragma once
#include <string>
#include <vector>
#include <memory>

class Discount {
protected:
    unsigned discountId;
    static unsigned nextDiscountId;
    double discountPercent;

public:
    Discount(double percent);
    Discount(unsigned id, double percent);
    virtual ~Discount() = default;

    unsigned getId() const;
    double getPercent() const;

    virtual double apply(const std::vector<Fragrance>& items) const;

    virtual std::string describe() const;

    virtual std::string serialize() const;
    virtual std::string getType() const;

    static std::unique_ptr<Discount> deserialize(const std::string& line);
    static void setNextId(unsigned id);
};