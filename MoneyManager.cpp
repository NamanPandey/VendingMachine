#include <vector>

#include "Utils.cpp"

using namespace std;

map<float, long> MoneyManager::getCollections() {
    return this->m_collections;
}

MoneyManager::MoneyManager() {}

float MoneyManager::getTotalRevenue() {
    return this->m_totalAmount;
}

ReturnCode MoneyManager::initiateTransaction(Transaction** latestTransaction) {
    if (m_transactions.size() && m_transactions.back().status == PaymentStatus::OPEN ) {
        *latestTransaction = &m_transactions.back();
    } else {
        *latestTransaction = new Transaction();
        if (*latestTransaction == nullptr) {
            cout << "DEBUG:: latestTransaction is null" << endl;
            return ReturnCode::FAILURE;
        }
        m_transactions.push_back(**latestTransaction);
    }
    return ReturnCode::SUCCESS;
}

Transaction* MoneyManager::getLatestTransaction() {
    return &(this->m_transactions.back());
}

ReturnCode MoneyManager::collect(float collectionAmount) {
    cout << endl;
    cout << "Total Amount to be paid: Rs. " << collectionAmount << endl;

    float collected = 0.0f;
    Transaction* latestTransaction = this->getLatestTransaction();
    latestTransaction->denominations.clear();
    float monies;
    while(collected < collectionAmount) {
        cin >> monies;
        latestTransaction->denominations.push_back(monies);
        collected += monies;
        if (collected < collectionAmount)
            cout << "Collected: Rs. " << collected << " | Balance: Rs. " << collectionAmount - collected << endl;
        
        auto itr = m_collections.find(monies);
        if(itr != m_collections.end()) {
            itr->second++;
        } else {
            m_collections.insert(pair<float, long>(monies, 1));
        }
    }

    int refundAmount = collected - collectionAmount;
    if (refundAmount > 0) {
        if (! this->initiateRefundBalance(refundAmount)) {
            return ReturnCode::FAILURE;
        }
    }

    this->m_totalAmount += collectionAmount;
    return ReturnCode::SUCCESS;
}

bool MoneyManager::initiateRefundBalance(float refundAmount) {
    // Objective of the function is to initiate a refund of change and if not possible,
    // Return the originally collected denominations.
    vector<float> refundDenominations;
    bool success = true;

    // Calculate the best way to return change
    refundDenominations = calcRefundDenominations(refundAmount, &success);

    cout << "================== REFUND CHANGE ==================" << endl;
    if (refundDenominations.size() > 0) {
        cout << "Here is your change: " ;
        for(auto change : refundDenominations) {
            m_collections[change]--;
            cout << change << " ";
        }
        cout << endl;
        cout << "===================================================" << endl;
        return true;
    } else {
        cout << "Unable to process balance refund due to insufficient denominations" << endl;
        for (auto change : getLatestTransaction()->denominations) {
            m_collections[change]--;
            cout << change << " ";
        }
        cout << endl;
        cout << "===================================================" << endl;
        return false;
    }
}

vector<float> MoneyManager::calcRefundDenominations(float amount, bool* success) {
    vector<float> denoms, temp;

    // Base Case. If we have a denomination of exact value, return it
    auto itr = m_collections.find(amount);
    if (itr != m_collections.end() && itr->second > 0) {
        denoms.push_back(itr->first);
        *success = true;
        return denoms;
    }

    // Check how can we make up exact change with the denominations we have
    // This is a recursive solution and will use memoization for optimization.
    bool localSuccess;
    for(auto itr = m_collections.begin(); itr != m_collections.end(); itr++) {
        temp.clear();
        if (itr->second > 0 && itr->first < amount) {
            itr->second--;
            temp = calcRefundDenominations(amount - itr->first, &localSuccess);
            if (localSuccess) {
                temp.push_back(itr->first);
            
                if (denoms.size() == 0 || ((denoms.size() != 0) && temp.size() < denoms.size()))
                    denoms = temp;
            }
            itr->second++;
        }
    }

    if (denoms.size() == 0)
        *success = false;
    else
        *success = true;
    
    return denoms;
}