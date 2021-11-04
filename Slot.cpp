#include "Slot.hpp"

Slot::Slot(int maxSize): m_maxSize(maxSize) {}

bool Slot::isEmpty() {
    return m_slotStack.empty();
}

int Slot::getMaxProducts() {
    return m_slotStack.size();
}

Product* Slot::seekProduct() {
    return &m_slotStack.front();
}

void Slot::addProduct(Product product) {
    if (m_slotStack.size() < m_maxSize)
        m_slotStack.push_back(product);
}

Product Slot::rotate() {
    Product toDispense = m_slotStack.back();
    m_slotStack.pop_back();
    return toDispense;
}
