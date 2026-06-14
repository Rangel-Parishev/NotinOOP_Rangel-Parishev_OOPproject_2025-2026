#include "Client.h"
#include <iostream>
#include "BonusDiscount.h"
#include "BrandDiscount.h"
#include <sstream>
#include <algorithm>
#include <map>
#include <random>
#include <iomanip>

Client::Client(const std::string& username, const std::string& password)
    : User(username, password), balance(0.0), removedReviewCount(0) {
}

Client::Client(unsigned id, const std::string& username, const std::string& password, double balance, unsigned removedReviews)
    : User(id, username, password), balance(balance), removedReviewCount(removedReviews) {
}

double Client::getBalance() const { return balance; }
const std::vector<std::string>& Client::getWishlist() const { return wishlist; }
const std::vector<Purchase>& Client::getPurchases() const { return purchases; }
std::vector<Purchase>& Client::getPurchases() { return purchases; }
const std::vector<Fragrance>& Client::getCart() const { return cart; }
unsigned Client::getRemovedReviews() const { return removedReviewCount; }
void Client::incrementRemovedReviews() { ++removedReviewCount; }

void Client::addToBalance(double amount) {
    if (amount <= 0) { std::cout << "Amount must be positive.\n"; return; }
    balance += amount;
    std::cout << "Balance updated. New balance: $" << std::fixed << std::setprecision(2) << balance << "\n";
}

void Client::addToWishlist(const std::string& fragranceName,
    const std::vector<Fragrance>& catalog) {
    auto it = std::find_if(catalog.begin(), catalog.end(), [&](const Fragrance& f) { return f.getName() == fragranceName; });
    if (it == catalog.end()) { std::cout << "Fragrance not found.\n"; return; }

    if (std::find(wishlist.begin(), wishlist.end(), fragranceName) != wishlist.end()) {
        std::cout << fragranceName << " is already in your wishlist.\n"; return;
    }
    wishlist.push_back(fragranceName);
    std::cout << fragranceName << " added to wishlist.\n";
}

void Client::removeFromWishlist(const std::string& fragranceName) {
    auto it = std::find(wishlist.begin(), wishlist.end(), fragranceName);
    if (it == wishlist.end()) { std::cout << "Not in wishlist.\n"; return; }
    wishlist.erase(it);
    std::cout << fragranceName << " removed from wishlist.\n";
}

void Client::addToCart(const std::string& fragranceName,
    const std::vector<Fragrance>& catalog) {
    auto it = std::find_if(catalog.begin(), catalog.end(), [&](const Fragrance& f) { return f.getName() == fragranceName; });
    if (it == catalog.end()) { std::cout << "Fragrance not found.\n"; return; }
    if (it->getQuantity() == 0) { std::cout << "Out of stock.\n"; return; }
    cart.push_back(*it);
    std::cout << fragranceName << " added to cart.\n";
}

void Client::removeFromCart(const std::string& fragranceName) {
    auto it = std::find_if(cart.begin(), cart.end(), [&](const Fragrance& f) { return f.getName() == fragranceName; });
    if (it == cart.end()) { std::cout << "Not in cart.\n"; return; }
    cart.erase(it);
    std::cout << fragranceName << " removed from cart.\n";
}

void Client::viewCart() const {
    if (cart.empty()) { std::cout << "Your cart is empty.\n"; return; }
    double total = 0.0;
    std::cout << "-- Cart --\n";
    for (const auto& f : cart) {
        std::cout << "  " << f.getName() << " ($" << f.getPrice() << ")\n";
        total += f.getPrice();
    }
    std::cout << "  Total: $" << std::fixed << std::setprecision(2) << total << "\n";

    if (!discounts.empty()) {
        int best = bestDiscountIndex(cart);
        if (best >= 0) {
            double discounted = discounts[best]->apply(cart);
            std::cout << "  Best voucher: " << discounts[best]->describe() << " → $" << std::fixed << std::setprecision(2) << discounted << "\n";
        }
    }
}

void Client::viewBought() const {
    bool any = false;
    for (const auto& p : purchases) {
        if (p.getState() == PurchaseState::DELIVERED) {
            p.show(); any = true;
        }
    }
    if (!any) std::cout << "No delivered purchases yet.\n";
}

void Client::viewPurchases() const {
    if (purchases.empty()) { std::cout << "No purchases yet.\n"; return; }
    for (const auto& p : purchases) p.show();
}

int Client::bestDiscountIndex(const std::vector<Fragrance>& items) const {
    if (discounts.empty()) return -1;
    int best = 0;
    double bestTotal = discounts[0]->apply(items);
    for (size_t i = 1; i < discounts.size(); ++i) {
        double t = discounts[i]->apply(items);
        if (t < bestTotal) { bestTotal = t; best = (int)i; }
    }
    return best;
}

