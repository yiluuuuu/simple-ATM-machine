#include <iostream>
#include <fstream>
using namespace std;
class Account {
public:
    Account(int accountNumber, string  name, double balance, string  password)
            : accountNumber(accountNumber), name(name), balance(balance), password(password) {}
    Account()=default;
     int getAccountNumber() const { return accountNumber; }
     string getName() const { return name; }
     double getBalance() const { return balance; }
     string getPassword() const { return password; }
    void deposit(double amount) { balance += amount; }
    bool withdraw(double amount) {
        if (balance >= amount) { balance -= amount; return true;}
        else { cout << "\nInsufficient funds.\n" << endl; return false;} }
private:
    int accountNumber{};
    string name;
    double balance{};
    string password;
};
class Bank {
public:
    Bank(const string& accountFile) : accountFile(accountFile),accounts() { loadAccountsFromFile(accountFile); }
    bool customerLogin(const string& username, const string& password) {
        for (int i = 0; i < numCustomers; i++) {
            if (customers[i].first == username && customers[i].second == password) {
                return true;
            }
        }
        return false;
    }
    int findAccountNumber(const string& username){
        int loggedInCustomerAccountNumber = findAccount(username)->getAccountNumber();
        return loggedInCustomerAccountNumber;
    }
    static bool bankerLogin(const string& username, const string& password) {
        if (username == "admin" && password == "admin") {
            return true;
        }
        return false;
    }
    void createAccount(const string& name, double initialBalance, const string& password) {
        int accountNumber = generateUniqueAccountNumber();
        Account account(accountNumber, name, initialBalance, password);
        accounts[numAccounts++] = account;
        customers[numCustomers++] = make_pair(name, password);
        cout << "\nAccount created successfully. Account number: " << accountNumber <<"\n"<<endl;
    }
    void deleteAccount(int accountNumber) {
        for (int i = 0; i < numAccounts; i++) {
            if (accounts[i].getAccountNumber() == accountNumber) {
                accounts[i] = accounts[numAccounts - 1];
                numAccounts--;
                cout << "\nAccount deleted successfully.\n" << endl;
                return;
            }
        }
        cout << "\nAccount not found.\n" << endl;
    }
    void deposit(int accountNumber, double amount) {
        Account* account = findAccount(accountNumber);
        if (account) {
            account->deposit(amount);
            cout << "\nDeposit successful. New balance: " << account->getBalance() << endl;
        } else
            cout << "\nAccount not found.\n" << endl;
    }
    void withdraw(int accountNumber, double amount) {
        Account* account = findAccount(accountNumber);
        if (account) {
            double fee = amount * 0.05;
            amount += fee;
            if (account->withdraw(amount)){
            cout<<"\nSuccess!!"<<endl;
            cout<<"Withdrawal Fee: "<<fee<<" Birr"<<endl;
            cout<<" Balance: " << account->getBalance()<<" Birr"<<endl<<endl;}
       } else cout << "Account not found.\n"<< endl;
    }
    void transfer(int sourceAccountNumber, int destinationAccountNumber, double amount) {
        Account* sourceAccount = findAccount(sourceAccountNumber);
        Account* destinationAccount = findAccount(destinationAccountNumber);
        if (sourceAccount && destinationAccount) {
            if (sourceAccount->getBalance() >= amount) {
                sourceAccount->withdraw(amount);
                destinationAccount->deposit(amount);
                cout << "\nTransfer successful." << endl;
            } else
                cout << "\nInsufficient funds in the source account." << endl;
        } else
            cout << "\nOne or both of the accounts not found." << endl<<endl;
    }
    void saveAccountsToFile(const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            for (int i = 0; i < numAccounts; i++) {
                file << accounts[i].getAccountNumber() << " " << accounts[i].getName() << " " << accounts[i].getBalance() << " " << accounts[i].getPassword() << endl;
            }
            file.close();
        }
    }
    void displayAccountDetails(int accountNumber) {
        Account* account = findAccount(accountNumber);
        if (account) {
            cout <<endl<< "Account Number: " << account->getAccountNumber() << endl;
            cout << "Account Holder Name: " << account->getName() << endl;
            cout << "Account Balance: " << account->getBalance() <<" Birr"<<endl;
        } else cout << "\nAccount not found." << endl<<endl;
    }
