#include "NotinOOP.h"
#include "Discount.h"
#include "BonusDiscount.h"
#include "BrandDiscount.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

const std::string NotinOOP::DB_FILE = "database.txt";

NotinOOP::NotinOOP() {
    load();
    bool hasAdmin = false;
    for (const auto& u : users)
        if (u->isAdmin()) { hasAdmin = true; break; }
    if (!hasAdmin)
        users.push_back(std::make_unique<Administrator>("admin", "admin123"));
}

Fragrance* NotinOOP::findFragrance(const std::string& name) {
    for (auto& f : catalog)
        if (f.getName() == name) return &f;
    return nullptr;
}

Fragrance* NotinOOP::findFragranceById(unsigned id) {
    for (auto& f : catalog)
        if (f.getId() == id) return &f;
    return nullptr;
}

Client* NotinOOP::findClient(const std::string& username) {
    for (auto& u : users)
        if (!u->isAdmin() && u->getUsername() == username)
            return static_cast<Client*>(u.get());
    return nullptr;
}

Client* NotinOOP::findClientById(unsigned id) {
    for (auto& u : users)
        if (!u->isAdmin() && u->getId() == id)
            return static_cast<Client*>(u.get());
    return nullptr;
}

std::vector<std::string> NotinOOP::tokenize(const std::string& line) {
    std::vector<std::string> tokens;
    std::string cur;
    bool inQuote = false;
    for (char c : line) {
        if (c == '"') { inQuote = !inQuote; continue; }
        if (c == ' ' && !inQuote) {
            if (!cur.empty()) { tokens.push_back(cur); cur.clear(); }
        }
        else {
            cur += c;
        }
    }
    if (!cur.empty()) tokens.push_back(cur);
    return tokens;
}

void NotinOOP::load() {
    std::ifstream f(DB_FILE);
    if (!f.is_open()) return;

    std::string line;

    struct PurchaseRecord {
        unsigned purchaseId, userId;
        PurchaseState state;
        std::vector<unsigned> fragIds;
    };
    std::vector<PurchaseRecord> purchaseRecords;

    while (std::getline(f, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string tag;
        std::getline(iss, tag, '|');

        if (tag == "FRAGRANCE") {
            try {
                Fragrance fr = Fragrance::deserialize(line.substr(10));
                catalog.push_back(fr);
            }
            catch (...) {}
        }
        else if (tag == "REVIEW") {
            std::string fragIdStr, reviewData;
            std::getline(iss, fragIdStr, '|');
            std::getline(iss, reviewData);
            try {
                unsigned fragId = std::stoul(fragIdStr);
                Review r = Review::deserialize(reviewData);
                Fragrance* frag = findFragranceById(fragId);
                if (frag) frag->addReview(r);
            }
            catch (...) {}
        }
        else if (tag == "CLIENT") {
            try {
                std::string sid, uname, pw, sblocked, sbal, sremoved;
                std::getline(iss, sid, '|');
                std::getline(iss, uname, '|');
                std::getline(iss, pw, '|');
                std::getline(iss, sblocked, '|');
                std::getline(iss, sbal, '|');
                std::getline(iss, sremoved);
                auto c = std::make_unique<Client>(
                    std::stoul(sid), uname, pw,
                    std::stod(sbal), std::stoul(sremoved));
                c->setBlocked(sblocked == "1");
                users.push_back(std::move(c));
            }
            catch (...) {}
        }
        else if (tag == "ADMIN") {
            try {
                std::string sid, uname, pw, sblocked;
                std::getline(iss, sid, '|');
                std::getline(iss, uname, '|');
                std::getline(iss, pw, '|');
                std::getline(iss, sblocked);
                auto a = std::make_unique<Administrator>(
                    std::stoul(sid), uname, pw);
                a->setBlocked(sblocked == "1");
                users.push_back(std::move(a));
            }
            catch (...) {}
        }
        else if (tag == "WISHLIST") {
            std::string suid, names;
            std::getline(iss, suid, '|');
            std::getline(iss, names);
            Client* c = findClientById(std::stoul(suid));
            if (c) c->loadWishlist(names);
        }
        else if (tag == "DISCOUNT") {
            std::string suid, discData;
            std::getline(iss, suid, '|');
            std::getline(iss, discData);
            Client* c = findClientById(std::stoul(suid));
            if (c) {
                auto d = Discount::deserialize(discData);
                if (d) c->addLoadedDiscount(std::move(d));
            }
        }
        else if (tag == "PURCHASE") {
            try {
                std::string spid, suid, sstate, sfrags;
                std::getline(iss, spid, '|');
                std::getline(iss, suid, '|');
                std::getline(iss, sstate, '|');
                std::getline(iss, sfrags);

                PurchaseRecord rec;
                rec.purchaseId = std::stoul(spid);
                rec.userId = std::stoul(suid);
                rec.state = purchaseStateFromString(sstate);

                std::istringstream fragss(sfrags);
                std::string fid;
                while (std::getline(fragss, fid, ','))
                    if (!fid.empty()) rec.fragIds.push_back(std::stoul(fid));
                purchaseRecords.push_back(rec);
            }
            catch (...) {}
        }
    }

    for (auto& rec : purchaseRecords) {
        Client* c = findClientById(rec.userId);
        if (!c) continue;
        std::vector<Fragrance> frags;
        for (unsigned fid : rec.fragIds) {
            Fragrance* fp = findFragranceById(fid);
            if (fp) frags.push_back(*fp);
        }
        c->getPurchases().emplace_back(
            rec.purchaseId, rec.userId, rec.state, frags);
    }
}

void NotinOOP::save() const {
    std::ofstream f(DB_FILE, std::ios::trunc);
    if (!f.is_open()) { std::cerr << "Cannot write database.\n"; return; }

    for (const auto& fr : catalog) {
        f << "FRAGRANCE|" << fr.serialize() << "\n";
        for (const auto& r : fr.getReviews())
            f << "REVIEW|" << fr.getId() << "|" << r.serialize() << "\n";
    }

    for (const auto& u : users) {
        if (u->isAdmin()) {
            f << "ADMIN|" << u->getId() << "|" << u->getUsername()
                << "|" << u->serialize().substr(u->serialize().find('|', 
                    u->serialize().find('|') + 1) + 1) << "\n";
        }
    }
    f.seekp(0);
    f.close();

    std::ofstream out(DB_FILE, std::ios::trunc);

    for (const auto& fr : catalog) {
        out << "FRAGRANCE|" << fr.serialize() << "\n";
        for (const auto& r : fr.getReviews())
            out << "REVIEW|" << fr.getId() << "|" << r.serialize() << "\n";
    }

    for (const auto& u : users) {
        out << u->serialize() << "\n";
        if (!u->isAdmin()) {
            Client* c = static_cast<Client*>(u.get());
            std::string wl = c->serializeWishlist();
            if (!wl.empty()) out << wl << "\n";
            out << c->serializeDiscounts();

            for (const auto& p : c->getPurchases()) {
                out << "PURCHASE|" << p.getId() << "|" << p.getUserId()
                    << "|" << purchaseStateToString(p.getState()) << "|";
                const auto& frags = p.getFragrances();
                for (size_t i = 0; i < frags.size(); ++i) {
                    if (i > 0) out << ",";
                    out << frags[i].getId();
                }
                out << "\n";
            }
        }
    }
}
