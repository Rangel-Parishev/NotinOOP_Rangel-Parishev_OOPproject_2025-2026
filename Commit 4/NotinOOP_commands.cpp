#include "NotinOOP.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>

void NotinOOP::run() {
    std::cout << "Welcome to NotinOOP Perfume Shop!\n"
        << "Type 'help' for available commands.\n\n";

    std::string line;
    while (true) {
        std::cout << (loggedIn ? loggedIn->getUsername() : "guest") << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;

        auto tokens = tokenize(line);
        std::string cmd = tokens[0];
        std::vector<std::string> args(tokens.begin() + 1, tokens.end());

        if (cmd == "end") {
            save();
            std::cout << "Goodbye!\n";
            return;
        }

        if (!loggedIn) {
            handleLoggedOut(cmd, args);
        }
        else if (loggedIn->isAdmin()) {
            handleAdmin(static_cast<Administrator*>(loggedIn), cmd, args);
        }
        else {
            handleClient(static_cast<Client*>(loggedIn), cmd, args);
        }
    }
}

void NotinOOP::handleLoggedOut(const std::string& cmd,
    const std::vector<std::string>& args) {
    if (cmd == "register") {
        if (args.size() < 2) { std::cout << "Usage: register <username> <password>\n"; return; }
        for (const auto& u : users)
            if (u->getUsername() == args[0]) { std::cout << "Username taken.\n"; return; }
        users.push_back(std::make_unique<Client>(args[0], args[1]));
        std::cout << "Registered successfully. You can now log in.\n";
    }
    else if (cmd == "login") {
        if (args.size() < 2) { std::cout << "Usage: login <username> <password>\n"; return; }
        for (auto& u : users) {
            if (u->getUsername() == args[0]) {
                if (u->isBlocked()) { std::cout << "This account has been blocked.\n"; return; }
                if (!u->checkPassword(args[1])) { std::cout << "Wrong password.\n"; return; }
                loggedIn = u.get();
                std::cout << "Logged in as " << args[0] << ".\n";
                return;
            }
        }
        std::cout << "User not found.\n";
    }
    else if (cmd == "help") {
        std::cout << "  register <username> <password>\n"
            << "  login <username> <password>\n"
            << "  end\n";
    }
    else {
        std::cout << "Please log in first.\n";
    }
}

