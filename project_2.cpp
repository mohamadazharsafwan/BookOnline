#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <iomanip>

using namespace std;

#define YELLOW  "\033[33m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define RESET   "\033[0m"

const string CUSTOMER_FILE = "customers.txt";
const string BOOK_FILE = "books.txt";
const string ORDER_FILE = "orders.txt";
const string ADMIN_FILE = "admins.txt";
const string SALES_FILE = "sales_report.txt";

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

class Book {
public:
    string ISBN;
    string title;
    string author;
    double price;
    int stock;
    Book *next;

    Book(string isbn, string t, string a, double p, int s)
        : ISBN(isbn), title(t), author(a), price(p), stock(s), next(nullptr) {}

    void display() const {
        cout << YELLOW << "ISBN: " << ISBN << "\nTitle: " << title 
             << "\nAuthor: " << author << "\nPrice: $" << fixed << setprecision(2) << price 
             << "\nStock: " << stock << RESET << "\n\n";
    }
};

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

    void display() const {
        cout << YELLOW << "Order ID: " << orderId << "\nCustomer ID: " << customerId
             << "\nTotal: $" << fixed << setprecision(2) << total << "\nDate: " << date.toString()
             << "\nBooks: ";
        for (const auto &isbn : bookISBNs) cout << isbn << " ";
        cout << RESET << "\n\n";
    }
};

class Customer {
public:
    string id;
    string name;
    string email;
    string password;
    Order *orderHistory;
    Customer *next;

    Customer() : orderHistory(nullptr), next(nullptr) {
        id = generateID();
    }

    static string generateID() {
        return to_string(rand() % 90000 + 10000);
    }

    void display() const {
        cout << GREEN << "ID: " << id << "\nName: " << name 
             << "\nEmail: " << email << RESET << "\n\n";
    }

    void addOrder(Order *order) {
        if (!orderHistory) orderHistory = order;
        else {
            Order *temp = orderHistory;
            while (temp->next) temp = temp->next;
            temp->next = order;
        }
    }

    void updateProfile() {
        cout << "Enter new name (" << name << "): ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter new email (" << email << "): ";
        cin >> email;
    }
};

class Admin : public Customer {
public:
    Admin() {
        id = "ADM-" + generateID();
    }

    void display() const {
        cout << RED << "Admin ID: " << id << "\nName: " << name 
             << "\nEmail: " << email << RESET << "\n\n";
    }
};

class BookManager {
public:
    Book *head;
    
    BookManager() : head(nullptr) { loadFromFile(); }
    ~BookManager() { clear(); }

    void addBook(Book *newBook) {
        if (!head) head = newBook;
        else {
            Book *temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newBook;
        }
    }

    void displayBooks() const {
        Book *current = head;
        while (current) {
            current->display();
            current = current->next;
        }
    }

    Book* findBook(const string &ISBN) {
        Book *current = head;
        while (current) {
            if (current->ISBN == ISBN) return current;
            current = current->next;
        }
        return nullptr;
    }

    void deleteBook(const string &ISBN) {
        Book *curr = head, *prev = nullptr;
        while (curr) {
            if (curr->ISBN == ISBN) {
                if (prev) prev->next = curr->next;
                else head = curr->next;
                delete curr;
                cout << GREEN << "Book deleted!\n";
                saveToFile();
                return;
            }
            prev = curr;
            curr = curr->next;
        }
        cout << RED << "Book not found!\n";
    }

    void saveToFile() {
        ofstream file(BOOK_FILE);
        Book *current = head;
        while (current) {
            file << current->ISBN << "," << current->title << ","
                 << current->author << "," << current->price << ","
                 << current->stock << "\n";
            current = current->next;
        }
    }

    void mergeSortByTitle() {
        head = mergeSort(head);
        cout << GREEN << "Sorted (A-Z) using Merge Sort!\n";
    }

