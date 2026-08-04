#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <ctime>

using namespace std;

// ==========================================
//         TRANSACTION STRUCTURE
// ==========================================

struct Transaction
{
    string date;
    string type;
    double amount;
    string details;

    Transaction(string d, string t, double a, string det)
    {
        date = d;
        type = t;
        amount = a;
        details = det;
    }
};

// ==========================================
//           ACCOUNT CLASS
// ==========================================

class Account
{
private:
    string accountNumber;
    double balance;
    vector<Transaction> history;

    // Helper function to get current time safely
    string getCurrentTime()
    {
        time_t now = time(0);
        char timeStr[26];
        ctime_s(timeStr, sizeof(timeStr), &now); // Secure version for Visual Studio
        string dt = string(timeStr);
        if (!dt.empty() && dt.back() == '\n')
        {
            dt.pop_back(); // Remove newline
        }
        return dt;
    }

public:
    Account()
    {
        accountNumber = "";
        balance = 0.0;
    }

    Account(string accNum, double initialBalance)
    {
        accountNumber = accNum;
        balance = initialBalance;
    }

    string getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }

    void deposit(double amount, string details)
    {
        if (amount > 0)
        {
            balance += amount;
            history.push_back(Transaction(getCurrentTime(), "CREDIT", amount, details));
            cout << "\n [SUCCESS] $" << amount << " deposited successfully!" << endl;
        }
        else
        {
            cout << "\n [ERROR] Invalid amount!" << endl;
        }
    }

    bool withdraw(double amount, string details)
    {
        if (amount <= 0)
        {
            cout << "\n [ERROR] Invalid amount!" << endl;
            return false;
        }
        else if (amount > balance)
        {
            cout << "\n [ERROR] Insufficient balance! Your balance is: $" << balance << endl;
            return false;
        }
        else
        {
            balance -= amount;
            history.push_back(Transaction(getCurrentTime(), "DEBIT", amount, details));
            cout << "\n [SUCCESS] $" << amount << " withdrawn successfully!" << endl;
            return true;
        }
    }

    void displayHistory() const
    {
        cout << "\n--------------------------------------------" << endl;
        cout << "        TRANSACTION HISTORY                 " << endl;
        cout << "--------------------------------------------" << endl;

        if (history.empty())
        {
            cout << " No transactions yet." << endl;
        }
        else
        {
            cout << setw(5) << "#"
                << setw(22) << "Date"
                << setw(10) << "Type"
                << setw(12) << "Amount"
                << "Details" << endl;
            cout << string(70, '-') << endl;

            for (size_t i = 0; i < history.size(); i++)
            {
                cout << setw(5) << (i + 1)
                    << setw(22) << history[i].date
                    << setw(10) << history[i].type
                    << "$" << setw(11) << fixed << setprecision(2) << history[i].amount
                    << history[i].details << endl;
            }
        }
        cout << "--------------------------------------------" << endl;
    }
};

// ==========================================
//           CUSTOMER CLASS
// ==========================================

class Customer
{
private:
    string name;
    string pin;
    Account account;

public:
    Customer() {}

    Customer(string custName, string custPin, string accNum)
    {
        name = custName;
        pin = custPin;
        account = Account(accNum, 0.0);
    }

    string getName() const { return name; }
    string getPin() const { return pin; }
    Account& getAccount() { return account; }

    void displayInfo() const
    {
        cout << "\n============================================" << endl;
        cout << "            CUSTOMER PROFILE                " << endl;
        cout << "============================================" << endl;
        cout << " Name:           " << name << endl;
        cout << " Account Number: " << account.getAccountNumber() << endl;
        cout << " Balance:        $" << fixed << setprecision(2) << account.getBalance() << endl;
        cout << "============================================" << endl;
    }
};

// ==========================================
//         BANKING SYSTEM MANAGER
// ==========================================

class BankingSystem
{
private:
    vector<Customer> customers;
    const string DATA_FILE = "bank_data.txt";

    string generateAccountNumber()
    {
        string accNum = "ACC-";
        for (int i = 0; i < 6; i++)
        {
            accNum += to_string(rand() % 10);
        }
        return accNum;
    }

    void logToFile(string message)
    {
        ofstream file(DATA_FILE, ios::app);
        if (file.is_open())
        {
            time_t now = time(0);
            char timeStr[26];
            ctime_s(timeStr, sizeof(timeStr), &now); // Secure version
            string dt = string(timeStr);
            if (!dt.empty() && dt.back() == '\n') dt.pop_back();

            file << "[" << dt << "] " << message << endl;
            file.close();
        }
    }

public:
    BankingSystem()
    {
        // Cast to unsigned int to fix the time_t conversion warning
        srand(static_cast<unsigned int>(time(0)));
    }

    void showMenu()
    {
        cout << "\n============================================" << endl;
        cout << "         BANKING MANAGEMENT SYSTEM          " << endl;
        cout << "============================================" << endl;
        cout << "  1. Create New Account" << endl;
        cout << "  2. Login to Account" << endl;
        cout << "  3. Exit" << endl;
        cout << "============================================" << endl;
    }

    void createAccount()
    {
        string name, pin;

        cout << "\n--------------------------------------------" << endl;
        cout << "          CREATE NEW ACCOUNT                " << endl;
        cout << "--------------------------------------------" << endl;

        cout << " Enter Full Name: ";
        cin.ignore();
        getline(cin, name);

        cout << " Create a 4-digit PIN: ";
        cin >> pin;

        while (pin.length() != 4)
        {
            cout << " [ERROR] PIN must be exactly 4 digits. Try again: ";
            cin >> pin;
        }

        string newAccNum = generateAccountNumber();
        Customer newCustomer(name, pin, newAccNum);

        customers.push_back(newCustomer);

        cout << "\n============================================" << endl;
        cout << " [SUCCESS] Account Created Successfully!     " << endl;
        cout << "============================================" << endl;
        cout << " Your Account Number is: " << newAccNum << endl;
        cout << " Please remember your PIN: " << pin << endl;
        cout << "============================================" << endl;

        logToFile("New account created: " + newAccNum + " for user " + name);
    }

