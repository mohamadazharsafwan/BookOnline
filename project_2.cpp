#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime> // For random ID generation

#define YELLOW  "\033[33m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

using namespace std;

void customersetup();
void adminsetup();
void customerpage();
void adminpage();
void viewAllCustomers();
void manageOrders();
void viewReports();
void addNewBooks();
void generateSalesReport();


// Structures
struct Date {
    int day, month, year;

    string toString() const {
        return to_string(day) + "/" + to_string(month) + "/" + to_string(year);
    }

    bool operator>(const Date &other) const {
        if (year != other.year) return year > other.year;
        if (month != other.month) return month > other.month;
        return day > other.day;
    }
};

struct Address {
    string street, city, state;
};

// Book Class
class Book {
public:
    string ISBN;
    string title;
    string author;
    double price;
    int stock;
    Book *next;

    Book(string isbn, string t, string a, double p, int s)  // to be repair 
        : ISBN(isbn), title(t), author(a), price(p), stock(s), next(nullptr) {}

    friend void displayBook(Book *b);
};

// Order Class
class Order {
public:
    string orderId;
    string customerId;
    vector<string> bookISBNs;
    double total;
    Date date;
    Order *next;

    Order(string oid, string cid, vector<string> isbns, double t, Date d) 
        : orderId(oid), customerId(cid), bookISBNs(isbns), total(t), date(d), next(nullptr) {}

    friend void displayOrder(Order *o);
};

// Report and SalesReport Classes
class Report {
public:
    virtual void generate() = 0;
};

class SalesReport : public Report {
    vector<Order*> orders;
public:
    void generate() override {
        // Implementation to generate report
    }
    friend void displaySalesReport(SalesReport *sr);
};

class Customer {
public:
    string id;
    string name;
    string email;
    string password;
    Customer *next; // Pointer to next customer

    Customer() {
        id = generateID();
        next = NULL;
    }


    // Friend function to display profile (used in both Customer and Admin)
    friend void displayProfile(Customer *c);

protected:
    string generateID() {
        srand(time(0)); // Seed random number generator
        return to_string(rand() % 90000 + 10000); // Generate 5-digit ID
    }
};

// Admin class inherits from Customer
class Admin : public Customer {
public:
    Customer *head; // Add head member
    

    Admin() {
        id = "ADM-" + generateID(); // Admin ID with ADM prefix
        head = NULL;
        
    }
};

// Friend function to display profiles
void displayProfile(Customer *c) {
    cout << GREEN << "ID: " << c->id << endl;
    cout << "Name: " << c->name << endl;
    cout << "Email: " << c->email << endl;
}

// Customer List (Linked List)
class CustomerList {
public:
    Customer *head;
    Customer *temp;

    CustomerList() {
        head = NULL;
        temp = NULL;
    }

    void addCustomer(string id, string name, string email, string password) {
        Customer *newCustomer = new Customer();
        newCustomer->id = id;
        newCustomer->name = name;
        newCustomer->email = email;
        newCustomer->password = password;
        newCustomer->next = NULL;

        if (head == NULL) {
            head = newCustomer;
            temp = newCustomer;
        } else {
            temp->next = newCustomer;
            temp = newCustomer;
        }
    }

    Customer *searchCustomer(string email) {
        Customer *current = head;
        while (current != NULL) {
            if (current->email == email) {
                return current;
            }
            current = current->next;
        }
        return NULL;
    }

