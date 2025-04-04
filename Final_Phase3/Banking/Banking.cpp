// Banking.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
using namespace std;

//transaction node for recording
struct TransactionNode {
    string type;
    double amount;
    TransactionNode* next;

    TransactionNode(string t, double a)
        : type(t), amount(a), next(nullptr) {
    }
};


class Account {
protected:
    string accountAddress;
    string accountHolder;
    string accountPassword;
    double balance;
    TransactionNode* transactionHead; // Linked list head

    //special classes for later
    friend class InterestManager;
    friend class OverdraftManager;

public:
    // Constructor
    Account(string number, string holder, double initialBalance, string password)
        : accountAddress(number), accountHolder(holder), balance(initialBalance),
        accountPassword(password), transactionHead(nullptr) {
    }

    // Add transaction to linked list
    void addTransaction(string type, double amount) {
        TransactionNode* newNode = new TransactionNode(type, amount);
        newNode->next = transactionHead;
        transactionHead = newNode;
    }

    // Deposit using operator overloading
    virtual Account& operator+=(double amount) {
        if (amount > 0) {
            balance += amount;
            addTransaction("Deposit", amount);
        }
        return *this;
    }

    // Withdraw using operator overloading
    virtual Account& operator-=(double amount) {
        if (amount > 0 && balance >= amount) {
            balance -= amount;
            addTransaction("Withdrawal", amount);
        }
        else {
            cout << "Insufficient funds!\n";
        }
        return *this;
    }

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

    virtual void display() const {
        cout << "\n--- Account Details ---\n";
        cout << "Account Address: " << accountAddress << "\n";
        cout << "Account Holder: " << accountHolder << "\n";
        cout << "Balance: $" << balance << "\n";
    }

    virtual void showTransactions() const {
        cout << "\n--- Transaction History ---\n";
        TransactionNode* current = transactionHead;
        if (!current) {
            cout << "No transactions available.\n";
            return;
        }
        while (current) {
            cout << "Transaction Type: " << current->type
                << ", Amount: $" << current->amount << "\n";
            current = current->next;
        }
    }

    virtual ~Account() {
        // Free linked list memory
        TransactionNode* current = transactionHead;
        while (current) {
            TransactionNode* temp = current;
            current = current->next;
            delete temp;
        }
    }

    // Delete the first matching transaction
    bool deleteTransaction(string type, double amount) {
        TransactionNode* current = transactionHead;
        TransactionNode* prev = nullptr;

        while (current) {
            if (current->type == type && current->amount == amount) {
                if (prev) {
                    prev->next = current->next;
                }
                else {
                    transactionHead = current->next;
                }
                delete current;
                cout << "Transaction deleted successfully.\n";
                return true;
            }
            prev = current;
            current = current->next;
        }
        cout << "Transaction not found.\n";
        return false;
    }

    // Update the first matching transaction
    bool updateTransaction(string oldType, double oldAmount, string newType, double newAmount) {
        TransactionNode* current = transactionHead;

        while (current) {
            if (current->type == oldType && current->amount == oldAmount) {
                current->type = newType;
                current->amount = newAmount;
                cout << "Transaction updated successfully.\n";
                return true;
            }
            current = current->next;
        }
        cout << "Transaction not found.\n";
        return false;
    }

    // Search for a transaction
    bool searchTransaction(string type, double amount) {
        TransactionNode* current = transactionHead;

        while (current) {
            if (current->type == type && current->amount == amount) {
                cout << "Transaction found: " << type << ", $" << amount << "\n";
                return true;
            }
            current = current->next;
        }
        cout << "Transaction not found.\n";
        return false;
    }

};


// Derived class: CheckingAccount
class CheckingAccount : public Account {
private:
    double overdraftLimit;
    //special class
    friend class OverdraftManager;

public:
    CheckingAccount(string number, string holder, double initialBalance, string password, double overdraft)
        : Account(number, holder, initialBalance, password), overdraftLimit(overdraft) {
    }

