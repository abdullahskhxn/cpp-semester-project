#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <iomanip>
using namespace std;
/*Bank Management System
*/
// ANSI Escape codes for colors
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

// Global variables
string currentUser = "";
double currentBalance = 0.0;

// Function prototypes
void displayMainMenu();
void displayUserMenu(const string& username);
void displayAdminMenu();
void createAccount();
bool login();
void deposit();
void withdraw();
void viewBalance();
void viewTransactionHistory();
void setWithdrawLimit();
void currencyExchange();
void billPayments();
void transfer();
void logout();
void adminLogin();
void adminViewAccounts();
void adminViewTransactionHistory(const string& username);
string generateCreditCardNumber();
void saveCreditCardNumber(const string& username, const string& creditCard);
void recordTransaction(const string& type, double amount);

// Main function
int main() {
    srand(time(0)); // Seed for random number generation

    while (true) {
        displayMainMenu();
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                if (login()) {
                    while (true) {
                        displayUserMenu(currentUser);
                        cout << "Enter your choice: ";
                        cin >> choice;

                        switch (choice) {
                            case 1: deposit(); break;
                            case 2: withdraw(); break;
                            case 3: viewBalance(); break;
                            case 4: viewTransactionHistory(); break;
                            case 5: transfer(); break;
                            case 6: currencyExchange(); break;
                            case 7: billPayments(); break;
                            case 8: logout(); goto mainMenu;
                            default: cout << RED << "Invalid choice. Try again.\n" << RESET;
                        }
                    }
                }
                break;
            case 3:
                adminLogin();
                break;
            case 0:
                cout << BLUE << "Exiting the system. Goodbye!" << RESET << endl;
                return 0;
            default:
                cout << RED << "Invalid choice. Try again.\n" << RESET;
        }
    mainMenu:
        continue;
    }
}

// Function definitions
void displayMainMenu() {
    cout << YELLOW << "\n===== Bank Management System =====\n" << RESET;
    cout <<CYAN<<"1. Create Account\n";
    cout << "2. Login to Account\n";
    cout << "3. Admin Login\n";
    cout << "0. Exit\n"<<RESET;
    cout <<YELLOW<<"==================================\n"<<RESET;
}

void displayUserMenu(const string& username) {
    cout << CYAN << "\n=== Welcome, " << username << "! ===\n" << RESET;
    cout <<MAGENTA<< "1. Deposit\n";
    cout << "2. Withdraw\n";
    cout << "3. View Balance\n";
    cout << "4. View Transaction History\n";
    cout << "5. Transfer to Another Account\n";
    cout << "6. Currency Exchange\n";
    cout << "7. Pay Bills\n";
    cout << "8. Logout\n"<<RESET;
    cout <<CYAN<< "=================================\n"<<RESET;
}

void displayAdminMenu() {
    cout << MAGENTA << "\n=== Admin Menu ===\n" << RESET;
    cout <<CYAN<< "1. View All Accounts and Balances\n";
    cout << "2. View Transaction History of a User\n";
    cout << "3. Logout\n"<<RESET;
    cout <<MAGENTA<< "=====================\n"<<RESET;
}
bool isValidNumber(const string &number, int length) {
    if (number.length() != length) return false;
    for (char c : number) {
        if (!isdigit(c)) return false;
    }
    return true;
}

void createAccount() {
    string username, password, contactNumber, cnicNumber;

    cout << "Enter username: ";
    cin >> username;

    cout << "Enter password: ";
    cin >> password;

    // Check if user file already exists
    ifstream checkFile(username + ".txt");
    if (checkFile.is_open()) {
        cout << RED << "Username already exists. Please choose another username.\n" << RESET;
        checkFile.close();
        return;
    }

    // Get and validate contact number
    while (true) {
        cout << "Enter contact number (11 digits): ";
        cin >> contactNumber;
        if (isValidNumber(contactNumber, 11)) break;
        cout << RED << "Invalid contact number. It must be 11 digits.\n" << RESET;
    }

    // Get and validate CNIC number
    while (true) {
        cout << "Enter CNIC number (13 digits): ";
        cin >> cnicNumber;
        if (isValidNumber(cnicNumber, 13)) break;
        cout << RED << "Invalid CNIC number. It must be 13 digits.\n" << RESET;
    }

    // Generate credit card number
    string creditCardNumber = generateCreditCardNumber();

    // Create user file and save data
    ofstream userFile(username + ".txt");
    if (userFile.is_open()) {
        userFile << "Password: " << password << endl;
        userFile << "Contact Number: " << contactNumber << endl;
        userFile << "CNIC Number: " << cnicNumber << endl;
        userFile << "Credit Card Number: " << creditCardNumber << endl;
        userFile.close();

        // Add username to the usernames_list.txt
        ofstream usersFile("usernames_list.txt", ios::app);
        if (usersFile.is_open()) {
            usersFile << username << endl;
            usersFile.close();
        } else {
            cout << RED << "Error saving username to the list.\n" << RESET;
        }

        // Initialize balance
        ofstream balanceFile(username + "_balance.txt");
        if (balanceFile.is_open()) {
            balanceFile << "0.00" << endl; // Set initial balance to 0.00
            balanceFile.close();
        }

        saveCreditCardNumber(username, creditCardNumber);

        cout << GREEN << "Account created successfully.\nYour Credit Card Number is: " 
             << creditCardNumber << RESET << endl;
    } else {
        cout << RED << "Error creating user file.\n" << RESET;
    }
}