    bool login(string email, string password) {
        Customer *current = head;
        while (current != NULL) {
            if (current->email == email && current->password == password) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
};

void customerpage() {   //cutomer page binary search quick sort for book sorting
    cout << YELLOW << "Welcome to Customer Page\n";
    cout << "1. Display Profile\n";
    cout << "2. Update Profile\n"; 
    cout << "3. History Purchase\n"; //searching planned
    cout << "4. Delete Profile\n";
}

void adminpage() {
    int choice;
    cout << YELLOW << "Welcome to Admin Page\n";
    cout << "1. View All Customers\n"; //searching  hash
    cout << "2. Manage Orders\n"; //sorting and searching planned merge sort
    cout << "3. View Reports\n"; //sorting planned bubble sort
    cout << "4. Add New Books\n"; 
    cout << "5. Generate Sales Report\n";
    cin >> choice;

    switch (choice) {
            case 1:
                viewAllCustomers();
                break;
            case 2:
                manageOrders();
                break;
            case 3:
                viewReports();
                break;
            case 4:
                addNewBooks();
                break;
            case 5:
                generateSalesReport();
                break;
            case 6:
                cout << "Exiting Admin Page. Goodbye!\n";
                return;
            default:
                cout << RED << "Invalid choice. Please try again.\n";
    }
}

void viewAllCustomers() {
    cout << YELLOW << "View All Customers\n";
    cout << "------------------------------------------------\n";
    cout << "****************VIEW ALL CUSTOMER***************\n";
    cout << "------------------------------------------------\n";
    // Display all customers
}

void manageOrders() {
    cout << YELLOW << "Manage Orders\n";
    cout << "------------------------------------------------\n";
    cout << "******************MANAGE ORDERS*****************\n";
    cout << "------------------------------------------------\n";
    // Display all orders
}

void viewReports() {
    cout << YELLOW << "View Reports\n";
    cout << "------------------------------------------------\n";
    cout << "******************VIEW REPORTS******************\n";
    cout << "------------------------------------------------\n";
    // Display reports
}

void addNewBooks() {
    cout << YELLOW << "Add New Books\n";
    cout << "------------------------------------------------\n";
    cout << "******************ADD NEW BOOKS*****************\n";
    cout << "------------------------------------------------\n";
    // Add new books
}

void generateSalesReport() {
    cout << YELLOW << "Generate Sales Report\n";
    cout << "------------------------------------------------\n";
    cout << "**************GENERATE SALES REPORT*************\n";
    cout << "------------------------------------------------\n";
    // Generate sales report
}

void customersetup() {
    CustomerList cusL;
    string line;

    // Read data from customer.txt
    ifstream CustomerFile("customer.txt");
    string id, name, email, password;
    while (getline(CustomerFile, line)) {
        stringstream ss(line);
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, email, ',');
        getline(ss, password, ',');
        cusL.addCustomer(id, name, email, password);
    }

    int choice;
    do {
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                Customer newCustomer;
                cout << "Enter Name: ";
                cin.ignore();
                getline(cin, newCustomer.name);
                cout << "Enter Email: ";
                cin >> newCustomer.email;
                cout << "Enter Password: ";
                cin >> newCustomer.password;

                cusL.addCustomer(newCustomer.id, newCustomer.name, newCustomer.email, newCustomer.password);
                cout << GREEN << "Registration successful!\n";
                break;
            }
            case 2: {
                cout << "Enter Email: ";
                cin >> email;
                cout << "Enter Password: ";
                cin >> password;

                if (cusL.login(email, password)) {
                    cout << GREEN << "Login successful!\n";
                    Customer *loggedInUser = cusL.searchCustomer(email);
                    displayProfile(loggedInUser);
                    customerpage();
                    return;
                } else {
                    cout << RED << "Login Failed\n";
                }
                break;
            }
            case 3:
                cout << "Exiting...\n";
                return;
            default:
                cout << "Invalid choice, please try again.\n";
        }
    } while (choice != 3);
}

void adminsetup() {
    // linked list for admin
    Admin admin1;
    


    int choice;
    string email, password;
    do {
        cout << "Admin Login\n";
        cout << "Enter Email: ";
        cin >> email;
        cout << "Enter Password: ";
        cin >> password;

        if (email == admin1.email && password == admin1.password) {
            cout << GREEN << "Admin Login Successful!\n";
            displayProfile(&admin1);
            adminpage();
            return;
        } else {
            cout << RED << "Invalid Credentials!\n";
        }
    } while (true);
}

int main() {
    int role;
    cout << "************************************************\n";
    cout << "------------------------------------------------\n";
    cout << "*****WELCOME TO ONLINE BOOK SHOOPING SYSTEM*****\n";
    cout << "------------------------------------------------\n";
    cout << "************************************************\n";
    
    
    cout << "1. Customer\n2. Admin\nChoose Role: ";
    cin >> role;

    if (role == 1) {
        customersetup();
    } else if (role == 2) {
        adminsetup();
    } else {
        cout << RED << "Invalid Choice!\n";
    }
    return 0;
}
