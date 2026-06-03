#pragma once
#include "User.h"
#include <memory>
#include <vector>
using namespace std;

class Client : public User
{
	vector<Fragrance> cart;
	vector<Purchase> purchases;
	vector<Fragrance> wishlist;
	double balance;
	vector<unique_ptr<Discount>> discounts;

public:
	void addToBalance(double amount);
	void addToWishlist(const string& fragranceName);
	void removeFromWishlist(const string& fragranceName);
	void addToCart(const string& fragranceName);
	void removeFromCart(const string& fragranceName);
	void viewCart() const;
	void viewBought() const;
	void viewPurchases() const;
	void recommend() const;
	void checkout();
	void cancel(unsigned purchaseId);
	void makeReview(const string& fragranceName, double rating, const string& comment);
};
