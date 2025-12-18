#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cstring>

using namespace std;

// -------------------- CONSTANTS --------------------
const string CUSTOMER_FILE = "customers.txt";
const string BILLING_FILE  = "billing.txt";
const string PASSWORD_FILE = "password.txt";

// -------------------- CLASSES --------------------

class Customer {
public:
    int id;
    string name;
    string phone;
    string address;
    string plan;

    void save(ofstream &out) {
        out << id << "|" << name << "|" << phone << "|" << address << "|" << plan << "\n";
    }
};

class Billing {
public:
    int customerId;
    float callDuration;
    float internetData;
    float roamingCalls;
    float internationalCalls;

    void save(ofstream &out) {
        out << customerId << "|" << callDuration << "|" << internetData << "|"
            << roamingCalls << "|" << internationalCalls << "\n";
    }
};

// -------------------- GLOBAL DATA --------------------
vector<Customer> customers;
vector<Billing> billings;

// -------------------- UTILITY FUNCTIONS --------------------

bool validPhone(const string &p) {
    if (p.length() != 10) return false;
    for (char c : p)
        if (!isdigit(c)) return false;
    return true;
}

// -------------------- PASSWORD SYSTEM --------------------

bool checkPassword(const string &input) {
    ifstream in(PASSWORD_FILE);
    if (!in) {
        ofstream out(PASSWORD_FILE);
        out << input;
        out.close();
        cout << "Password set successfully!\n";
        return true;
    }

    string stored;
    getline(in, stored);
    return input == stored;
}

void changePassword() {
    string p1, p2;
    cout << "Enter new password: ";
    cin >> p1;
    cout << "Confirm password: ";
    cin >> p2;

    if (p1 == p2) {
        ofstream out(PASSWORD_FILE);
        out << p1;
        out.close();
        cout << "Password changed successfully!\n";
    } else {
        cout << "Passwords do not match!\n";
    }
}

// -------------------- FILE LOADING --------------------

void loadCustomers() {
    ifstream in(CUSTOMER_FILE);
    if (!in) return;

    Customer c;
    string line;
    while (getline(in, line)) {
        sscanf(line.c_str(), "%d|", &c.id);
        size_t p1 = line.find('|') + 1;
        size_t p2 = line.find('|', p1);
        c.name = line.substr(p1, p2 - p1);

        p1 = p2 + 1; p2 = line.find('|', p1);
        c.phone = line.substr(p1, p2 - p1);

        p1 = p2 + 1; p2 = line.find('|', p1);
        c.address = line.substr(p1, p2 - p1);

        c.plan = line.substr(p2 + 1);
        customers.push_back(c);
    }
}

void loadBilling() {
    ifstream in(BILLING_FILE);
    if (!in) return;

    Billing b;
    string line;
    while (getline(in, line)) {
        sscanf(line.c_str(), "%d|%f|%f|%f|%f",
               &b.customerId, &b.callDuration, &b.internetData,
               &b.roamingCalls, &b.internationalCalls);
        billings.push_back(b);
    }
}

// -------------------- CUSTOMER FUNCTIONS --------------------

void addCustomer() {
    Customer c;
    cout << "Enter ID: ";
    cin >> c.id;

    for (auto &x : customers)
        if (x.id == c.id) {
            cout << "ID already exists!\n";
            return;
        }

    cout << "Name: ";
    cin >> c.name;

    do {
        cout << "Phone (10 digits): ";
        cin >> c.phone;
    } while (!validPhone(c.phone));

    cout << "Address: ";
    cin.ignore();
    getline(cin, c.address);

    do {
        cout << "Plan (prepaid/postpaid): ";
        cin >> c.plan;
    } while (c.plan != "prepaid" && c.plan != "postpaid");

    customers.push_back(c);
    ofstream out(CUSTOMER_FILE, ios::app);
    c.save(out);
    cout << "Customer added successfully!\n";
}

void viewCustomers() {
    cout << "\nID\tName\tPhone\t\tPlan\n";
    for (auto &c : customers)
        cout << c.id << "\t" << c.name << "\t" << c.phone << "\t" << c.plan << "\n";
}

// -------------------- BILLING FUNCTIONS --------------------

void addBilling() {
    Billing b;
    cout << "Enter Customer ID: ";
    cin >> b.customerId;

    cout << "Call Duration (min): ";
    cin >> b.callDuration;
    cout << "Internet Data (MB): ";
    cin >> b.internetData;
    cout << "Roaming Calls (min): ";
    cin >> b.roamingCalls;
    cout << "International Calls (min): ";
    cin >> b.internationalCalls;

    billings.push_back(b);
    ofstream out(BILLING_FILE, ios::app);
    b.save(out);

    cout << "Billing added successfully!\n";
}

void generateBill() {
    int id;
    cout << "Enter Customer ID: ";
    cin >> id;

    for (auto &b : billings) {
        if (b.customerId == id) {
            float total = b.callDuration * 1.5 +
                          b.internetData * 5 +
                          b.roamingCalls * 3 +
                          b.internationalCalls * 6;

            cout << fixed << setprecision(2);
            cout << "\nTOTAL BILL = Rs. " << total << "\n";
            return;
        }
    }
    cout << "Billing not found!\n";
}

// -------------------- MAIN MENU --------------------

void menu() {
    int ch;
    do {
        cout << "\n--- TELECOM BILLING SYSTEM ---\n";
        cout << "1. Add Customer\n";
        cout << "2. View Customers\n";
        cout << "3. Add Billing\n";
        cout << "4. Generate Bill\n";
        cout << "5. Exit\n";
        cout << "Choice: ";
        cin >> ch;

        switch (ch) {
            case 1: addCustomer(); break;
            case 2: viewCustomers(); break;
            case 3: addBilling(); break;
            case 4: generateBill(); break;
        }
    } while (ch != 5);
}

// -------------------- MAIN --------------------

int main() {
    loadCustomers();
    loadBilling();

    string pass;
    cout << "Enter Password: ";
    cin >> pass;

    if (!checkPassword(pass)) {
        cout << "Access Denied!\n";
        return 0;
    }

    menu();
    return 0;
}