bool login() {
    string username, password, storedPasswordLine, storedPassword;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    ifstream userFile(username + ".txt");
    if (userFile.is_open()) {
        // Read the password line
        getline(userFile, storedPasswordLine);

        // Extract actual password from the line
        size_t pos = storedPasswordLine.find(": ");
        if (pos != string::npos) {
            storedPassword = storedPasswordLine.substr(pos + 2); // Skip "Password: "
        }

        // Compare the entered password with the stored one
        if (storedPassword == password) {
            currentUser = username;

            // Read the balance
            ifstream balanceFile(username + "_balance.txt");
            if (balanceFile.is_open()) {
                balanceFile >> currentBalance;
                balanceFile.close();
            }

            cout << GREEN << "Login successful. Welcome, " << username << "!\n" << RESET;
            return true;
        } else {
            cout << RED << "Incorrect password.\n" << RESET;
        }
    } else {
        cout << RED << "User not found.\n" << RESET;
    }
    return false;
}


void deposit() {
    double amount;
    cout << "Enter amount to deposit: ";
    cin >> amount;
    if (amount > 0) {
        currentBalance += amount;
        recordTransaction("Deposit", amount);

        ofstream balanceFile(currentUser + "_balance.txt");
        balanceFile << currentBalance << endl;
        balanceFile.close();

        cout << GREEN << "Deposit successful. New balance: " << currentBalance << endl << RESET;
    } else {
        cout << RED << "Invalid amount.\n" << RESET;
    }
}

void withdraw() {
    double amount;
    cout << "Enter amount to withdraw: ";
    cin >> amount;
    if (amount > 0 && amount <= currentBalance) {
        currentBalance -= amount;
        recordTransaction("Withdraw", amount);

        ofstream balanceFile(currentUser + "_balance.txt");
        balanceFile << currentBalance << endl;
        balanceFile.close();

        cout << GREEN << "Withdrawal successful. Remaining balance: " << currentBalance << endl << RESET;
    } else {
        cout << RED << "Invalid amount or insufficient balance.\n" << RESET;
    }
}

void viewBalance() {
    cout << "Current balance: " << currentBalance << endl;
}

void viewTransactionHistory() {
    ifstream transactionFile(currentUser + "_transactions.txt");
    if (transactionFile.is_open()) {
        cout << "Transaction History:\n";
        string line;
        while (getline(transactionFile, line)) {
            cout << YELLOW<<line << endl<<RESET;
        }
        transactionFile.close();
    } else {
        cout << "No transaction history available.\n";
    }
}

void transfer() {
    string recipient;
    double amount;
    cout << "Enter recipient username: ";
    cin >> recipient;
    cout << "Enter amount to transfer: ";
    cin >> amount;

    if (amount > 0 && amount <= currentBalance) {
        ifstream recipientFile(recipient + ".txt");
        if (recipientFile.is_open()) {
            recipientFile.close();

            ifstream recipientBalanceFile(recipient + "_balance.txt");
            double recipientBalance;
            recipientBalanceFile >> recipientBalance;
            recipientBalanceFile.close();

            recipientBalance += amount;
            currentBalance -= amount;

            ofstream recipientBalanceFileOut(recipient + "_balance.txt");
            recipientBalanceFileOut << recipientBalance << endl;
            recipientBalanceFileOut.close();

            ofstream userBalanceFile(currentUser + "_balance.txt");
            userBalanceFile << currentBalance << endl;
            userBalanceFile.close();

            recordTransaction("Transfer to " + recipient, amount);
            cout << GREEN << "Transfer successful.\n" << RESET;
        } else {
            cout << RED << "Recipient not found.\n" << RESET;
        }
    } else {
        cout << RED << "Invalid amount or insufficient balance.\n" << RESET;
    }
}