    void quickSortByTitle() {
        head = quickSort(head, true);
        cout << GREEN << "Sorted (Z-A) using Quick Sort!\n";
    }

private:
    void loadFromFile() {
        ifstream file(BOOK_FILE);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string ISBN, title, author, priceStr, stockStr;
            getline(ss, ISBN, ',');
            getline(ss, title, ',');
            getline(ss, author, ',');
            getline(ss, priceStr, ',');
            getline(ss, stockStr, ',');

            addBook(new Book(ISBN, title, author, stod(priceStr), stoi(stockStr)));
        }
    }

    void clear() {
        while (head) {
            Book *temp = head;
            head = head->next;
            delete temp;
        }
    }

    Book* mergeSort(Book* head) {
        if (!head || !head->next) return head;
        
        Book* slow = head;
        Book* fast = head->next;
        
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        
        Book* mid = slow->next;
        slow->next = nullptr;
        
        return merge(mergeSort(head), mergeSort(mid));
    }

    Book* merge(Book* left, Book* right) {
        Book dummy("", "", "", 0, 0);
        Book* curr = &dummy;
        
        while (left && right) {
            if (left->title < right->title) {
                curr->next = left;
                left = left->next;
            } else {
                curr->next = right;
                right = right->next;
            }
            curr = curr->next;
        }
        
        curr->next = left ? left : right;
        return dummy.next;
    }

    Book* quickSort(Book *start, bool byTitle) {
        if (!start || !start->next) return start;

        Book *pivot = start;
        Book *left = nullptr;
        Book *right = nullptr;
        Book *curr = start->next;

        while (curr) {
            Book *next = curr->next;
            if ((byTitle && curr->title > pivot->title) || 
                (!byTitle && curr->ISBN < pivot->ISBN)) {
                curr->next = left;
                left = curr;
            } else {
                curr->next = right;
                right = curr;
            }
            curr = next;
        }

        left = quickSort(left, byTitle);
        right = quickSort(right, byTitle);

        pivot->next = right;
        if (!left) return pivot;

        Book *temp = left;
        while (temp->next) temp = temp->next;
        temp->next = pivot;
        return left;
    }
};

class OrderManager {
public:
    Order *head;

    OrderManager() : head(nullptr) { loadOrders(); }
    ~OrderManager() { clear(); }

    void addOrder(Order *newOrder) {
        if (!head) head = newOrder;
        else {
            Order *temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newOrder;
        }
        saveOrders();
    }

    void displayOrders() const {
        Order *current = head;
        while (current) {
            current->display();
            current = current->next;
        }
    }

    void deleteOrder(const string &orderId) {
        Order *curr = head, *prev = nullptr;
        while (curr) {
            if (curr->orderId == orderId) {
                if (prev) prev->next = curr->next;
                else head = curr->next;
                delete curr;
                cout << GREEN << "Order deleted!\n";
                saveOrders();
                return;
            }
            prev = curr;
            curr = curr->next;
        }
        cout << RED << "Order not found!\n";
    }

    void saveOrders() {
        ofstream file(ORDER_FILE);
        Order *current = head;
        while (current) {
            file << current->orderId << "," << current->customerId << ",";
            for (size_t i = 0; i < current->bookISBNs.size(); ++i) {
                file << current->bookISBNs[i];
                if (i != current->bookISBNs.size() - 1) file << "|";
            }
            file << ";" << current->total << "," << current->date.toString() << "\n";
            current = current->next;
        }
    }

private:
    void loadOrders() {
        ifstream file(ORDER_FILE);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string orderId, customerId, dateStr, totalStr;
            vector<string> isbns;

            getline(ss, orderId, ',');
            getline(ss, customerId, ',');
            
            string isbnList;
            getline(ss, isbnList, ';');
            stringstream iss(isbnList);
            string isbn;
            while (getline(iss, isbn, '|')) {
                isbns.push_back(isbn);
            }

            getline(ss, totalStr, ',');
            getline(ss, dateStr, ',');

            Date orderDate;
            stringstream dss(dateStr);
            string part;
            getline(dss, part, '/');
            orderDate.day = stoi(part);
            getline(dss, part, '/');
            orderDate.month = stoi(part);
            getline(dss, part, '/');
            orderDate.year = stoi(part);

            addOrder(new Order(orderId, customerId, isbns, stod(totalStr), orderDate));
        }
    }

    void clear() {
        while (head) {
            Order *temp = head;
            head = head->next;
            delete temp;
        }
    }
};

class CustomerManager {
public:
    Customer *head;
    Admin *admin;

    CustomerManager() : head(nullptr), admin(nullptr) { 
        loadCustomers();
        loadAdmin();
    }

    ~CustomerManager() { 
        clearCustomers();
        delete admin;
    }

    Customer* registerCustomer() {
        Customer *newCustomer = new Customer();
        cout << "Enter name: ";
        cin.ignore();
        getline(cin, newCustomer->name);
        cout << "Enter email: ";
        cin >> newCustomer->email;
        cout << "Enter password: ";
        cin >> newCustomer->password;

        if (!head) head = newCustomer;
        else {
            Customer *temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newCustomer;
        }
        saveCustomers();
        return newCustomer;
    }

