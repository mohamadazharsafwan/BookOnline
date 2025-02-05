#include <iostream>  // For cin, cout
#include <string>  // For string
#include <vector> // For vector
#include <fstream> // For file I/O
#include <sstream> // For string stream
#include <ctime> // For time
#include <cstdlib> // For rand, srand
#include <iomanip> // For setprecision

using namespace std;

// ANSI color codes for text formatting
#define YELLOW "\033[33m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define RESET "\033[0m"


//Stores the filename for the customer data file
const string CUSTOMER_FILE = "customers.txt"; 

//Stores the filename for the book data file
const string BOOK_FILE = "books.txt";

//Stores the filename for the order data file
const string ORDER_FILE = "orders.txt";

//Stores the filename for the admin data file
const string ADMIN_FILE = "admins.txt";

//Stores the filename for the sales report file
const string SALES_FILE = "sales_report.txt";


//Structure to store the date
struct Date
{
    //Stores the day, month, and year
    int day, month, year;

    //Converts the date to a string
    string toString() const
    {
        //Returns the date in the format day/month/year
        return to_string(day) + "/" + to_string(month) + "/" + to_string(year);
    }

    //Overloads the less than operator
    bool operator>(const Date &other) const
    {
        //Compares the years first
        if (year != other.year)

        //Returns true if the year is greater than the other year
            return year > other.year;

            //Compares the months next
        if (month != other.month)

        //Returns true if the month is greater than the other month
            return month > other.month;
        return day > other.day;
    }
};

//Class to store the book data
class Book
{
    //Stores the ISBN, title, author, price, stock, and a pointer to the next book
public:
    string ISBN;
    string title;
    string author;
    double price;
    int stock;

    //Pointer to the next book
    Book *next;

    //Constructor to initialize the book data
    Book(string isbn, string t, string a, double p, int s)
        : ISBN(isbn), title(t), author(a), price(p), stock(s), next(nullptr) {}

    //Displays the book data
    void display() const
    {
        //Displays the book data in the format ISBN, title, author, price, and stock
        cout << YELLOW << "ISBN: " << ISBN << "\nTitle: " << title
             << "\nAuthor: " << author << "\nPrice: $" << fixed << setprecision(2) << price
             << "\nStock: " << stock << RESET << "\n\n";
    }
};

//Class to store the order data
class Order
{
public:
    string orderId;
    string customerId;

    //Stores the ISBNs of the books in the order
    vector<string> bookISBNs;
    double total;
    Date date;

    //Pointer to the next order
    Order *next;

    //Constructor to initialize the order data
    Order(string oid, string cid, vector<string> isbns, double t, Date d)
        : orderId(oid), customerId(cid), bookISBNs(isbns), total(t), date(d), next(nullptr) {}

    //Displays the order data
    void display() const
    {
        //Displays the order data in the format Order ID, Customer ID, Total, Date, and Books
        cout << YELLOW << "Order ID: " << orderId << "\nCustomer ID: " << customerId
             << "\nTotal: $" << fixed << setprecision(2) << total << "\nDate: " << date.toString()
             << "\nBooks: ";

        //Displays the ISBNs of the books in the order
        for (const auto &isbn : bookISBNs)
            cout << isbn << " ";
        cout << RESET << "\n\n";
    }
};

//Class to store the customer data
class Customer
{
    //Stores the customer ID, name, email, password, order history, and a pointer to the next customer
public:
    string id;
    string name;
    string email;
    string password;

    //Pointer to the order history
    Order *orderHistory;

    //Pointer to the next customer
    Customer *next;

    //Constructor to initialize the customer data
    Customer() : orderHistory(nullptr), next(nullptr)
    {
        //Generates a random ID for the customer
        id = generateID();
    }

    //Generates a random ID for the customer
    static string generateID()
    {
        //Returns a random 5-digit number as a string
        return to_string(rand() % 90000 + 10000);
    }

    //Displays the customer data
    void display() const
    {
        //Displays the customer data in the format ID, name, and email
        cout << GREEN << "ID: " << id << "\nName: " << name
             << "\nEmail: " << email << RESET << "\n\n";
    }

    //Adds an order to the order history
    void addOrder(Order *order)
    {
        //If the order history is empty, set the order as the first order
        if (!orderHistory)

            //Otherwise, add the order to the end of the order history
            orderHistory = order;
        else
        {   
            //Pointer to the last order in the order history
            Order *temp = orderHistory;

            //Iterates through the order history to find the last order
            while (temp->next)

                //Sets the next order as the last order
                temp = temp->next;

                //Adds the order to the end of the order history
            temp->next = order;
        }
    }