void currencyExchange() {
    double amount;
    int currencyOption;
    cout << "Enter amount in PKR: ";
    cin >> amount;

    if (amount > 0 && amount <= currentBalance) {
        cout <<GREEN<< "Select currency to convert to:\n";
        cout << "1. USD (1 USD = 280 PKR)\n";
        cout << "2. EUR (1 EUR = 300 PKR)\n";
        cout << "3. GBP (1 GBP = 350 PKR)\n"<<RESET;
        cout << "Enter your choice: ";
        cin >> currencyOption;

        double convertedAmount = 0.0;
        string currency;

        switch (currencyOption) {
            case 1:
                convertedAmount = amount / 280.0;
                currency = "USD";
                break;
            case 2:
                convertedAmount = amount / 300.0;
                currency = "EUR";
                break;
            case 3:
                convertedAmount = amount / 350.0;
                currency = "GBP";
                break;
            default:
                cout<<RED << "Invalid currency option.\n"<<RESET;
                return;
        }

        currentBalance -= amount;
        ofstream balanceFile(currentUser + "_balance.txt");
        balanceFile << currentBalance << endl;
        balanceFile.close();

        recordTransaction("Currency Exchange to " + currency, amount);
        cout <<MAGENTA<< "Converted Amount: " << fixed << setprecision(2) << convertedAmount << " " << currency <<RESET<< endl;
        cout <<YELLOW<< "Remaining Balance: " << currentBalance << " PKR\n"<<RESET;
    } else {
        cout<<RED << "Invalid amount or insufficient balance.\n"<<RESET;
    }
}

void billPayments() {
          if (currentUser.empty()) {
        cout << "Please login to pay bills.\n";
        return;
    }

    // File to track paid bills
    string billStatusFile = currentUser + "_bills.txt";

    // Default bill amounts
    const int electricityBill = 5000;
    const int gasBill = 2000;
    const int internetBill = 1000;

    // Track bill payment status
    bool electricityPaid = false, gasPaid = false, internetPaid = false;

    // Read existing bill status if the file exists
    ifstream billStatusIn(billStatusFile);
    if (billStatusIn.is_open()) {
        billStatusIn >> electricityPaid >> gasPaid >> internetPaid;
        billStatusIn.close();
    }

    while (true) {
        cout << "\n=== Pay Bills Menu ===\n";
        cout <<BLUE<< "1. Pay Electricity Bill (" << (electricityPaid ? "Paid" : "Unpaid") << ")\n";
        cout << "2. Pay Gas Bill (" << (gasPaid ? "Paid" : "Unpaid") << ")\n";
        cout << "3. Pay Internet Bill (" << (internetPaid ? "Paid" : "Unpaid") << ")\n";
        cout << "4. Exit Bill Payments\n"<<RESET;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                if (electricityPaid) {
                    cout <<YELLOW<< "Electricity bill already paid.\n"<<RESET;
                } else if (currentBalance >= electricityBill) {
                    currentBalance -= electricityBill;
                    electricityPaid = true;
                    recordTransaction("Electricity Bill Payment", electricityBill);
                    cout <<GREEN<< "Electricity bill of PKR " << electricityBill << " paid successfully.\n"<<RESET;
                } else {
                    cout <<RED<< "Insufficient balance to pay electricity bill.\n"<<RESET;
                }
                break;

            case 2:
                if (gasPaid) {
                    cout <<YELLOW<< "Gas bill already paid.\n"<<RESET;
                } else if (currentBalance >= gasBill) {
                    currentBalance -= gasBill;
                    gasPaid = true;
                    recordTransaction("Gas Bill Payment", gasBill);
                    cout <<GREEN<< "Gas bill of PKR " << gasBill << " paid successfully.\n"<<RESET;
                } else {
                    cout <<RED<< "Insufficient balance to pay gas bill.\n"<<RESET;
                }
                break;

            case 3:
                if (internetPaid) {
                    cout<<YELLOW << "Internet bill already paid.\n"<<RESET;
                } else if (currentBalance >= internetBill) {
                    currentBalance -= internetBill;
                    internetPaid = true;
                    recordTransaction("Internet Bill Payment", internetBill);
                    cout <<GREEN<< "Internet bill of PKR " << internetBill << " paid successfully.\n"<<RESET;
                } else {
                    cout <<RED<< "Insufficient balance to pay internet bill.\n"<<RESET;
                }
                break;

            case 4:
                cout << "Exiting bill payments.\n";
                break;

            default:
                cout <<RED<< "Invalid choice. Please try again.\n"<<RESET;
        }

        // Update bill status file
        ofstream billStatusOut(billStatusFile);
        if (billStatusOut.is_open()) {
            billStatusOut << electricityPaid << " " << gasPaid << " " << internetPaid;
            billStatusOut.close();
        }

        // Exit if user selects 4
        if (choice == 4) break;

        // Check if all bills are paid
        if (electricityPaid && gasPaid && internetPaid) {
            cout << "All bills have been paid.\n";
            break;
        }
    }

    // Update user's balance file
    ofstream balanceFile(currentUser + "_balance.txt");
    if (balanceFile.is_open()) {
        balanceFile << currentBalance;
        balanceFile.close();
    }
}