void NotinOOP::handleClient(Client* c, const std::string& cmd,
    const std::vector<std::string>& args) {
    if (cmd == "logout") {
        std::cout << "Logged out.\n";
        loggedIn = nullptr;
    }
    else if (cmd == "help") {
        c->help();
    }
    else if (cmd == "add-to-balance") {
        if (args.empty()) { std::cout << "Usage: add-to-balance <amount>\n"; return; }
        try { c->addToBalance(std::stod(args[0])); }
        catch (...) { std::cout << "Invalid amount.\n"; }
    }
    else if (cmd == "add-to-wishlist") {
        if (args.empty()) { std::cout << "Usage: add-to-wishlist <fragrance-name>\n"; return; }
        c->addToWishlist(args[0], catalog);
    }
    else if (cmd == "remove-from-wishlist") {
        if (args.empty()) { std::cout << "Usage: remove-from-wishlist <fragrance-name>\n"; return; }
        c->removeFromWishlist(args[0]);
    }
    else if (cmd == "add-to-cart") {
        if (args.empty()) { std::cout << "Usage: add-to-cart <fragrance-name>\n"; return; }
        c->addToCart(args[0], catalog);
    }
    else if (cmd == "remove-from-cart") {
        if (args.empty()) { std::cout << "Usage: remove-from-cart <fragrance-name>\n"; return; }
        c->removeFromCart(args[0]);
    }
    else if (cmd == "view-cart") { c->viewCart(); }
    else if (cmd == "view-bought") { c->viewBought(); }
    else if (cmd == "view-purchases") { c->viewPurchases(); }
    else if (cmd == "recommend") { c->recommend(catalog); }
    else if (cmd == "checkout") { c->checkout(catalog); }
    else if (cmd == "cancel") {
        if (args.empty()) { std::cout << "Usage: cancel <purchase-id>\n"; return; }
        try { c->cancel(std::stoul(args[0])); }
        catch (...) { std::cout << "Invalid id.\n"; }
    }
    else if (cmd == "make-review") {
        if (args.size() < 3) {
            std::cout << "Usage: make-review <fragrance-name> <rating> <comment>\n"; return;
        }
        Fragrance* f = findFragrance(args[0]);
        if (!f) { std::cout << "Fragrance not found.\n"; return; }
        try {
            double rating = std::stod(args[1]);
            std::string comment;
            for (size_t i = 2; i < args.size(); ++i) {
                if (i > 2) comment += " ";
                comment += args[i];
            }
            Review r(c->getId(), f->getName(), comment, rating);
            f->addReview(r);
            std::cout << "Review added.\n";
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
    else if (cmd == "browse") {
        if (catalog.empty()) { std::cout << "No fragrances available.\n"; return; }
        for (const auto& f : catalog) f.show();
    }
    else {
        std::cout << "Unknown command. Type 'help' for options.\n";
    }
}

void NotinOOP::handleAdmin(Administrator* a, const std::string& cmd,
    const std::vector<std::string>& args) {
    if (cmd == "logout") {
        std::cout << "Logged out.\n";
        loggedIn = nullptr;
    }
    else if (cmd == "help") {
        a->help();
    }
    else if (cmd == "block-user") {
        if (args.empty()) { std::cout << "Usage: block-user <username>\n"; return; }
        Client* c = findClient(args[0]);
        if (!c) { std::cout << "User not found.\n"; return; }
        users.erase(std::remove_if(users.begin(), users.end(),
            [&](const std::unique_ptr<User>& u) { return u.get() == c; }),
            users.end());
        if (loggedIn == c) loggedIn = nullptr;
        std::cout << "User " << args[0] << " has been blocked and removed.\n";
    }
    else if (cmd == "create-fragrance") {
        if (args.size() < 4) {
            std::cout << "Usage: create-fragrance <name> <brand> <price> <fragranceFamily>\n"; return;
        }
        try {
            Brand b = brandFromString(args[1]);
            double price = std::stod(args[2]);
            FragranceFamily ff = fragranceFamilyFromString(args[3]);
            catalog.emplace_back(args[0], b, price, ff, 0);
            std::cout << "Fragrance '" << args[0] << "' added to catalog.\n";
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
    else if (cmd == "add-quantity") {
        if (args.size() < 2) { std::cout << "Usage: add-quantity <fragrance-name> <qty>\n"; return; }
        Fragrance* f = findFragrance(args[0]);
        if (!f) { std::cout << "Fragrance not found.\n"; return; }
        try {
            unsigned qty = std::stoul(args[1]);
            f->addQuantity(qty);
            std::cout << "Quantity updated. New stock: " << f->getQuantity() << "\n";
        }
        catch (...) { std::cout << "Invalid quantity.\n"; }
    }
    else if (cmd == "deliver") {
        if (args.empty()) { std::cout << "Usage: deliver <purchase-id>\n"; return; }
        unsigned pid;
        try { pid = std::stoul(args[0]); }
        catch (...) { std::cout << "Invalid id.\n"; return; }
        for (auto& u : users) {
            if (u->isAdmin()) continue;
            Client* c = static_cast<Client*>(u.get());
            for (auto& p : c->getPurchases()) {
                if (p.getId() == pid) {
                    if (p.getState() != PurchaseState::PENDING) {
                        std::cout << "Purchase is not PENDING.\n"; return;
                    }
                    p.setState(PurchaseState::DELIVERED);
                    std::cout << "Purchase #" << pid << " marked as DELIVERED.\n";
                    return;
                }
            }
        }
        std::cout << "Purchase not found.\n";
    }
    else if (cmd == "remove-review") {
        if (args.size() < 2) { std::cout << "Usage: remove-review <fragrance-id> <review-id>\n"; return; }
        unsigned fid, rid;
        try { fid = std::stoul(args[0]); rid = std::stoul(args[1]); }
        catch (...) { std::cout << "Invalid ids.\n"; return; }

        Fragrance* f = findFragranceById(fid);
        if (!f) { std::cout << "Fragrance not found.\n"; return; }

        unsigned reviewOwner = 0;
        for (const auto& r : f->getReviews())
            if (r.getId() == rid) { reviewOwner = r.getUserId(); break; }

        if (!f->removeReview(rid)) { std::cout << "Review not found.\n"; return; }

        std::cout << "Review #" << rid << " removed.\n";

        if (reviewOwner) {
            Client* c = findClientById(reviewOwner);
            if (c) {
                c->incrementRemovedReviews();
                if (c->getRemovedReviews() >= 7) {
                    std::cout << "User " << c->getUsername()
                        << " has had 7+ reviews removed — auto-blocked.\n";
                    c->setBlocked(true);
                    users.erase(std::remove_if(users.begin(), users.end(),
                        [c](const std::unique_ptr<User>& u) { return u.get() == c; }),
                        users.end());
                    if (loggedIn == c) loggedIn = nullptr;
                }
            }
        }
    }
    else if (cmd == "browse") {
        for (const auto& f : catalog) f.show();
    }
    else {
        std::cout << "Unknown command. Type 'help' for options.\n";
    }
}
