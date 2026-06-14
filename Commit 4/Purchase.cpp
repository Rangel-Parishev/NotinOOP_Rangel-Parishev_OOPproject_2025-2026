#include "Purchase.h"
#include <iostream>
#include <sstream>
#include <numeric>

unsigned Purchase::nextPurchaseId = 1;

Purchase::Purchase(unsigned userId, const std::vector<Fragrance>& items)
    : purchaseId(nextPurchaseId++), fragrances(items), state(PurchaseState::PENDING), userId(userId) {
}

Purchase::Purchase(unsigned id, unsigned userId, PurchaseState state, const std::vector<Fragrance>& items)
    : purchaseId(id), fragrances(items), state(state), userId(userId)
{
    if (id >= nextPurchaseId) nextPurchaseId = id + 1;
}

unsigned Purchase::getId() const { return purchaseId; }
unsigned Purchase::getUserId() const { return userId; }
PurchaseState Purchase::getState() const { return state; }
const std::vector<Fragrance>& Purchase::getFragrances() const { return fragrances; }

void Purchase::setState(PurchaseState s) { state = s; }

double Purchase::getTotal() const {
    double total = 0.0;
    for (const auto& f : fragrances) total += f.getPrice();
    return total;
}

void Purchase::show() const {
    std::cout << "Purchase #" << purchaseId
        << " | User #" << userId
        << " | Status: " << purchaseStateToString(state)
        << " | Total: $" << getTotal() << "\n";
    for (const auto& f : fragrances)
        std::cout << "  - " << f.getName() << " ($" << f.getPrice() << ")\n";
}

std::string Purchase::serializeHeader() const {
    std::ostringstream oss;
    oss << purchaseId << "|" << userId << "|" << purchaseStateToString(state) << "|";
    for (size_t i = 0; i < fragrances.size(); ++i) {
        if (i > 0) oss << ",";
        oss << fragrances[i].getId();
    }
    return oss.str();
}

Purchase Purchase::deserializeHeader(const std::string& line) {
    std::istringstream iss(line);
    std::string token;
    unsigned id, uid;

    std::getline(iss, token, '|'); id = std::stoul(token);
    std::getline(iss, token, '|'); uid = std::stoul(token);
    std::getline(iss, token, '|'); PurchaseState st = purchaseStateFromString(token);
    return Purchase(id, uid, st, {});
}

void Purchase::setNextId(unsigned id) { if (id >= nextPurchaseId) nextPurchaseId = id + 1; }