void logout() {
    cout << CYAN << "Logging out...\n" << RESET;
    currentUser = "";
    currentBalance = 0.0;
}

void adminLogin() {
    string adminUsername = "admin";
    string adminPassword = "admin123";

    string username, password;
    cout << "Enter Admin username: ";
    cin >> username;
    cout << "Enter Admin password: ";
    cin >> password;

    if (username == adminUsername && password == adminPassword) {
        cout << GREEN << "Admin login successful.\n" << RESET;
        while (true) {
            displayAdminMenu();
            int choice;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    adminViewAccounts();
                    break;
                case 2: {
                    string username;
                    cout << "Enter username to view transaction history: ";
                    cin >> username;
                    adminViewTransactionHistory(username);
                    break;
                }
                case 3:
                    return;
                default:
                    cout << "Invalid choice. Try again.\n";
            }
        }
    } else {
        cout << RED << "Invalid admin credentials.\n" << RESET;
    }
}

void adminViewAccounts() {
    cout << YELLOW << "\n=== Account Details ===\n" << RESET;

    ifstream usersFile("usernames_list.txt");
    if (usersFile.is_open()) {
        string username;
        while (getline(usersFile, username)) {
            // Read balance
            double balance = 0.0;
            ifstream balanceFile(username + "_balance.txt");
            if (balanceFile.is_open()) {
                balanceFile >> balance;
                balanceFile.close();
            }

            // Read user details
            ifstream userFile(username + ".txt");
            string line, contactNumber, cnicNumber, creditCardNumber;

            if (userFile.is_open()) {
                while (getline(userFile, line)) {
                    if (line.find("Contact Number:") != string::npos)
                        contactNumber = line.substr(line.find(":") + 2);
                    else if (line.find("CNIC Number:") != string::npos)
                        cnicNumber = line.substr(line.find(":") + 2);
                    else if (line.find("Credit Card Number:") != string::npos)
                        creditCardNumber = line.substr(line.find(":") + 2);
                }
                userFile.close();
            }

            // Display user details with color coding
            cout << GREEN << "Username: " << RESET << username << endl;
            cout << RED << "Balance: " << RESET << fixed << setprecision(2) << balance << " PKR\n";
            cout << YELLOW << "Contact Number: " << RESET << contactNumber << endl;
            cout << CYAN << "CNIC Number: " << RESET << cnicNumber << endl;
            cout << MAGENTA << "Credit Card Number: " << RESET << creditCardNumber << "\n" << endl;
        }
        usersFile.close();
    } else {
        cout << RED << "No user data available.\n" << RESET;
    }
}


void adminViewTransactionHistory(const string& username) {
    cout <<BLUE<< "\n=== Transaction History for " <<MAGENTA<< username <<BLUE<< " ===\n"<<RESET;
    ifstream transactionFile(username + "_transactions.txt");
    if (transactionFile.is_open()) {
        string line;
        while (getline(transactionFile, line)) {
            cout <<YELLOW<< line <<RESET<< endl;
        }
        transactionFile.close();
    } else {
        cout <<RED<< "No transaction history available for this user.\n"<<RESET;
    }
}


string generateCreditCardNumber() {
    string cardNumber = "";
    for (int i = 0; i < 16; ++i) {
        cardNumber += to_string(rand() % 10);
    }
    return cardNumber;
}

void saveCreditCardNumber(const string& username, const string& creditCard) {
    ofstream creditCardFile(username + "_creditCard.txt");
    if (creditCardFile.is_open()) {
        creditCardFile << creditCard << endl;
        creditCardFile.close();
        cout << GREEN << "Credit Card Number saved successfully.\n" << RESET;
    } else {
        cout << RED << "Error saving credit card number.\n" << RESET;
    }
}

void recordTransaction(const string& type, double amount) {
    ofstream transactionFile(currentUser + "_transactions.txt", ios::app);
    if (transactionFile.is_open()) {
        time_t now = time(0);
        tm *ltm = localtime(&now);

        transactionFile << type << " | Amount: " << amount
                        << " | Date: " << 1900 + ltm->tm_year << "-"
                        << 1 + ltm->tm_mon << "-" << ltm->tm_mday
                        << " " << ltm->tm_hour << ":"
                        << ltm->tm_min << ":"
                        << ltm->tm_sec << endl;

        transactionFile.close();
    } else {
        cout << RED << "Error recording transaction.\n" << RESET;
    }
}
