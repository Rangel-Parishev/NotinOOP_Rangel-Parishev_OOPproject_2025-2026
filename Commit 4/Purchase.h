#pragma once
#include <vector>
#include <string>
#include "Fragrance.h"
#include "PurchaseState.h"

class Purchase {
    unsigned purchaseId;
    static unsigned nextPurchaseId;

    std::vector<Fragrance> fragrances;
    PurchaseState state;
    unsigned userId;

public:
    Purchase(unsigned userId, const std::vector<Fragrance>& items);

    Purchase(unsigned id, unsigned userId, PurchaseState state, const std::vector<Fragrance>& items);

    unsigned getId() const;
    unsigned getUserId() const;
    PurchaseState getState() const;
    const std::vector<Fragrance>& getFragrances() const;

    void setState(PurchaseState s);

    double getTotal() const;
    void show() const;

    std::string serializeHeader() const;
    static Purchase deserializeHeader(const std::string& line);

    static void setNextId(unsigned id);
};