Purchase* Client::checkout(std::vector<Fragrance>& catalog) {
    if (cart.empty()) { std::cout << "Cart is empty.\n"; return nullptr; }

    double total;
    int best = bestDiscountIndex(cart);
    if (best >= 0) {
        total = discounts[best]->apply(cart);
        std::cout << "Voucher applied: " << discounts[best]->describe() << "\n";
        discounts.erase(discounts.begin() + best);
    }
    else {
        total = 0.0;
        for (const auto& f : cart) total += f.getPrice();
    }

    if (balance < total) {
        std::cout << "Insufficient balance. Need $" << std::fixed << std::setprecision(2) << total << ", have $" << balance << "\n";
        return nullptr;
    }

    for (const auto& cartItem : cart) {
        auto it = std::find_if(catalog.begin(), catalog.end(), [&](Fragrance& f) { return f.getId() == cartItem.getId(); });
        if (it != catalog.end()) it->reduceQuantity(1);
    }

    balance -= total;
    purchases.emplace_back(userId, cart);
    cart.clear();

    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> pctDist(5, 25);
    int pct = pctDist(rng);
    int type = std::uniform_int_distribution<int>(0, 2)(rng);
    if (type == 0)
        discounts.push_back(std::make_unique<Discount>(pct));
    else if (type == 1)
        discounts.push_back(std::make_unique<BonusDiscount>(pct, 5.0 * (pct / 5)));
    else {
        std::vector<std::string> brands = { "CHANEL","DIOR","GUCCI","TOM_FORD","YSL" };
        std::string brand = brands[std::uniform_int_distribution<int>(0, 4)(rng)];
        discounts.push_back(std::make_unique<BrandDiscount>(pct, brand));
    }

    std::cout << "Checkout successful! Total charged: $" << std::fixed << std::setprecision(2) << total
        << " | New balance: $" << balance << "\n";
    std::cout << "You received a new discount voucher!\n";

    return &purchases.back();
}

void Client::cancel(unsigned purchaseId) {
    for (auto& p : purchases) {
        if (p.getId() == purchaseId) {
            if (p.getState() != PurchaseState::PENDING) {
                std::cout << "Only PENDING orders can be cancelled.\n"; return;
            }
            p.setState(PurchaseState::CANCELED);
            std::cout << "Purchase #" << purchaseId << " cancelled.\n";
            return;
        }
    }
    std::cout << "Purchase not found.\n";
}

void Client::addDiscount(std::unique_ptr<Discount> d) { discounts.push_back(std::move(d)); }

void Client::recommend(const std::vector<Fragrance>& catalog) const {
    if (wishlist.empty()) {
        std::cout << "Your wishlist is empty. Add some fragrances first.\n"; return;
    }

    // Count family occurrences in wishlist
    std::map<FragranceFamily, int> familyCount;
    for (const auto& name : wishlist) {
        auto it = std::find_if(catalog.begin(), catalog.end(), [&](const Fragrance& f) { return f.getName() == name; });
        if (it != catalog.end())
            familyCount[it->getFamily()]++;
    }
    if (familyCount.empty()) { std::cout << "No matches in catalog.\n"; return; }

    auto maxIt = std::max_element(familyCount.begin(), familyCount.end(), [](const auto& a, const auto& b) { return a.second < b.second; });
    FragranceFamily topFamily = maxIt->first;

    std::vector<const Fragrance*> candidates;
    for (const auto& f : catalog) {
        if (f.getFamily() != topFamily) continue;
        if (f.getQuantity() == 0) continue;
        if (std::find(wishlist.begin(), wishlist.end(), f.getName()) != wishlist.end()) continue;
        candidates.push_back(&f);
    }

    if (candidates.empty()) {
        std::cout << "No recommendations available (all "
            << fragranceFamilyToString(topFamily) << " fragrances are in your wishlist).\n";
        return;
    }

    std::vector<const Fragrance*> shuffled = candidates;
    static std::mt19937 rng(std::random_device{}());
    std::shuffle(shuffled.begin(), shuffled.end(), rng);

    std::cout << "-- Recommendations (" << fragranceFamilyToString(topFamily) << ") --\n";
    for (size_t i = 0; i < std::min(shuffled.size(), size_t(3)); ++i)
        shuffled[i]->show();
}

void Client::help() const {
    std::cout <<
        "Available commands:\n"
        "  logout\n"
        "  add-to-balance <amount>\n"
        "  add-to-wishlist <fragrance-name>\n"
        "  remove-from-wishlist <fragrance-name>\n"
        "  add-to-cart <fragrance-name>\n"
        "  remove-from-cart <fragrance-name>\n"
        "  view-cart\n"
        "  view-bought\n"
        "  view-purchases\n"
        "  recommend\n"
        "  checkout\n"
        "  cancel <purchase-id>\n"
        "  make-review <fragrance-name> <rating> <comment>\n"
        "  help\n";
}

bool Client::isAdmin() const { return false; }
std::string Client::getRole() const { return "CLIENT"; }

std::string Client::serialize() const {
    std::ostringstream oss;
    oss << "CLIENT|" << userId << "|" << username << "|" << password
        << "|" << (blocked ? 1 : 0)
        << "|" << balance << "|" << removedReviewCount;
    return oss.str();
}

std::string Client::serializeWishlist() const {
    if (wishlist.empty()) return "";
    std::ostringstream oss;
    oss << "WISHLIST|" << userId << "|";
    for (size_t i = 0; i < wishlist.size(); ++i) {
        if (i > 0) oss << ",";
        oss << wishlist[i];
    }
    return oss.str();
}

std::string Client::serializeDiscounts() const {
    std::ostringstream oss;
    for (const auto& d : discounts)
        oss << "DISCOUNT|" << userId << "|" << d->serialize() << "\n";
    return oss.str();
}

void Client::loadWishlist(const std::string& line) {
    std::istringstream iss(line);
    std::string name;
    while (std::getline(iss, name, ','))
        if (!name.empty()) wishlist.push_back(name);
}

void Client::addLoadedDiscount(std::unique_ptr<Discount> d) { discounts.push_back(std::move(d)); }