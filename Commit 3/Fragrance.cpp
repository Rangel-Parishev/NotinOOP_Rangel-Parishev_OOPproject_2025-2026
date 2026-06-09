#include "Fragrance.h"
#include <iostream>
#include <sstream>
#include <numeric>
#include <algorithm>

unsigned Fragrance::nextFragranceId = 1;

Fragrance::Fragrance(const std::string& name, Brand brand, double price, FragranceFamily family, unsigned quantity)
    : fragranceId(nextFragranceId++), name(name), brand(brand), price(price), fragranceFamily(family), quantity(quantity) {
}

Fragrance::Fragrance(unsigned id, const std::string& name, Brand brand, double price, FragranceFamily family, unsigned quantity)
    : fragranceId(id), name(name), brand(brand), price(price), fragranceFamily(family), quantity(quantity)
{
    if (id >= nextFragranceId) nextFragranceId = id + 1;
}

unsigned Fragrance::getId() const { return fragranceId; }
const std::string& Fragrance::getName() const { return name; }
Brand Fragrance::getBrand() const { return brand; }
double Fragrance::getPrice() const { return price; }
FragranceFamily Fragrance::getFamily() const { return fragranceFamily; }
unsigned Fragrance::getQuantity() const { return quantity; }
const std::vector<Review>& Fragrance::getReviews() const { return reviews; }

void Fragrance::addQuantity(unsigned qty) { quantity += qty; }

bool Fragrance::reduceQuantity(unsigned qty) {
    if (quantity < qty) return false;
    quantity -= qty;
    return true;
}

double Fragrance::getRating() const {
    if (reviews.empty()) return 0.0;
    double sum = 0.0;
    for (const auto& r : reviews) sum += r.getRating();
    return sum / reviews.size();
}

void Fragrance::addReview(const Review& r) { reviews.push_back(r); }

bool Fragrance::removeReview(unsigned reviewId) {
    auto it = std::find_if(reviews.begin(), reviews.end(),
        [reviewId](const Review& r) { return r.getId() == reviewId; });
    if (it == reviews.end()) return false;
    reviews.erase(it);
    return true;
}

void Fragrance::show() const {
    std::cout << "[#" << fragranceId << "] " << name
        << " | Brand: " << brandToString(brand)
        << " | Family: " << fragranceFamilyToString(fragranceFamily)
        << " | Price: $" << price
        << " | Qty: " << quantity
        << " | Rating: " << getRating() << "/5\n";
}

std::string Fragrance::serialize() const {
    std::ostringstream oss;
    oss << fragranceId << "|" << name << "|" << brandToString(brand)
        << "|" << price << "|" << fragranceFamilyToString(fragranceFamily)
        << "|" << quantity;
    return oss.str();
}

Fragrance Fragrance::deserialize(const std::string& line) {
    std::istringstream iss(line);
    std::string token;
    unsigned id, qty;
    std::string name;
    double price;

    std::getline(iss, token, '|'); id = std::stoul(token);
    std::getline(iss, name, '|');
    std::getline(iss, token, '|'); Brand brand = brandFromString(token);
    std::getline(iss, token, '|'); price = std::stod(token);
    std::getline(iss, token, '|'); FragranceFamily family = fragranceFamilyFromString(token);
    std::getline(iss, token);      qty = std::stoul(token);

    return Fragrance(id, name, brand, price, family, qty);
}

void Fragrance::setNextId(unsigned id) { if (id >= nextFragranceId) nextFragranceId = id + 1; }