    //Updates the customer profile
    void updateProfile()
    {
        //Prompts the user to enter the new name and email
        cout << "Enter new name (" << name << "): ";
        cin.ignore();

        //Stores the new name
        getline(cin, name);

        //Prompts the user to enter the new email
        cout << "Enter new email (" << email << "): ";
        cin >> email;
    }
};

//Class to store the admin data
class Admin : public Customer //Inherits the customer data
{
public:
    //Constructor to initialize the admin data
    Admin()
    {
        //Generates a random ID for the admin
        id = "ADM-" + generateID();
    }

    //Displays the admin data
    void display() const
    {

        //Displays the admin data in the format ID, name, and email
        cout << RED << "Admin ID: " << id << "\nName: " << name
             << "\nEmail: " << email << RESET << "\n\n";
    }
};

//Class to manage the book data
class BookManager
{
public:

    //Pointer to the first book
    Book *head;

    //Constructor to initialize the book manager
    BookManager() : head(nullptr) { loadFromFile(); }

    //Destructor to clear the book manager
    ~BookManager() { clear(); }

    //Adds a new book to the book manager
    void addBook(Book *newBook)
    {
        //If the book manager is empty, set the new book as the first book
        if (!head)

            //Otherwise, add the new book to the end of the book manager
            head = newBook;
        else
        {
            //Pointer to the last book in the book manager
            Book *temp = head;

            //Iterates through the book manager to find the last book
            while (temp->next)

                //Sets the next book as the last book
                temp = temp->next;

                //Adds the new book to the end of the book manager
            temp->next = newBook;
        }
    }

    //Displays all the books in the book manager
    void displayBooks() const
    {
        //Pointer to the current book
        Book *current = head;


        //Iterates through the book manager to display all the books
        while (current)
        {
            //Displays the current book
            current->display();

            //Moves to the next book
            current = current->next;
        }
    }

    //Finds a book by ISBN
    Book *findBook(const string &ISBN)
    {
        //Pointer to the current book
        Book *current = head;

        //Iterates through the book manager to find the book with the given ISBN
        while (current)
        {
            //If the ISBN of the current book matches the given ISBN, return the current book
            if (current->ISBN == ISBN)
                return current;

                //Moves to the next book
            current = current->next;
        }

        //Returns null if the book with the given ISBN is not found
        return nullptr;
    }

    //Deletes a book by ISBN
    void deleteBook(const string &ISBN)
    {
        //Pointer to the current book and the previous book
        Book *curr = head, *prev = nullptr;
        while (curr)
        {
            //If the ISBN of the current book matches the given ISBN
            if (curr->ISBN == ISBN)
            {
                //If the previous book is not null, set the next book as the next of the previous book
                if (prev)
                    prev->next = curr->next;
                else

                    //Otherwise, set the next book as the first book
                    head = curr->next;

                    //Deletes the current book
                delete curr;
                cout << GREEN << "Book deleted!\n";
                saveToFile();
                return;
            }

            //Moves to the next book
            prev = curr;
            curr = curr->next;
        }
        cout << RED << "Book not found!\n";
    }

    //Saves the book data to a file
    void saveToFile()
    {
        //Opens the file for writing
        ofstream file(BOOK_FILE);

        //Pointer to the current book
        Book *current = head;

        //Iterates through the book manager to write the book data to the file
        while (current)
        {
            //Writes the ISBN, title, author, price, and stock of the current book to the file
            file << current->ISBN << "," << current->title << ","
                 << current->author << "," << current->price << ","
                 << current->stock << "\n";

                 //Moves to the next book
            current = current->next;
        }
    }

    //Loads the book data from a file
    void mergeSortByTitle()
    {
        //Sorts the books by title using merge sort
        head = mergeSort(head);
        
        //Displays a message after sorting
        cout << GREEN << "Sorted (A-Z) using Merge Sort!\n";
    }

    //Loads the book data from a file
    void quickSortByTitle()
    {
        //Sorts the books by title using quick sort
        head = quickSort(head, true);

        //Displays a message after sorting
        cout << GREEN << "Sorted (Z-A) using Quick Sort!\n";
    }

     vector<Book*> getBooksVector() {
        vector<Book*> booksVec;
        Book* current = head;
        while (current) {
            booksVec.push_back(current);
            current = current->next;
        }
        return booksVec;
    }