private:
    Account accounts[100];
    pair<string, string> customers[100];
    string accountFile;
    int numAccounts = 0;
    int numCustomers = 0;
    int generateUniqueAccountNumber() {
        int uniqueAccountNumber;
        bool isUnique;
        do {
            uniqueAccountNumber = rand() % 90000 + 10000; // Generate a 5-digit number
            isUnique = true;
            for (int i = 0; i < numAccounts; i++) {
                if (accounts[i].getAccountNumber() == uniqueAccountNumber) {
                    isUnique = false;
                    break;
                }
            }
        } while (!isUnique);
        return uniqueAccountNumber;
    }
    Account* findAccount(int accountNumber) {
        for (int i = 0; i < numAccounts; i++) {
            if (accounts[i].getAccountNumber()== accountNumber) {
                return &accounts[i];
            }
        }
        return nullptr;
    }
    Account* findAccount(const string& username) {
        for (int i = 0; i < numAccounts; i++) {
            if (accounts[i].getName() == username) {
                return &accounts[i];
            }
        }
        return nullptr;
    }
    void loadAccountsFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            int accountNumber;
            string name;
            double balance;
            string password;
            while (file >> accountNumber >> name >> balance >> password) {
                Account account(accountNumber, name, balance, password);
                accounts[numAccounts++] = account;
                customers[numCustomers++] = make_pair(name, password);
            }
            file.close();
        }
    }
};
int main() {
    Bank bank("accounts.txt");
    int choice,loggedInCustomerAccountNumber;
    cout << "\n\tWelcome to the IF Bank Management System!\n";
    cout<<"________________________________________________________________"<<endl;
    while (true) {
        cout << "1. Customer Login" << endl;
        cout << "2. Banker Login" << endl;
        cout << "3. Exit\n" << endl;
        cout << "Enter your choice:";
        cin >> choice;
        if (choice == 1) {
            string username, password;
            cout << "Enter your account holder name: ";
            cin >> username;
            cout << "Enter your password: ";
            cin >> password;
            if (bank.customerLogin(username, password)) {
                int customerChoice;
                cout <<endl<< "Customer login successful!!\n" << endl;
                while (true) {
                    cout << "1. Transfer" << endl;
                    cout << "2. Display Account Details" << endl;
                    cout << "3. Logout\n" << endl;
                    cout << "Enter your choice:";
                    cin >> customerChoice;
                    loggedInCustomerAccountNumber=bank.findAccountNumber(username);
                    if (customerChoice == 1) {
                        int destinationAccountNumber;
                        double amount;
                        cout << "Enter destination account number: ";
                        cin >> destinationAccountNumber;
                        cout << "Enter amount to transfer: ";
                        cin >> amount;
                        bank.transfer(loggedInCustomerAccountNumber, destinationAccountNumber, amount);
                        bank.saveAccountsToFile("accounts.txt");
                    } else if (customerChoice == 2) {
                        int accountNumber = loggedInCustomerAccountNumber;
                        bank.displayAccountDetails(accountNumber);
                    } else if (customerChoice == 3) {
                        bank.saveAccountsToFile("accounts.txt");
                        break;
                    } else
                        cout << "\nInvalid choice. Please try again." << endl;
                }
            } else cout << "\nInvalid username or password." << endl;
        } else if (choice == 2) {
            string username, password;
            cout << "Enter your username: ";
            cin >> username;
            cout << "Enter your password: ";
            cin >> password;
            if (Bank::bankerLogin(username, password)) {
                int bankerChoice;
                cout <<endl<< "Banker login successful!!\n" << endl;
                while (true) {
                    cout << "1. Create Account" << endl;
                    cout << "2. Delete Account" << endl;
                    cout << "3. Deposit" << endl;
                    cout << "4. Withdraw" << endl;
                    cout << "5. Transfer" << endl;
                    cout << "6. Display Account Details" << endl;
                    cout << "7. Logout\n" << endl;
                    cout << "Enter your choice: ";
                    cin >> bankerChoice;
                    if (bankerChoice == 1) {
                        string name;
                        double balance;
                        string Password;
                        cout << "Enter account holder name: ";
                        cin >> name;
                        cout << "Enter initial balance: ";
                        cin >> balance;
                        cout << "Enter password: ";
                        cin >> Password;
                        bank.createAccount(name, balance, Password);
                        bank.saveAccountsToFile("accounts.txt");
                    } else if (bankerChoice == 2) {
                        int accountNumber;
                        cout << "Enter account number: ";
                        cin >> accountNumber;
                        bank.deleteAccount(accountNumber);
                        bank.saveAccountsToFile("accounts.txt");
                    } else if (bankerChoice == 3) {
                        int accountNumber;
                        double amount;
                        cout << "Enter account number: ";
                        cin >> accountNumber;
                        cout << "Enter amount to deposit: ";
                        cin >> amount;
                        bank.deposit(accountNumber, amount);
                        bank.saveAccountsToFile("accounts.txt");
                    } else if (bankerChoice ==4) {
                        int accountNumber;
                        double amount;
                        cout << "Enter account number: ";
                        cin >> accountNumber;
                        cout << "Enter amount to withdraw: ";
                        cin >> amount;
                        bank.withdraw(accountNumber, amount);
                        bank.saveAccountsToFile("accounts.txt");
                    } else if (bankerChoice ==5) {
                        int sourceAccountNumber, destinationAccountNumber;
                        double amount;
                        cout << "Enter source account number: ";
                        cin >> sourceAccountNumber;
                        cout << "Enter destination account number: ";
                        cin >> destinationAccountNumber;
                        cout << "Enter amount to transfer: ";
                        cin >> amount;
                        bank.transfer(sourceAccountNumber, destinationAccountNumber, amount);
                        bank.saveAccountsToFile("accounts.txt");
                    } else if (bankerChoice ==6) {
                        int accountNumber;
                        cout << "Enter account number: ";
                        cin >> accountNumber;
                        bank.displayAccountDetails(accountNumber);
                    } else if (bankerChoice ==7) {
                        bank.saveAccountsToFile("accounts.txt");
                        break;
                    } else cout << "\nInvalid choice. Please try again." << endl;
                }
            } else cout << "\nInvalid username or password.\n" << endl;
        } else if (choice == 3) {
            bank.saveAccountsToFile("accounts.txt");
            cout << "\nThank you for using the Bank Management System. Goodbye!\n" << endl;
            break;
        } else cout << "\nInvalid choice. Please try again.\n" << endl;
    }return 0;
}
