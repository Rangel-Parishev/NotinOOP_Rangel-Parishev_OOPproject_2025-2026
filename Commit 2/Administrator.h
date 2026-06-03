#pragma once
#include "User.h"

class Administrator : public User
{
	Administrator(string username, string password) : User(username, password) {};
	Administrator(const Administrator&) = delete;
	Administrator& operator=(const Administrator&) = delete;

public:
	static Administrator& getInstance(const string& username = "admin", const string& password = "admin123");
	void registerUser(const string& username, const string& password) override;
	void loginUser(const string& username, const string& password) override;
	void logoutUser() override;
	void help() override;
	void blockUser(const string& username);
	void createFragrance(const string& name, const string& brand, double price, const string& fragranceFamily);
	void addQuantity(const string& fragranceName, unsigned quantity);
	void deliverPurchase(unsigned purchaseId);
	void removeReview(unsigned fragranceId, unsigned reviewId);
};
//block - user <username> -блокира даден потребител и изтрива акаунта му
//create - fragrance <name> <brand> <price> <fragranceFamily> -добавя нов парфюм към системата
//add - quantity <fragrance - name> <quantity> -добавя въпросното количество към дадения парфюм
//deliver <purchase - id> -симулира завършена доставка - състоянието на поръчката се сменя от PENDING на DELIVERED
//remove - review <fragrance - id> <review - id> -премахва въпросното ревю за дадения парфюм.Ако един потребител има 7 + премахнати ревюта, той бива автоматично блокиран от системата
