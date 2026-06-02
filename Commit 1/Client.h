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
//add - to - wishlist <fragrance - name> -добавя парфюма с това име към wishlist - а на потребителя
//remove - from - wishlist <fragrance - name> -премахва парфюма с това име от wishlist - а на потребителя
//add - to - cart <fragrance - name> -добавя парфюма с това име към количката на потребителя
//remove - from - cart <fragrance - name> -премахва парфюма с това име от количката на потребителя
//view - cart - показва съдържанието на количката
//view - bought - показва списък от всички успешни покупки(всяка покупка се характеризира с парфюмите в нея и общата цена на поръчката)
//view - purchases - показва списък от всички покупки - за разлика от view - bought тук влиза всеки един вид покупка - PENDING, DELIVERED или CANCELED(това следва да бъде отразено в представянето на покупката)
//recommend - предлага 3 * (свободни сте да промените тази бройка) на брой парфюма, най - близки до предпочитанията на потребителя, на база неговия wishlist.
//За целта първо се намира коя нотка(fragrance family) е най - харесвана от потребителя.След като се определи коя е, системата избира случайно 3 * парфюма със същата нотка, които не са в wishlist - а на купувача.
//checkout - завършва покупката и изпразва количката, стига купувача да има нужната сума в сметката си.
//Ако клиентът има ваучери за намаление, то системата автоматично използва този, който би му дал най - голяма отстъпка
//cancel <purchase - id> -отказва вече създадена поръчка, само ако е със състояние PENDING
//make - review <fragrance - name> <rating> <comment> -създава ревю за дадения парфюм със следната оценка и коментар