    Customer* login(const string &email, const string &password) {
        Customer *current = head;
        while (current) {
            if (current->email == email && current->password == password) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void deleteCustomer(Customer *customer) {
        Customer *curr = head, *prev = nullptr;
        while (curr) {
            if (curr == customer) {
                if (prev) prev->next = curr->next;
                else head = curr->next;
                delete curr;
                cout << GREEN << "Account deleted!\n";
                saveCustomers();
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }

    void saveCustomers() {
        ofstream file(CUSTOMER_FILE);
        Customer *current = head;
        while (current) {
            file << current->id << "," << current->name << ","
                 << current->email << "," << current->password << "\n";
            current = current->next;
        }
    }

private:
    void loadCustomers() {
        ifstream file(CUSTOMER_FILE);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string id, name, email, password;
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, email, ',');
            getline(ss, password, ',');

            Customer *newCustomer = new Customer();
            newCustomer->id = id;
            newCustomer->name = name;
            newCustomer->email = email;
            newCustomer->password = password;

            if (!head) head = newCustomer;
            else {
                Customer *temp = head;
                while (temp->next) temp = temp->next;
                temp->next = newCustomer;
            }
        }
    }

    void loadAdmin() {
        ifstream file(ADMIN_FILE);
        string line;
        if (getline(file, line)) {
            stringstream ss(line);
            string id, name, email, password;
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, email, ',');
            getline(ss, password, ',');

            admin = new Admin();
            admin->id = id;
            admin->name = name;
            admin->email = email;
            admin->password = password;
        }
    }

    void clearCustomers() {
        while (head) {
            Customer *temp = head;
            head = head->next;
            delete temp;
        }
    }
};

class BookstoreApp {
    BookManager books;
    CustomerManager customers;
    OrderManager orders;

public:
    void run() {
        srand(time(0));
        showWelcome();
        int role = getRole();

        if (role == 1) customerFlow();
        else if (role == 2) adminFlow();
        else cout << RED << "Invalid selection!\n";
    }

private:
    void showWelcome() {
        cout << YELLOW << "************************************************\n"
             << "------------------------------------------------\n"
             << "*****WELCOME TO ONLINE BOOK SHOPPING SYSTEM*****\n"
             << "------------------------------------------------\n"
             << "************************************************\n" << RESET;
    }

    int getRole() {
        int role;
        cout << "\n1. Customer\n2. Admin\nChoose Role: ";
        cin >> role;
        return role;
    }

    void customerFlow() {
        int choice;
        do {
            cout << YELLOW << "\n1. Register\n2. Login\n3. Exit\nChoice: ";
            cin >> choice;

            if (choice == 1) {
                Customer *customer = customers.registerCustomer();
                customerMenu(customer);
            } else if (choice == 2) {
                string email, password;
                cout << "Email: ";
                cin >> email;
                cout << "Password: ";
                cin >> password;
                
                Customer *customer = customers.login(email, password);
                if (customer) {
                    cout << GREEN << "Login successful!\n";
                    customerMenu(customer);
                } else {
                    cout << RED << "Invalid credentials!\n";
                }
            }
        } while (choice != 3);
    }

    void customerMenu(Customer *customer) {
        int choice;
        do {
            cout << YELLOW << "\nCustomer Menu\n"
                 << "1. View Profile\n"
                 << "2. View Books\n"
                 << "3. View Order History\n"
                 << "4. Place Order\n"
                 << "5. Edit Profile\n"
                 << "6. Delete Account\n"
                 << "7. Logout\nChoice: ";
            cin >> choice;

            switch (choice) {
                case 1: customer->display(); break;
                case 2: displayBooks(); break;
                case 3: displayOrderHistory(customer); break;
                case 4: placeOrder(customer); break;
                case 5: 
                    customer->updateProfile();
                    customers.saveCustomers();
                    break;
                case 6: 
                    customers.deleteCustomer(customer);
                    return;
            }
        } while (choice != 7);
    }

    void adminFlow() {
        string email, password;
        cout << "Admin Email: ";
        cin >> email;
        cout << "Password: ";
        cin >> password;

        if (customers.admin && customers.admin->email == email && 
            customers.admin->password == password) {
            adminMenu();
        } else {
            cout << RED << "Admin login failed!\n";
        }
    }

