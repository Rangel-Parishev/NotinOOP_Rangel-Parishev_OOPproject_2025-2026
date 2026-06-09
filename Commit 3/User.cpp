#include "User.h"
#include <sstream>

unsigned User::nextUserId = 1;

User::User(const std::string& username, const std::string& password) 
    : userId(nextUserId++), username(username), password(password) {
}

User::User(unsigned id, const std::string& username, const std::string& password)
    : userId(id), username(username), password(password) {
    if (id >= nextUserId) nextUserId = id + 1;
}

unsigned User::getId() const { return userId; }
const std::string& User::getUsername() const { return username; }
bool User::isBlocked() const { return blocked; }
void User::setBlocked(bool b) { blocked = b; }

bool User::checkPassword(const std::string& pw) const { return password == pw; }
bool User::isAdmin() const { return false; }

void User::setNextId(unsigned id) { if (id >= nextUserId) nextUserId = id + 1; }

std::string User::serialize() const {
    std::ostringstream oss;
    oss << getRole() << "|" << userId << "|" << username << "|" << password << "|" << (blocked ? 1 : 0);
    return oss.str();
}
