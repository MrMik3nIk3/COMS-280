// Banking.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>


class Account {
private:
    std::string accountNumber;
    std::string accountHolder;
    double balance;

public:
    Account(std::string number, std::string holder, double initialBalance) :
        accountNumber(number), accountHolder(holder), balance(initialBalance) {
    }

    // Deposit function
    Account& operator+=(double amount) {
        if (amount > 0) {
            balance += amount;
        }
        return *this;
    }

    // Withdraw function
    Account& operator-=(double amount) {
        if (amount > 0 && balance >= amount) {
            balance -= amount;
        }
        return *this;
    }



    //Display Balance
    void display() const {
        std::cout << "Account Number: " << accountNumber << std::endl;
        std::cout << "Account Holder: " << accountHolder << std::endl;
        std::cout << "Balance: " << balance << std::endl;
    }
};

int main() {
    Account myAccount("123456789", "Michael M Hull", 1000);
    myAccount.display();

    myAccount += 500;
    myAccount.display();

    myAccount -= 200;
    myAccount.display();

    return 0;
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
