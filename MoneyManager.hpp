#include <map>
#include <vector>

#include "Utils.hpp"
#include "Transaction.hpp"

using namespace std;

class MoneyManager {
    public:
        //Brief  : Ctor
        MoneyManager();

        //Brief  : Getter for money collection map
        //Returns: m_collections Map
        map<float, long> getCollections();

        //Brief  : Initiate a new transaction, unless one is in open state
        //         Save the pointer to transaction object in latest transaction
        //         Save the object in a member variables
        //Returns: Bool ReturnCode
        ReturnCode initiateTransaction(Transaction** latestTransaction);

        //Brief  : Returns Latest Transactions
        //Returns: Pointer to required transaction object
        Transaction* getLatestTransaction();

        //Brief  : Collect given amount of money
        //Returns: Bool ReturnCode
        ReturnCode collect(float collectionAmount);
        
    private:
        float totalAmount = 0.0f;           //Total revenue collected yet
        map<float, long> m_collections;     //Denominations collection map
        vector<Transaction> m_transactions; //Vector to push transactions

        //Brief  : Refund the said amount if we have enough denominations
        //Returns: Bool, if we have exact change.
        bool initiateRefundBalance(float refundAmount);

        //Brief  : Calculate the change denominations
        //         Algorithm would minimise the number of denominations to return
        //Returns: Vector of denominations to return.
        vector<float> calcRefundDenominations(float amount, bool* success);
};