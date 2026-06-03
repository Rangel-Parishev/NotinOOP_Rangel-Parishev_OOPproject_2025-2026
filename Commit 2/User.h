#pragma once
#include <string>
using namespace std;

class User
{
protected:
	unsigned userId;
	static unsigned nextUserId;
	string username;
	string password;
	User(string username, string password) : userId(++nextUserId), username(username), password(password) {};
	virtual ~User() = default;

public:
	virtual void registerUser(const string& username, const string& password) = 0;
	virtual void loginUser(const string& username, const string& password) = 0;
	virtual void logoutUser() = 0;
	virtual void help() = 0;
};