#include <map>
#include <vector>
#include <string>

#include "Machine.cpp"

#define MAX_RETRY 3

using namespace std;

ReturnCode retry(Machine* myMachine, ReturnCode (Machine::*foo)()) {
    ReturnCode status;
    int retry = 0;
    while (true) {
        retry++;
        status = (myMachine->*foo)();
        if (status == ReturnCode::SUCCESS) {
            return ReturnCode::SUCCESS;
        } else if ( retry == MAX_RETRY ) {
            cout << "ERROR: Max retries limit exceeded. Reverting Transaction." << endl;
            sleep(3);
            return ReturnCode::FAILURE;
        }
    }
}

int main() {
    Machine* myMachine = Machine::createMachine(4,5);
    myMachine->display();

    // Fill Products
    vector<map<string, float>> products = {
        {{"Blue Lays", 10.0f}}, {{"Pringles", 10.0f}}, {{"Green Lays", 10.0f}},
        {{"M&M", 10.0f}}, {{"Kitkat", 10.0f}}, {{"Gems", 10.0f}},
        {{"Wafers", 10.0f}}, {{"Mars", 10.0f}}, {{"5 Star", 10.0f}},
    };
    for (int i = 0; i < 20; i++){
        if((float)rand()/RAND_MAX > 0.5) {
            cout << "Adding Product to " << i << endl;
            map<string, float> productMeta = *(products.begin() + rand()%9);
            for(int q = 0; q < rand()%10; q++)
                myMachine->addProduct(i, Product(productMeta.begin()->first, productMeta.begin()->second));
        }
    }

    // Run Loop to dispense product
    int retryRemaining = 3;
    Product* product;
    ReturnCode status;
    while(1) {
        myMachine->display();
        if (myMachine->initiateTransaction() == ReturnCode::SUCCESS) {
            ReturnCode status = retry(myMachine, &Machine::chooseSlot);
            if (status) {
                ReturnCode status = retry(myMachine, &Machine::chooseQuantity);
                if (status) {
                    ReturnCode status = retry(myMachine, &Machine::collectAmount);
                    if (status) {
                        vector<Product> productBin;
                        ReturnCode status = myMachine->dispenseProduct(&productBin);
                        cout << "================== PRODUCT BIN  =======================" << endl;
                        for (auto p:productBin)
                            cout << p.getName() << " ";
                        cout << endl;
                        cout << "=======================================================" << endl;
                        sleep(3);
                    }
                }
            }
        }
    }

    return 0;
}