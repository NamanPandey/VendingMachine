#include <iostream>
#include <vector>
#include <string>

#ifdef linux
#include <unistd.h>
#elif _WIN32
#include <windows.h>
#endif

#include "Machine.hpp"
#include "Utils.cpp"
#include "MoneyManager.cpp"
#include "Slot.cpp"
#include "Product.cpp"

using namespace std;

Machine* Machine::m_instance = nullptr;

Machine* Machine::createMachine(int slotWidth, int slotHeight) {
    if (m_instance == nullptr) {
        m_instance = new Machine(slotWidth, slotHeight);
    }
    return m_instance; 
}

float Machine::getTotalRevenue() {
    return m_collector->getTotalRevenue();
}

Machine::Machine(int slotWidth, int slotHeight): 
    m_slotWidth(slotWidth), m_slotHeight(slotHeight) {
        cout << "Creating a Machine with Width: " << m_slotWidth << " | Height: " << m_slotHeight << endl;
        for(int idx = 0; idx < slotHeight*slotWidth; idx++) {
            m_slots.push_back(Slot(10));
        }
        m_collector = new MoneyManager();
}

void Machine::display() {
    for(int i=0; i < 100; i++)
        cout << endl;

    cout << "=====================================================================" << endl;
    cout << "                    Welcome to Vending Machine                       " << endl;
    cout << "=====================================================================" << endl;

    for(int h = 0; h < m_slotHeight; h++) {
        for(int w = 0; w < m_slotWidth; w++) {
            int index = h*m_slotWidth + w;
            Slot* currentSlot = &m_slots[index];
            
            string productName = "...";
            float productPrice = 0.0f;
            if(!currentSlot->isEmpty()) {
                productName = currentSlot->seekProduct()->getName();
                productPrice = currentSlot->seekProduct()->getPrice();
            }

            cout << "    " << index+1 << ". " << productName << "(Rs." << productPrice << ")";
        }
        cout << endl;
    }
}

void Machine::addProduct(int slot, Product product) {
    m_slots[slot].addProduct(product);
}

ReturnCode Machine::initiateTransaction() {
    return m_collector->initiateTransaction(&m_latestTransaction);
}

ReturnCode Machine::chooseSlot() {
    if (m_latestTransaction != nullptr && m_latestTransaction->status == PaymentStatus::OPEN) {
        int maxSlots = m_slotWidth*m_slotHeight;
        int slotId;
        cout << ">> Which product would you like(1-" << maxSlots << "): ";
        cin >> slotId;

        if(slotId < 1 || slotId > maxSlots || m_slots[slotId-1].getMaxProducts() == 0) {
            cout << ">> Invalid Entry! Please try again..." << endl;
            m_latestTransaction->slot = nullptr;
            sleep(3);
            return ReturnCode::FAILURE;
        }
        m_latestTransaction->slot = &m_slots[slotId-1]; 
        return ReturnCode::SUCCESS;
    }

    cout << "DEBUG: Latest Transaction is not in open state" << endl;
    return ReturnCode::FAILURE;
}

ReturnCode Machine::chooseQuantity() {
    if (m_latestTransaction != nullptr && m_latestTransaction->status == PaymentStatus::OPEN) {
        int maxQty = m_latestTransaction->slot->getMaxProducts();
        cout << ">> How many would you like: (1";
        if (maxQty > 1)
            cout << "-" << maxQty;
        cout << "): ";
        cin >> m_latestTransaction->quantity;

        if(m_latestTransaction->quantity < 1 || m_latestTransaction->quantity > maxQty) {
            cout << ">> Invalid Entry! Please try again..." << endl;
            m_latestTransaction->quantity = -1;
            sleep(3);
            return ReturnCode::FAILURE;
        }
        
        return ReturnCode::SUCCESS;
    }

    cout << "DEBUG: Latest Transaction is not in open state" << endl;
    return ReturnCode::FAILURE;
}

ReturnCode Machine::collectAmount() {
    if (m_latestTransaction != nullptr && m_latestTransaction->status == PaymentStatus::OPEN && 
        m_latestTransaction->quantity > 0 && 
        m_latestTransaction->slot != nullptr) {
            m_latestTransaction->price = 
                m_latestTransaction->slot->seekProduct()->getPrice() * m_latestTransaction->quantity;
            if (m_collector->collect(m_latestTransaction->price)) {
                m_latestTransaction->status = PaymentStatus::COMPLETED;
                return ReturnCode::SUCCESS;
            }
            return ReturnCode::FAILURE;
    } else {
        cout << "DEBUG: Transaction is not in open state" << endl;
        return ReturnCode::FAILURE;
    }
}

ReturnCode Machine::dispenseProduct(vector<Product>* productBin) {
    if (m_latestTransaction != nullptr && m_latestTransaction->status == PaymentStatus::COMPLETED) {
        for (int q = 0; q < m_latestTransaction->quantity; q++)
            productBin->push_back(m_latestTransaction->slot->rotate());
        return productBin != nullptr ? ReturnCode::SUCCESS : ReturnCode::FAILURE;
    }

    cout << "DEBUG: Transaction is not compelete." << endl;
    return ReturnCode::FAILURE;
}