    CheckingAccount& operator-=(double amount) {
        if (amount > (balance + overdraftLimit)) {
            cout << "Overdraft limit exceeded!\n";
        }
        else {
            balance -= amount;
            addTransaction("Overdraft Withdrawal", amount);
            cout << "Withdrawn from Checking Account: $" << amount << "\n";
        }
        return *this;
    }
};


// Derived class: SavingsAccount
class SavingsAccount : public Account {
private:
    double interestRate;
    //special class
    friend class InterestManager;

public:
    SavingsAccount(string accNumber, string holder, double initialBalance, string password, double rate)
        : Account(accNumber, holder, initialBalance, password), interestRate(rate) {
    }

    void applyInterest() {
        double interest = (balance * interestRate) / 100;
        balance += interest;
        addTransaction("Interest Applied", interest);
        cout << "Interest applied: $" << interest << "\n";
    }
};

//deals with overdraft limit
class OverdraftManager {
public:
    //sets how much you can overdraft
    static void setOverdraftLimit(CheckingAccount& account, double newLimit) {
        account.overdraftLimit = newLimit;
        cout << "Overdraft limit updated to $" << newLimit << "\n";
    }
    //occurs when overdrafting
    static void overdraftWithdraw(CheckingAccount& account, double amount) {
        if (amount <= account.balance + account.overdraftLimit) {
            account.balance -= amount;
            account.addTransaction("Overdraft Withdraw (Friend)", amount);
            cout << "[Friend] Withdrawn with overdraft: $" << amount << "\n";
        }
        else {
            cout << "[Friend] Overdraft limit exceeded!\n";
        }
    }
};

//manages interest rate for savings account
class InterestManager {
public:
    static void applyInterest(SavingsAccount& account) {
        double interest = (account.balance * account.interestRate) / 100;
        account.balance += interest;
        account.addTransaction("Interest Applied (Friend)", interest);
        cout << "[Friend] Interest Applied: $" << interest << "\n";
    }
};


//linked list for customer info
struct CustomerNode {
    string name;
    string id;
    Account* account;
    CustomerNode* next;

    CustomerNode(string name, string id, Account* acc)
        : name(name), id(id), account(acc), next(nullptr) {
    }
};

class CustomerList {
private:
    CustomerNode* head;

public:
    CustomerList() : head(nullptr) {}
    //creates a new customer account
    void addCustomer(string name, string id, Account* account) {
        CustomerNode* newNode = new CustomerNode(name, id, account);
        newNode->next = head;
        head = newNode;
    }
    //used to switch accounts
    CustomerNode* findCustomer(string id) {
        CustomerNode* current = head;
        while (current) {
            if (current->id == id) return current;
            current = current->next;
        }
        return nullptr;
    }
    //this method displays every customer added so far
    void displayAllCustomers() {
        CustomerNode* current = head;
        while (current) {
            cout << "\nCustomer: " << current->name << " (ID: " << current->id << ")\n";
            current->account->display();
            current = current->next;
        }
    }

    ~CustomerList() {
        CustomerNode* current = head;
        while (current) {
            CustomerNode* next = current->next;
            delete current->account;
            delete current;
            current = next;
        }
    }

};



