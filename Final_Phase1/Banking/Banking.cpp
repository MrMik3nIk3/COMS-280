// Banking.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>


class Account {
private:
    std::string accountAddress;
    std::string accountHolder;
    std::string accountPassword;    
    double balance;
    std::vector<std::string>type;
    std::vector<int>change;

public:
    Account(std::string number, std::string holder, double initialBalance, std::string password):
        accountAddress(number), accountHolder(holder), balance(initialBalance), accountPassword(password){
    }
    
    

    // Deposit function, adds to record
    Account& operator+=(double amount) {
        if (amount > 0) {
            balance += amount;
        }
        return *this;
    }

    // Withdraw function, adds to record
    Account& operator-=(double amount) {
        if (amount > 0 && balance >= amount) {
            balance -= amount;
        }
        return *this;
    }

    //Authorize Function
    void authorize() {
        std::string check;
        do
        {
            std::cout << "\nPlease Enter Password: ";
            std::cin >> check;
            if (check != accountPassword)
            {
                std::cout << "\nIncorrect Password" << std::endl;
            }
            else {
                std::cout << "\nWelcome, " << accountHolder << std::endl;
            }
        } while (check != accountPassword);
        
        
    }

    //Display Balance
    void display() const {
        std::cout << "Account Address: " << accountAddress << std::endl;
        std::cout << "Account Holder: " << accountHolder << std::endl;
        std::cout << "Balance: " << balance << std::endl;
    }
};

struct Transaction {
    std::string type;
    double amount;
};

std::vector<Transaction> transactions;

void addTransaction(const std::string& type, double amount) {
    transactions.push_back({ type , amount });
}

Transaction* getPreviousTransactions(int current) {
    if (transactions.empty()) {
        return nullptr; // occurs when no previous transaction
    }
    return &transactions[current];
}


int main() {
    

    Account myAccount("123456abc", "Michael M Hull", 1000, "M0N3Y");
    
    int select;
    double money;
    myAccount.authorize();

    do
    {
        myAccount.display();

        std::cout << "What would you like to do?\n";
        std::cout << "------------------------------\n";
        std::cout << "1.Deposit\n";
        std::cout << "2.Withdraw\n";
        std::cout << "3.Display Transaction History\n";
        std::cout << "enter 0 to close\n";

        std::cin >> select;
    
        switch (select)
        {
        case 1:
            std::cout << "Please Enter Amount: $";
            std::cin >> money;

            myAccount += money;
            addTransaction("Deposit", money);
            break;
        case 2:
            std::cout << "Please Enter Amount: $";
            std::cin >> money;

            myAccount -= money;
            addTransaction("Withdrawl", money);
            break;
        case 3:
            for (size_t i = 0; i < transactions.size(); i++)
            {
                Transaction* prevTransaction = getPreviousTransactions(i);
                if (prevTransaction != nullptr) {
                    std::cout << "Transaction Type: " << prevTransaction->type << ", Amount: " << prevTransaction->amount << std::endl;
                }
                else {
                    std::cout << "No previous transaction found." << std::endl;
                }
            }
            
            break;
        default:
            break;
        }

    } while (select != 0);

    return 0;
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
