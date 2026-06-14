#pragma once
#include <string>
#include <vector>
#include "Brand.h"
#include "FragranceFamily.h"
#include "Review.h"

class Fragrance {
    unsigned fragranceId;
    static unsigned nextFragranceId;

    std::string name;
    Brand brand;
    double price;
    FragranceFamily fragranceFamily;
    unsigned quantity;
    std::vector<Review> reviews;

public:
    Fragrance(const std::string& name, Brand brand, double price, FragranceFamily family, unsigned quantity = 0);

    Fragrance(unsigned id, const std::string& name, Brand brand, double price, FragranceFamily family, unsigned quantity);

    unsigned getId() const;
    const std::string& getName() const;
    Brand getBrand() const;
    double getPrice() const;
    FragranceFamily getFamily() const;
    unsigned getQuantity() const;
    const std::vector<Review>& getReviews() const;
    void addQuantity(unsigned qty);
    bool reduceQuantity(unsigned qty);

    double getRating() const;

    void addReview(const Review& r);
    bool removeReview(unsigned reviewId);

    void show() const;

    std::string serialize() const;
    static Fragrance deserialize(const std::string& line);

    static void setNextId(unsigned id);
};
