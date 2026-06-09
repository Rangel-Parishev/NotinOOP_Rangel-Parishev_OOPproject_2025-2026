#include "Administrator.h"
#include <iostream>

Administrator::Administrator(const std::string& username, const std::string& password)
    : User(username, password) {
}

Administrator::Administrator(unsigned id, const std::string& username, const std::string& password)
    : User(id, username, password) {
}

void Administrator::help() const {
    std::cout <<
        "Available commands:\n"
        "  logout\n"
        "  block-user <username>\n"
        "  create-fragrance <name> <brand> <price> <fragranceFamily>\n"
        "  add-quantity <fragrance-name> <quantity>\n"
        "  deliver <purchase-id>\n"
        "  remove-review <fragrance-id> <review-id>\n"
        "  help\n";
}

bool Administrator::isAdmin() const { return true; }
std::string Administrator::getRole() const { return "ADMIN"; }