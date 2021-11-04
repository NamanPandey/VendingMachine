#pragma once

#include <string>

using namespace std;

class Product {
    public:
        //Brief  : Ctor
        Product(string name, float price);

        //Brief  : Getter for Product price
        //Returns: Price in float
        float getPrice();

        //Brief  : Getter for Product Name
        //Returns: Name in string
        string getName();
    
    private:
        string m_name; //Name of product
        float m_price; //Price of Product
};
