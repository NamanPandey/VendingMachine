#pragma once

#include <ctime>
#include "Slot.hpp"

enum PaymentStatus {
    OPEN, COMPLETED, FAILED
};

struct Transaction {
    int id;
    Slot* slot;
    int quantity;
    int price;
    vector<float> denominations;
    PaymentStatus status;

    Transaction() {
        this->id = (int)std::time(0);
        this->status = PaymentStatus::OPEN;
        this->slot = nullptr;
        this->quantity = -1;
        this->price = -1;
    }
};