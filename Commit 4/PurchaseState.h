#pragma once
#include <string>

enum class PurchaseState {
    PENDING,
    DELIVERED,
    CANCELED
};

inline std::string purchaseStateToString(PurchaseState s) {
    switch (s) {
    case PurchaseState::PENDING:   return "PENDING";
    case PurchaseState::DELIVERED: return "DELIVERED";
    case PurchaseState::CANCELED:  return "CANCELED";
    }
    return "UNKNOWN";
}

inline PurchaseState purchaseStateFromString(const std::string& s) {
    if (s == "DELIVERED") return PurchaseState::DELIVERED;
    if (s == "CANCELED")  return PurchaseState::CANCELED;
    return PurchaseState::PENDING;
}
