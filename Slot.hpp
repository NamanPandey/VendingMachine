#pragma once

#include <vector>

#include "Product.hpp"

using namespace std;

class Slot {
    public:
        //Brief  : Ctor
        Slot(int);

        //Brief  : Is Slot empty
        //Returns: Boolean
        bool isEmpty();

        //Brief  : Max number of products in slots
        //Returns: Integer
        int getMaxProducts();
        
        //Brief  : Add Product to slot
        //Returns: None
        void addProduct(Product product);
        
        //Brief  : Check front most product in the slot
        //Returns: Product pointer
        Product* seekProduct();

        //Brief  : Rotate the slot to dispense the product.
        //         Eliminate the product from the slot as well.
        //Returns: Product
        Product rotate();       

    private:
        const int m_maxSize;         //Max size of Slots
        vector<Product> m_slotStack; //Slot Vector to hold products
};