    void adminMenu() {
        int choice;
        do {
            cout << RED << "\nAdmin Menu\n"
                 << "1. View Customers\n"
                 << "2. View Orders\n"
                 << "3. View Books\n"
                 << "4. Add Book\n"
                 << "5. Edit Book\n"
                 << "6. Delete Book\n"
                 << "7. Generate Report\n"
                 << "8. Logout\nChoice: ";
            cin >> choice;

            switch (choice) {
                case 1: displayCustomers(); break;
                case 2: orders.displayOrders(); break;
                case 3: displayBooks(); break;
                case 4: addBook(); break;
                case 5: editBook(); break;
                case 6: deleteBook(); break;
                case 7: generateReport(); break;
            }
        } while (choice != 8);
    }

    void displayBooks() {
        cout << "\nBefore sorting:\n";
        books.displayBooks();
        
        cout << "Choose sorting algorithm:\n"
             << "1. Merge Sort by Title (A-Z)\n"
             << "2. Quick Sort by Title (Z-A)\nChoice: ";
        int sortChoice;
        cin >> sortChoice;
        
        if (sortChoice == 1) books.mergeSortByTitle();
        else if (sortChoice == 2) books.quickSortByTitle();
        
        cout << "\nAfter sorting:\n";
        books.displayBooks();
    }

    void displayOrderHistory(Customer *customer) {
        cout << YELLOW << "\nOrder History:\n";
        Order *current = customer->orderHistory;
        while (current) {
            current->display();
            current = current->next;
        }
    }

    void displayCustomers() {
        cout << YELLOW << "\nRegistered Customers:\n";
        Customer *current = customers.head;
        while (current) {
            current->display();
            current = current->next;
        }
    }

    Date getCurrentDate() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        return {ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year};
    }

    void placeOrder(Customer *customer) {
        vector<string> isbns;
        double total = 0;
        Book *current = books.head;
        
        cout << "\nAvailable Books:\n";
        books.displayBooks();
        
        cout << "Enter ISBNs to purchase (separated by spaces, 0 to finish):\n";
        string isbn;
        while (true) {
            cin >> isbn;
            if (isbn == "0") break;
            
            Book *book = books.findBook(isbn);
            if (book && book->stock > 0) {
                isbns.push_back(isbn);
                total += book->price;
                book->stock--;
            } else {
                cout << RED << "Invalid ISBN or out of stock!\n";
            }
        }

        if (!isbns.empty()) {
            Order *newOrder = new Order(
                "ORD" + to_string(rand() % 9000 + 1000),
                customer->id,
                isbns,
                total,
                getCurrentDate()
            );
            orders.addOrder(newOrder);
            customer->addOrder(newOrder);
            books.saveToFile();
            cout << GREEN << "Order placed successfully! Total: $" << total << "\n";
        }
    }

    void addBook() {
        string ISBN, title, author;
        double price;
        int stock;

        cout << "Enter ISBN: ";
        cin >> ISBN;
        cout << "Enter title: ";
        cin.ignore();
        getline(cin, title);
        cout << "Enter author: ";
        getline(cin, author);
        cout << "Enter price: ";
        cin >> price;
        cout << "Enter stock: ";
        cin >> stock;

        books.addBook(new Book(ISBN, title, author, price, stock));
        books.saveToFile();
        cout << GREEN << "Book added successfully!\n";
    }

    void editBook() {
        string ISBN;
        cout << "Enter ISBN to edit: ";
        cin >> ISBN;
        
        Book* book = books.findBook(ISBN);
        if (book) {
            cout << "Enter new price (" << book->price << "): ";
            cin >> book->price;
            cout << "Enter new stock (" << book->stock << "): ";
            cin >> book->stock;
            books.saveToFile();
            cout << GREEN << "Book updated!\n";
        } else {
            cout << RED << "Book not found!\n";
        }
    }

    void deleteBook() {
        string ISBN;
        cout << "Enter ISBN to delete: ";
        cin >> ISBN;
        books.deleteBook(ISBN);
    }

    void generateReport() {
        ofstream report(SALES_FILE);
        Order *current = orders.head;
        while (current) {
            report << "Order ID: " << current->orderId 
                   << ", Total: $" << current->total
                   << ", Date: " << current->date.toString() << "\n";
            current = current->next;
        }
        cout << GREEN << "Sales report generated!\n";
    }
};

int main() {
    BookstoreApp app;
    app.run();
    return 0;
}