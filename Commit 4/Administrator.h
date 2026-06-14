#pragma once
#include "User.h"

class Administrator : public User {
public:
    Administrator(const std::string& username, const std::string& password);
    Administrator(unsigned id, const std::string& username, const std::string& password);

    void help() const override;
    bool isAdmin() const override;
    std::string getRole() const override;
};
