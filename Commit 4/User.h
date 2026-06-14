#pragma once
#include <string>

class User {
protected:
    unsigned userId;
    static unsigned nextUserId;
    std::string username;
    std::string password;
    bool blocked = false;

    User(const std::string& username, const std::string& password);
    User(unsigned id, const std::string& username, const std::string& password);

public:
    virtual ~User() = default;

    unsigned getId() const;
    const std::string& getUsername() const;
    bool isBlocked() const;
    void setBlocked(bool b);

    bool checkPassword(const std::string& pw) const;

    virtual void help() const = 0;
    virtual bool isAdmin() const;

    virtual std::string serialize() const;
    virtual std::string getRole() const = 0;

    static void setNextId(unsigned id);
};
