#include "Product.hpp"

using namespace std;

Product::Product(string name, float price): m_name(name), m_price(price) {}

float Product::getPrice() { return this->m_price; }

std::string Product::getName()  { return this->m_name;  }
