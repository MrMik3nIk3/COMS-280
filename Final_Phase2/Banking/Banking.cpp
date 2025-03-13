// Banking.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Account {
protected:
    string accountAddress;
    string accountHolder;
    string accountPassword;
    double balance;
    vector<pair<string, double>> transactions; // Store transactions per account

public:
    // Constructor
    Account(string number, string holder, double initialBalance, string password)
        : accountAddress(number), accountHolder(holder), balance(initialBalance), accountPassword(password) {
    }

    // Deposit function using operator overloading
    virtual Account& operator+=(double amount) {
        if (amount > 0) {
            balance += amount;
            transactions.push_back({ "Deposit", amount });
        }
        return *this;
    }

    // Withdraw function using operator overloading
    virtual Account& operator-=(double amount) {
        if (amount > 0 && balance >= amount) {
            balance -= amount;
            transactions.push_back({ "Withdrawal", amount });
        }
        else {
            cout << "Insufficient funds!\n";
        }
        return *this;
    }

    // Authorization function
    void authorize() {
        string check;
        do {
            cout << "\nPlease Enter Password: ";
            cin >> check;
            if (check != accountPassword) {
                cout << "Incorrect Password! Try again.\n";
            }
            else {
                cout << "Welcome, " << accountHolder << "!\n";
            }
        } while (check != accountPassword);
    }

    // Display balance
    virtual void display() const {
        cout << "\n--- Account Details ---\n";
        cout << "Account Address: " << accountAddress << "\n";
        cout << "Account Holder: " << accountHolder << "\n";
        cout << "Balance: $" << balance << "\n";
    }

    // Display transaction history
    virtual void showTransactions() const {
        cout << "\n--- Transaction History ---\n";
        if (transactions.empty()) {
            cout << "No transactions available.\n";
        }
        else {
            for (const auto& t : transactions) {
                cout << "Transaction Type: " << t.first << ", Amount: $" << t.second << "\n";
            }
        }
    }

    virtual ~Account() {}
};

// Derived class: CheckingAccount
class CheckingAccount : public Account {
private:
    double overdraftLimit;

public:
    CheckingAccount(string number, string holder, double initialBalance, string password, double overdraft)
        : Account(number, holder, initialBalance, password), overdraftLimit(overdraft) {
    }

    // Overloaded withdrawal operator with overdraft feature
    CheckingAccount& operator-=(double amount) {
        if (amount > (balance + overdraftLimit)) {
            cout << "Overdraft limit exceeded!\n";
        }
        else {
            balance -= amount;
            transactions.push_back({ "Overdraft Withdrawal", amount });
            cout << "Withdrawn from Checking Account: $" << amount << "\n";
        }
        return *this;
    }
};

// Derived class: SavingsAccount
class SavingsAccount : public Account {
private:
    double interestRate; // in percentage

public:
    SavingsAccount(string accNumber, string holder, double initialBalance, string password, double rate)
        : Account(accNumber, holder, initialBalance, password), interestRate(rate) {
    }

    void applyInterest() {
        double interest = (balance * interestRate) / 100;
        balance += interest;
        transactions.push_back({ "Interest Applied", interest });
        cout << "Interest applied: $" << interest << "\n";
    }
};

int main() {
    Account mainAccount("123456abc", "Michael M Hull", 1000, "M0N3Y");
    CheckingAccount checking("CHK789", "Michael M Hull", 500, "M0N3Y", 200);
    SavingsAccount savings("SAV456", "Michael M Hull", 2000, "M0N3Y", 5.0);

    int select;
    double money;
    mainAccount.authorize();

    do {
        cout << "\n==============================\n";
        cout << "What would you like to do?\n";
        cout << "1. Deposit to Checking Account\n";
        cout << "2. Withdraw from Checking Account\n";
        cout << "3. Deposit to Savings Account\n";
        cout << "4. Withdraw from Savings Account\n";
        cout << "5. Apply Interest to Savings\n";
        cout << "6. Display All Account Balances\n";
        cout << "7. Show Transaction History\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> select;

        switch (select) {
        case 1:
            cout << "Enter deposit amount: $";
            cin >> money;
            checking += money;
            break;
        case 2:
            cout << "Enter withdrawal amount: $";
            cin >> money;
            checking -= money;
            break;
        case 3:
            cout << "Enter deposit amount: $";
            cin >> money;
            savings += money;
            break;
        case 4:
            cout << "Enter withdrawal amount: $";
            cin >> money;
            savings -= money;
            break;
        case 5:
            savings.applyInterest();
            break;
        case 6:

            checking.display();
            savings.display();
            break;
        case 7:
            cout << "\n--- Transaction History for Checking Account ---\n";
            checking.showTransactions();
            cout << "\n--- Transaction History for Savings Account ---\n";
            savings.showTransactions();
            break;
        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }

    } while (select != 0);

    return 0;
}





// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
