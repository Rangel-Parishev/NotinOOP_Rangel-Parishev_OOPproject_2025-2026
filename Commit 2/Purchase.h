#pragma once
#include <vector>
#include "Fragrance.h"
#include "PurchaseState.h"
using namespace std;

class Purchase
{
	vector<Fragrance> purchase;
	unsigned purchaseId;
	static unsigned nextPurchaseId;
	PurchaseState state = PurchaseState::PENDING;
	unsigned userId;
	
public:
	Purchase(unsigned userId) : purchaseId(++nextPurchaseId), userId(userId) {};
	void changeState(const PurchaseState& state);
	void getPrice() const;
	void show() const;
};