    Customer* login()
    {
        string accNum, pin;

        cout << "\n--------------------------------------------" << endl;
        cout << "                 LOGIN                       " << endl;
        cout << "--------------------------------------------" << endl;

        cout << " Enter Account Number: ";
        cin >> accNum;
        cout << " Enter PIN: ";
        cin >> pin;

        for (size_t i = 0; i < customers.size(); i++)
        {
            if (customers[i].getAccount().getAccountNumber() == accNum && customers[i].getPin() == pin)
            {
                cout << "\n [SUCCESS] Login Successful! Welcome, " << customers[i].getName() << "." << endl;
                logToFile("User logged in: " + accNum);
                return &customers[i];
            }
        }

        cout << "\n [ERROR] Invalid Account Number or PIN!" << endl;
        return nullptr;
    }

    void transferFunds(Customer* sender)
    {
        string targetAccNum;
        double amount;

        cout << "\n--------------------------------------------" << endl;
        cout << "            FUND TRANSFER                    " << endl;
        cout << "--------------------------------------------" << endl;
        cout << " Your Balance: $" << sender->getAccount().getBalance() << endl;
        cout << "--------------------------------------------" << endl;

        cout << " Enter recipient Account Number: ";
        cin >> targetAccNum;
        cout << " Enter amount to transfer: $";

        while (!(cin >> amount) || amount <= 0)
        {
            cout << " [ERROR] Invalid amount. Enter again: $";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        for (size_t i = 0; i < customers.size(); i++)
        {
            if (customers[i].getAccount().getAccountNumber() == targetAccNum)
            {
                if (sender->getAccount().getBalance() >= amount)
                {
                    sender->getAccount().withdraw(amount, "Transfer to " + targetAccNum);
                    customers[i].getAccount().deposit(amount, "Transfer from " + sender->getAccount().getAccountNumber());

                    logToFile("Transfer of $" + to_string(amount) + " from " + sender->getAccount().getAccountNumber() + " to " + targetAccNum);
                    return;
                }
                else
                {
                    cout << "\n [ERROR] Insufficient funds for transfer!" << endl;
                    return;
                }
            }
        }

        cout << "\n [ERROR] Recipient account not found!" << endl;
    }
};

// ==========================================
//              MAIN FUNCTION
// ==========================================

int main()
{
    BankingSystem bank;
    int mainChoice;
    int accChoice;

    while (true)
    {
        bank.showMenu();
        cout << "\n Enter choice (1-3): ";

        while (!(cin >> mainChoice) || mainChoice < 1 || mainChoice > 3)
        {
            cout << " [ERROR] Invalid choice. Enter 1, 2, or 3: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (mainChoice == 1)
        {
            bank.createAccount();
        }
        else if (mainChoice == 2)
        {
            Customer* loggedInCustomer = bank.login();

            if (loggedInCustomer != nullptr)
            {
                while (true)
                {
                    cout << "\n--------------------------------------------" << endl;
                    cout << "          ACCOUNT DASHBOARD                 " << endl;
                    cout << "--------------------------------------------" << endl;
                    cout << "  1. Check Balance" << endl;
                    cout << "  2. Deposit Money" << endl;
                    cout << "  3. Withdraw Money" << endl;
                    cout << "  4. Transfer Funds" << endl;
                    cout << "  5. View Transaction History" << endl;
                    cout << "  6. View Profile" << endl;
                    cout << "  7. Logout" << endl;
                    cout << "--------------------------------------------" << endl;
                    cout << " Enter choice (1-7): ";

                    while (!(cin >> accChoice) || accChoice < 1 || accChoice > 7)
                    {
                        cout << " [ERROR] Invalid choice. Enter 1-7: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }

                    if (accChoice == 1)
                    {
                        cout << "\n Current Balance: $" << fixed << setprecision(2) << loggedInCustomer->getAccount().getBalance() << endl;
                    }
                    else if (accChoice == 2)
                    {
                        double depAmount;
                        cout << "\n Enter deposit amount: $";
                        while (!(cin >> depAmount) || depAmount <= 0)
                        {
                            cout << " [ERROR] Invalid amount. Enter again: $";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                        loggedInCustomer->getAccount().deposit(depAmount, "Cash Deposit");
                    }
                    else if (accChoice == 3)
                    {
                        double withAmount;
                        cout << "\n Enter withdrawal amount: $";
                        while (!(cin >> withAmount) || withAmount <= 0)
                        {
                            cout << " [ERROR] Invalid amount. Enter again: $";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                        loggedInCustomer->getAccount().withdraw(withAmount, "ATM Withdrawal");
                    }
                    else if (accChoice == 4)
                    {
                        bank.transferFunds(loggedInCustomer);
                    }
                    else if (accChoice == 5)
                    {
                        loggedInCustomer->getAccount().displayHistory();
                    }
                    else if (accChoice == 6)
                    {
                        loggedInCustomer->displayInfo();
                    }
                    else if (accChoice == 7)
                    {
                        cout << "\n [INFO] Logged out successfully." << endl;
                        break;
                    }
                }
            }
        }
        else if (mainChoice == 3)
        {
            cout << "\n============================================" << endl;
            cout << "      Thank you for using our bank!       " << endl;
            cout << "              Goodbye!                    " << endl;
            cout << "============================================" << endl;
            break;
        }
    }

    return 0;
}