int main() {
    CustomerList bank;

    // Add checking and savings accounts to customer list
    bank.addCustomer("Michael M Hull", "CHK001", new CheckingAccount("CHK789", "Michael M Hull", 500, "M0N3Y", 200));
    bank.addCustomer("Michael M Hull", "SAV001", new SavingsAccount("SAV456", "Michael M Hull", 2000, "M0N3Y", 5.0));
    bank.addCustomer("Duke Nukem", "CHK002", new CheckingAccount("CHK456", "Duke Nukem", 600, "C001Dude", 200));
    bank.addCustomer("Duke Nukem", "SAV002", new SavingsAccount("SAV123", "Duke Nukem", 2400, "C001Dude", 5.0));
    string customerId = "CHK001"; // Example for login
    CustomerNode* currentCustomer = bank.findCustomer(customerId);

    if (!currentCustomer) {
        cout << "Customer not found.\n";
        return 1;
    }
 
    currentCustomer->account->authorize();

    int select;
    double money;

    do {
        cout << "\n==============================\n";
        cout << "What would you like to do?\n";
        cout << "1. Deposit\n";
        cout << "2. Withdraw\n";
        cout << "3. Show Transactions\n";
        cout << "4. Display Account Info\n";
        cout << "5. Switch Account (by ID)\n";
        cout << "6. Delete Transaction\n";
        cout << "7. Update Transaction\n";
        cout << "8. Search Transaction\n";
        cout << "9. Apply Interest (Friend Class)\n";
        cout << "10. Overdraft Withdraw (Friend Class)\n";
        cout << "11. Set Overdraft Limit\n";

        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> select;

        switch (select) {
        case 1:
            cout << "Enter deposit amount: $";
            cin >> money;
            *currentCustomer->account += money;
            break;
        case 2:
            cout << "Enter withdrawal amount: $";
            cin >> money;
            *currentCustomer->account -= money;
            break;
        case 3:
            currentCustomer->account->showTransactions();
            break;
        case 4:
            currentCustomer->account->display();
            break;
        case 5: {
            cout << "Enter new account ID: ";
            string newId;
            cin >> newId;
            CustomerNode* newCust = bank.findCustomer(newId);
            if (newCust) {
                //use the first account number for this,
                //ex: using CHK001 will get you to Michael's checking account
                currentCustomer = newCust;
                cout << "Switched to customer " << newCust->name << "\n";
                newCust->account->authorize();
            }
            else {
                cout << "Account not found.\n";
            }
            break;
        }
        case 6: {
            string type;
            double amount;
            cout << "Enter transaction type to delete: ";
            cin >> type;
            cout << "Enter amount: ";
            cin >> amount;
            currentCustomer->account->deleteTransaction(type, amount);
            break;
        }
        case 7: {
            string oldType, newType;
            double oldAmount, newAmount;
            cout << "Enter old transaction type: ";
            cin >> oldType;
            cout << "Enter old amount: ";
            cin >> oldAmount;
            cout << "Enter new transaction type: ";
            cin >> newType;
            cout << "Enter new amount: ";
            cin >> newAmount;
            currentCustomer->account->updateTransaction(oldType, oldAmount, newType, newAmount);
            break;
        }
        case 8: {
            string type;
            double amount;
            cout << "Enter transaction type to search: ";
            cin >> type;
            cout << "Enter amount: ";
            cin >> amount;
            currentCustomer->account->searchTransaction(type, amount);
            break;
        }
        case 9: {
            //used dynamic cast since its inherited
            SavingsAccount* sav = dynamic_cast<SavingsAccount*>(currentCustomer->account);
            if (sav) {
                InterestManager::applyInterest(*sav);
            }
            else {
                cout << "Not a savings account.\n";
            }
            break;
        }
        case 10: {
            //used dynamic cast here since its inherited
            CheckingAccount* chk = dynamic_cast<CheckingAccount*>(currentCustomer->account);
            if (chk) {
                double amt;
                cout << "Enter overdraft withdrawal amount: ";
                cin >> amt;
                OverdraftManager::overdraftWithdraw(*chk, amt);
            }
            else {
                cout << "Not a checking account.\n";
            }
            break;
        }
        case 11: {
            CheckingAccount* chk = dynamic_cast<CheckingAccount*>(currentCustomer->account);
            if (chk) {
                double limit;
                cout << "Enter new overdraft limit: ";
                cin >> limit;
                OverdraftManager::setOverdraftLimit(*chk, limit);
            }
            else {
                cout << "Not a checking account.\n";
            }
            break;
        }

        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }

    } while (select != 0);
    //displays every account at the end
    //just to confirm everything
    bank.displayAllCustomers();
    return 0;
}






// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
