#pragma once
#include "User.h"
#include "Fragrance.h"
#include "Purchase.h"
#include "Discount.h"
#include <vector>
#include <memory>

class Client : public User {
    std::vector<Fragrance> cart;
    std::vector<Purchase>  purchases;
    std::vector<std::string> wishlist;
    double balance = 0.0;
    std::vector<std::unique_ptr<Discount>> discounts;
    unsigned removedReviewCount = 0;

public:
    Client(const std::string& username, const std::string& password);
    Client(unsigned id, const std::string& username, const std::string& password, double balance, unsigned removedReviews);

    double getBalance() const;
    const std::vector<std::string>& getWishlist() const;
    const std::vector<Purchase>& getPurchases() const;
    std::vector<Purchase>& getPurchases();
    const std::vector<Fragrance>& getCart() const;
    unsigned getRemovedReviews() const;
    void incrementRemovedReviews();

    void addToBalance(double amount);

    void addToWishlist(const std::string& fragranceName, const std::vector<Fragrance>& catalog);
    void removeFromWishlist(const std::string& fragranceName);
    void addToCart(const std::string& fragranceName, const std::vector<Fragrance>& catalog);
    void removeFromCart(const std::string& fragranceName);
    void viewCart() const;
    void viewBought() const;
    void viewPurchases() const;

    int bestDiscountIndex(const std::vector<Fragrance>& items) const;

    Purchase* checkout(std::vector<Fragrance>& catalog);

    void cancel(unsigned purchaseId);
    void addDiscount(std::unique_ptr<Discount> d);

    void recommend(const std::vector<Fragrance>& catalog) const;

    void help() const override;
    bool isAdmin() const override;
    std::string getRole() const override;

    std::string serialize() const override;
    std::string serializeWishlist() const;
    std::string serializeDiscounts() const;

    void loadWishlist(const std::string& line);
    void addLoadedDiscount(std::unique_ptr<Discount> d);
};
