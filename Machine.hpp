    #include <vector>
#include <unordered_map>

#include "Slot.hpp"
#include "Transaction.hpp"
#include "MoneyManager.hpp"
#include "Utils.hpp"

using namespace std;

class Machine {
    public:
        // Create a singleton Machine object
        static Machine* createMachine(int slotswidth, int slotsHeight);

        /// Owner side APIs

        // Brief   : Get Total Income Generated.
        // Returns : Total INR float value 
        float getTotalRevenue();

        // Brief  : Add Product to a slot number
        // Returns: None
        void addProduct(int slotNum, Product product);


        /// Client side APIs

        //Brief  : Display The slot machine grid
        //Returns: None
        void display();

        //Brief  : Initiate a Transaction
        //Returns: Bool ReturnCode
        ReturnCode initiateTransaction();

        //Brief  : Request user to choose a slot
        //Returns: Bool ReturnCode
        ReturnCode chooseSlot();

        //Brief  : Request user to choose quantity
        //Returns: Bool ReturnCode
        ReturnCode chooseQuantity();

        //Brief  : Request money from user and dispense change, if required
        //Returns: Bool ReturnCode
        ReturnCode collectAmount();

        //Brief  : Dispense Product from slot to ProductBin
        //         This requires us to pass the address to an 
        //         existing productBin where we add dispensed products 
        //Returns: Bool ReturnCode
        ReturnCode dispenseProduct(vector<Product>* productBin);


    private:
        static Machine* m_instance;         //Machine Object Instance
        int m_slotHeight;                   //No. of slot height
        int m_slotWidth;                    //No. of slot width

        vector<Slot> m_slots;               //Slot Objects
        Transaction* m_latestTransaction;   //Latest Transaction Pointer
        
        MoneyManager* m_collector;          //MoneyManager's Instance

        Machine(int slotswidth, int slotsHeight);
};
