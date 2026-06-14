#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Fragrance.h"
#include "User.h"
#include "Client.h"
#include "Administrator.h"
#include "Purchase.h"

class NotinOOP {
    std::vector<Fragrance> catalog;
    std::vector<std::unique_ptr<User>> users;
    User* loggedIn = nullptr;

    static const std::string DB_FILE;

    Fragrance* findFragrance(const std::string& name);
    Fragrance* findFragranceById(unsigned id);
    Client* findClient(const std::string& username);
    Client* findClientById(unsigned id);

    void handleLoggedOut(const std::string& cmd,
        const std::vector<std::string>& args);
    void handleClient(Client* c,
        const std::string& cmd,
        const std::vector<std::string>& args);
    void handleAdmin(Administrator* a,
        const std::string& cmd,
        const std::vector<std::string>& args);

    void load();
    void save() const;

    static std::vector<std::string> tokenize(const std::string& line);

public:
    NotinOOP();
    void run();
};