    void bubbleSortByISBN(vector<Book*>& booksVec) {
        int n = booksVec.size();
        bool swapped;
        for (int i = 0; i < n-1; i++) {
            swapped = false;
            for (int j = 0; j < n-i-1; j++) {
                if (booksVec[j]->ISBN > booksVec[j+1]->ISBN) {
                    swap(booksVec[j], booksVec[j+1]);
                    swapped = true;
                }
            }
            if (!swapped) break;
        }
    }

    Book* binarySearchByISBN(const vector<Book*>& booksVec, const string& ISBN) {
        int left = 0;
        int right = booksVec.size() - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (booksVec[mid]->ISBN == ISBN) {
                return booksVec[mid];
            } else if (booksVec[mid]->ISBN < ISBN) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        return nullptr;
    }

private:
    void loadFromFile()
    {
        ifstream file(BOOK_FILE);
        string line;
        while (getline(file, line))
        {
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

    void clear()
    {
        while (head)
        {
            Book *temp = head;
            head = head->next;
            delete temp;
        }
    }

    Book *mergeSort(Book *head)
    {
        if (!head || !head->next)
            return head;

        Book *slow = head;
        Book *fast = head->next;

        while (fast && fast->next)
        {
            slow = slow->next;
            fast = fast->next->next;
        }

        Book *mid = slow->next;
        slow->next = nullptr;

        return merge(mergeSort(head), mergeSort(mid));
    }

    Book *merge(Book *left, Book *right)
    {
        Book dummy("", "", "", 0, 0);
        Book *curr = &dummy;

        while (left && right)
        {
            if (left->title < right->title)
            {
                curr->next = left;
                left = left->next;
            }
            else
            {
                curr->next = right;
                right = right->next;
            }
            curr = curr->next;
        }

        curr->next = left ? left : right;
        return dummy.next;
    }

    Book *quickSort(Book *start, bool byTitle)
    {
        if (!start || !start->next)
            return start;

        Book *pivot = start;
        Book *left = nullptr;
        Book *right = nullptr;
        Book *curr = start->next;

        while (curr)
        {
            Book *next = curr->next;
            if ((byTitle && curr->title > pivot->title) ||
                (!byTitle && curr->ISBN < pivot->ISBN))
            {
                curr->next = left;
                left = curr;
            }
            else
            {
                curr->next = right;
                right = curr;
            }
            curr = next;
        }

        left = quickSort(left, byTitle);
        right = quickSort(right, byTitle);

        pivot->next = right;
        if (!left)
            return pivot;

        Book *temp = left;
        while (temp->next)
            temp = temp->next;
        temp->next = pivot;
        return left;
    }
};

class OrderManager
{
public:
    Order *head;

    OrderManager() : head(nullptr) { loadOrders(); }
    ~OrderManager() { clear(); }

    void addOrder(Order *newOrder)
    {
        if (!head)
            head = newOrder;
        else
        {
            Order *temp = head;
            while (temp->next)
                temp = temp->next;
            temp->next = newOrder;
        }
        saveOrders();
    }

    void displayOrders() const
    {
        Order *current = head;
        while (current)
        {
            current->display();
            current = current->next;
        }
    }

    void deleteOrder(const string &orderId)
    {
        Order *curr = head, *prev = nullptr;
        while (curr)
        {
            if (curr->orderId == orderId)
            {
                if (prev)
                    prev->next = curr->next;
                else
                    head = curr->next;
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

    void saveOrders()
    {
        ofstream file(ORDER_FILE);
        Order *current = head;
        while (current)
        {
            file << current->orderId << "," << current->customerId << ",";
            for (size_t i = 0; i < current->bookISBNs.size(); ++i)
            {
                file << current->bookISBNs[i];
                if (i != current->bookISBNs.size() - 1)
                    file << "|";
            }
            file << ";" << current->total << "," << current->date.toString() << "\n";
            current = current->next;
        }
    }

private:
    void loadOrders()
    {
        ifstream file(ORDER_FILE);
        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            string orderId, customerId, dateStr, totalStr;
            vector<string> isbns;

            getline(ss, orderId, ',');
            getline(ss, customerId, ',');

            string isbnList;
            getline(ss, isbnList, ';');
            stringstream iss(isbnList);
            string isbn;
            while (getline(iss, isbn, '|'))
            {
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

    void clear()
    {
        while (head)
        {
            Order *temp = head;
            head = head->next;
            delete temp;
        }
    }
};

class CustomerManager
{
public:
    Customer *head;
    Admin *admin;

    CustomerManager() : head(nullptr), admin(nullptr)
    {
        loadCustomers();
        loadAdmin();
    }

    ~CustomerManager()
    {
        clearCustomers();
        delete admin;
    }

    Customer *registerCustomer()
    {
        Customer *newCustomer = new Customer();
        cout << "Enter name: ";
        cin.ignore();
        getline(cin, newCustomer->name);
        cout << "Enter email: ";
        cin >> newCustomer->email;
        cout << "Enter password: ";
        cin >> newCustomer->password;

        if (!head)
            head = newCustomer;
        else
        {
            Customer *temp = head;
            while (temp->next)
                temp = temp->next;
            temp->next = newCustomer;
        }
        saveCustomers();
        return newCustomer;
    }

    Customer *login(const string &email, const string &password)
    {
        Customer *current = head;
        while (current)
        {
            if (current->email == email && current->password == password)
            {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void deleteCustomer(Customer *customer)
    {
        Customer *curr = head, *prev = nullptr;
        while (curr)
        {
            if (curr == customer)
            {
                if (prev)
                    prev->next = curr->next;
                else
                    head = curr->next;
                delete curr;
                cout << GREEN << "Account deleted!\n";
                saveCustomers();
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }

    void saveCustomers()
    {
        ofstream file(CUSTOMER_FILE);
        Customer *current = head;
        while (current)
        {
            file << current->id << "," << current->name << ","
                 << current->email << "," << current->password << "\n";
            current = current->next;
        }
    }

private:
    void loadCustomers()
    {
        ifstream file(CUSTOMER_FILE);
        string line;
        while (getline(file, line))
        {
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

            if (!head)
                head = newCustomer;
            else
            {
                Customer *temp = head;
                while (temp->next)
                    temp = temp->next;
                temp->next = newCustomer;
            }
        }
    }

    void loadAdmin()
    {
        ifstream file(ADMIN_FILE);
        string line;
        if (getline(file, line))
        {
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

    void clearCustomers()
    {
        while (head)
        {
            Customer *temp = head;
            head = head->next;
            delete temp;
        }
    }
};

class BookstoreApp
{
    BookManager books;
    CustomerManager customers;
    OrderManager orders;

public:
    void run()
    {
        srand(time(0));
        showWelcome();
        int role = getRole();

        if (role == 1)
            customerFlow();
        else if (role == 2)
            adminFlow();
        else
            cout << RED << "Invalid selection!\n";
    }

private:
    void searchBookByISBN()
    {
        string isbn;
        cout << "Enter ISBN to search (0 to cancel): ";
        cin >> isbn;
        if (isbn == "0")
            return;

        vector<Book *> booksVec = books.getBooksVector();
        books.bubbleSortByISBN(booksVec);
        Book *foundBook = books.binarySearchByISBN(booksVec, isbn);

        if (foundBook)
        {
            foundBook->display();
        }
        else
        {
            cout << RED << "Book not found!\n";
        }
    }
    void showWelcome()
    {
        cout << YELLOW << "********************************************************************\n"
             << "--------------------------------------------------------------------\n"
             << "***************WELCOME TO ONLINE BOOK SHOPPING SYSTEM***************\n"
             << "--------------------------------------------------------------------\n"
             << "********************************************************************\n"
             << RESET;
    }

    int getRole()
    {
        int role;
        cout << "\n1. Customer\n2. Admin\nChoose Role: ";
        cin >> role;
        return role;
    }

    void customerFlow()
    {
        int choice;
        do
        {
            cout << YELLOW << "\n1. Register\n2. Login\n3. Exit\nChoice: ";
            cin >> choice;

            if (choice == 1)
            {
                Customer *customer = customers.registerCustomer();
                customerMenu(customer);
            }
            else if (choice == 2)
            {
                string email, password;
                cout << "Email: ";
                cin >> email;
                cout << "Password: ";
                cin >> password;

                Customer *customer = customers.login(email, password);
                if (customer)
                {
                    cout << GREEN << "Login successful!\n";
                    customerMenu(customer);
                }
                else
                {
                    cout << RED << "Invalid credentials!\n";
                }
            }
        } while (choice != 3);
    }

    void customerMenu(Customer *customer)
    {
        int choice;
        do
        {
            cout << YELLOW << "\nCustomer Menu\n"
                 << "1. View Profile\n"
                 << "2. View Books\n"
                 << "3. View Order History\n"
                 << "4. Place Order\n"
                 << "5. Edit Profile\n"
                 << "6. Delete Account\n"
                 << "7. Logout\nChoice: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
                customer->display();
                break;
            case 2:
                displayBooks();
                searchBookByISBN();
                break;
            case 3:
                displayOrderHistory(customer);
                break;
            case 4:
                placeOrder(customer);
                break;
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

    void adminFlow()
    {
        string email, password;
        cout << "Admin Email: ";
        cin >> email;
        cout << "Password: ";
        cin >> password;

        if (customers.admin && customers.admin->email == email &&
            customers.admin->password == password)
        {
            adminMenu();
        }
        else
        {
            cout << RED << "Admin login failed!\n";
        }
    }

    void adminMenu()
    {
        int choice;
        do
        {
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

            switch (choice)
            {
            case 1:
                displayCustomers();
                break;
            case 2:
                orders.displayOrders();
                break;
            case 3:
                displayBooks();
                searchBookByISBN();
                break;
            case 4:
                addBook();
                break;
            case 5:
                editBook();
                break;
            case 6:
                deleteBook();
                break;
            case 7:
                generateReport();
                break;
            }
        } while (choice != 8);
    }

    void displayBooks()
    {
        cout << "\nBooks:\n";
        books.displayBooks(); // Original list

        // Optional: Show sorted versions without modifying the original list
        cout << "Choose sorting algorithm (0 to skip):\n"
             << "1. Merge Sort by Title (A-Z)\n"
             << "2. Quick Sort by Title (Z-A)\nChoice: ";
        int sortChoice;
        cin >> sortChoice;

        if (sortChoice == 1 || sortChoice == 2)
        {
            // Create a temporary sorted list for display
            BookManager tempBooks;
            Book *current = books.head;
            while (current)
            {
                tempBooks.addBook(new Book(current->ISBN, current->title, current->author, current->price, current->stock));
                current = current->next;
            }
            if (sortChoice == 1)
                tempBooks.mergeSortByTitle();
            else
                tempBooks.quickSortByTitle();
            tempBooks.displayBooks();
        }
    }

    void displayOrderHistory(Customer *customer)
    {
        cout << YELLOW << "\nOrder History:\n";
        Order *current = customer->orderHistory;
        while (current)
        {
            current->display();
            current = current->next;
        }
    }

    void displayCustomers()
    {
        cout << YELLOW << "\nRegistered Customers:\n";
        Customer *current = customers.head;
        while (current)
        {
            current->display();
            current = current->next;
        }
    }

    Date getCurrentDate()
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        return {ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year};
    }

void placeOrder(Customer *customer) {
    vector<string> isbns;
    double total = 0;

    // Get sorted books by ISBN for efficient searching
    vector<Book*> booksVec = books.getBooksVector();
    books.bubbleSortByISBN(booksVec);

    cout << "\nAvailable Books:\n";
    books.displayBooks();
    
    cout << "Enter ISBNs to purchase (separated by spaces, 0 to finish):\n";
    string isbn;
    while (true) {
        cin >> isbn;
        if (isbn == "0") break;
        
        Book *book = books.binarySearchByISBN(booksVec, isbn);
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
    void addBook()
    {
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

    void editBook()
    {
        string ISBN;
        cout << "Enter ISBN to edit: ";
        cin >> ISBN;

        Book *book = books.findBook(ISBN);
        if (book)
        {
            cout << "Enter new price (" << book->price << "): ";
            cin >> book->price;
            cout << "Enter new stock (" << book->stock << "): ";
            cin >> book->stock;
            books.saveToFile();
            cout << GREEN << "Book updated!\n";
        }
        else
        {
            cout << RED << "Book not found!\n";
        }
    }

    void deleteBook()
    {
        string ISBN;
        cout << "Enter ISBN to delete: ";
        cin >> ISBN;
        books.deleteBook(ISBN);
    }

    void generateReport()
    {
        ofstream report(SALES_FILE);
        Order *current = orders.head;
        while (current)
        {
            report << "Order ID: " << current->orderId
                   << ", Total: $" << current->total
                   << ", Date: " << current->date.toString() << "\n";
            current = current->next;
        }
        cout << GREEN << "Sales report generated!\n";
    }
};

int main()
{
    BookstoreApp app;
    app.run();
    return 0;
}