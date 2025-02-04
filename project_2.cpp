#include <iostream> // Input/Output Library
#include <string> // String Library
#include <vector> // Vector Library
#include <fstream> // File Stream Library
#include <sstream> // String Stream Library
#include <ctime> // Time Library
#include <cstdlib> // Standard Library
#include <iomanip> // Input/Output Manipulation Library

using namespace std;

//Define the ANSI escape codes for text colors
#define YELLOW "\033[33m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define RESET "\033[0m"

//Store file names for the cutsomer, book, order, admin and sales report files
const string CUSTOMER_FILE = "customers.txt";
const string BOOK_FILE = "books.txt";
const string ORDER_FILE = "orders.txt";
const string ADMIN_FILE = "admins.txt";
const string SALES_FILE = "sales_report.txt";

//Define the Date struct to store the day, month and year of the order
struct Date
{
    int day, month, year;

//Define the constructor of the Date struct
    string toString() const
    {
        //Return the date as a string in the format day/month/year
        return to_string(day) + "/" + to_string(month) + "/" + to_string(year);
    }

    //Define the comparison operators for the Date struct
    bool operator>(const Date &other) const
    {
        /*If the year is greater than the other year, return true, otherwise if the month is greater than the other month, 
        return true, otherwise if the day is greater than the other day, return true*/
        if (year != other.year)
            return year > other.year;
        if (month != other.month)
            return month > other.month;
        return day > other.day;
    }
};

//Define the Book class to store the ISBN, title, author, price and stock of the book
class Book
{
public:
    string ISBN;
    string title;
    string author;
    double price;
    int stock;

    //Define the next book in the linked list
    Book *next;

    //Define the constructor of the Book class
    Book(string isbn, string t, string a, double p, int s)
        : ISBN(isbn), title(t), author(a), price(p), stock(s), next(nullptr) {}

    //Define the display function to display the book
    void display() const
    {
        //Display the ISBN, title, author, price and stock of the book
        cout << YELLOW << "ISBN: " << ISBN << "\nTitle: " << title
             << "\nAuthor: " << author << "\nPrice: $" << fixed << setprecision(2) << price
             << "\nStock: " << stock << RESET << "\n\n";
    }
};

//Define the Order class to store the order ID, customer ID, book ISBNs, total price and date of the order
class Order
{
public:
//Define the order ID, customer ID, book ISBNs, total price, date and next order
    string orderId;
    string customerId;
    vector<string> bookISBNs;
    double total;

    //Define the order date and the next order
    Date date;

    //Define the next order in the linked
    Order *next;

    //Define the constructor of the Order class
    Order(string oid, string cid, vector<string> isbns, double t, Date d)

        //Set the order ID, customer ID, book ISBNs, total price, date and next order
        : orderId(oid), customerId(cid), bookISBNs(isbns), total(t), date(d), next(nullptr) {}

    //Define the display function to display the order
    void display() const
    {
        cout << YELLOW << "Order ID: " << orderId << "\nCustomer ID: " << customerId
             << "\nTotal: $" << fixed << setprecision(2) << total << "\nDate: " << date.toString()
             << "\nBooks: ";

             //Display the book ISBNs
        for (const auto &isbn : bookISBNs)
            cout << isbn << " ";
        cout << RESET << "\n\n";
    }
};

//Define the Customer class to store the customer ID, name, email, password, order history and next customer
class Customer
{
public:
    string id;
    string name;
    string email;
    string password;
    Order *orderHistory;
    Customer *next;

    Customer() : orderHistory(nullptr), next(nullptr)
    {
        id = generateID();
    }

    static string generateID()
    {
        return to_string(rand() % 90000 + 10000);
    }

    void display() const
    {
        cout << GREEN << "ID: " << id << "\nName: " << name
             << "\nEmail: " << email << RESET << "\n\n";
    }

    void addOrder(Order *order)
    {
        if (!orderHistory)
            orderHistory = order;
        else
        {
            Order *temp = orderHistory;
            while (temp->next)
                temp = temp->next;
            temp->next = order;
        }
    }

    void updateProfile()
    {
        cout << "Enter new name (" << name << "): ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter new email (" << email << "): ";
        cin >> email;
    }
};

//Define the Admin class which is a subclass of the Customer class
class Admin : public Customer // Inheritance
{
public:
    Admin()
    {
        id = "ADM-" + generateID();
    }

    void display() const
    {
        cout << RED << "Admin ID: " << id << "\nName: " << name
             << "\nEmail: " << email << RESET << "\n\n";
    }
};

//Define the BookManager class to manage the books in the system
class BookManager
{
public:
//Define the head of the linked list of books
    Book *head;

//Define the constructor and destructor of the BookManager class
    BookManager() : head(nullptr) { loadFromFile(); }
    ~BookManager() { clear(); }

//Define the addBook function to add a new book to the linked list
    void addBook(Book *newBook)
    {
        //If the head is null, set the head to the new book
        if (!head)
            head = newBook;
        else
        {
            Book *temp = head;
            while (temp->next)
                temp = temp->next;
            temp->next = newBook;
        }
    }

//Define the displayBooks function to display all the books in the linked list
    void displayBooks() const
    {
        //Set the current book to the head of the linked list
        Book *current = head;
        while (current)
        {
            //Display the current book
            current->display();
            current = current->next;
        }
    }

//Define the findBook function to find a book by its ISBN
    Book *findBook(const string &ISBN)
    {
        //Set the current book to the head of the linked list
        Book *current = head;
        while (current)
        {
            //If the current book's ISBN matches the given ISBN, return the current book
            if (current->ISBN == ISBN)
                return current;
            current = current->next;
        }
        return nullptr;
    }

//Define the deleteBook function to delete a book by its ISBN
    void deleteBook(const string &ISBN)
    {
        //Set the current book to the head of the linked list and the previous book to null
        Book *curr = head, *prev = nullptr;
        while (curr)
        {
            //If the current book's ISBN matches the given ISBN
            if (curr->ISBN == ISBN)
            {
                //If the previous book is not null, set the previous book's next to the current book's next
                if (prev)
                    prev->next = curr->next;
                else
                    head = curr->next;
                delete curr;
                cout << GREEN << "Book deleted!\n";
                saveToFile();
                return;
            }
            //Set the previous book to the current book and the current book to the next book
            prev = curr;
            curr = curr->next;
        }
        cout << RED << "Book not found!\n";
    }

//Define the saveToFile function to save the books to the books.txt file
    void saveToFile()
    {
        //Open the books.txt file for writing
        ofstream file(BOOK_FILE);
        Book *current = head;
        while (current)
        {
            //Write the current book's ISBN, title, author, price and stock to the file
            file << current->ISBN << "," << current->title << ","
                 << current->author << "," << current->price << ","
                 << current->stock << "\n";
            current = current->next;
        }
    }

//Define the mergeSortByTitle function to sort the books by title using merge sort
    void mergeSortByTitle()
    {
        //Set the head to the result of the mergeSort function
        head = mergeSort(head);
        cout << GREEN << "Sorted (A-Z) using Merge Sort!\n";
    }

//Define the quickSortByTitle function to sort the books by title using quick sort
    void quickSortByTitle()
    {
        //Set the head to the result of the quickSort function
        head = quickSort(head, true);
        cout << GREEN << "Sorted (Z-A) using Quick Sort!\n";
    }

//Define the getBooksVector function to get a vector of books from the linked list
     vector<Book*> getBooksVector() {
        vector<Book*> booksVec;
        Book* current = head;
        while (current) {
            //Add the current book to the vector and set the current book to the next book
            booksVec.push_back(current);
            current = current->next;
        }
        return booksVec;
    }

//Define the bubbleSortByISBN function to sort the books by ISBN using bubble sort
    void bubbleSortByISBN(vector<Book*>& booksVec) {
        int n = booksVec.size();
        bool swapped;
        for (int i = 0; i < n-1; i++) {
            //Set swapped to false
            swapped = false;
            for (int j = 0; j < n-i-1; j++) {
                //If the current book's ISBN is greater than the next book's ISBN, swap the books
                if (booksVec[j]->ISBN > booksVec[j+1]->ISBN) {
                    swap(booksVec[j], booksVec[j+1]);
                    swapped = true;
                }
            }
            if (!swapped) break;
        }
    }

//Define the binarySearchByISBN function to search for a book by its ISBN using binary search
    Book* binarySearchByISBN(const vector<Book*>& booksVec, const string& ISBN) {
        int left = 0;
        int right = booksVec.size() - 1;
        while (left <= right) {
            //Set the mid index to the average of the left and right indices
            int mid = left + (right - left) / 2;
            if (booksVec[mid]->ISBN == ISBN) {
                //Return the book if the current book's ISBN matches the given ISBN
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
//Define the loadFromFile function to load the books from the books.txt file
    void loadFromFile()
    {
        //Open the books.txt file for reading
        ifstream file(BOOK_FILE);
        string line;
        //Read each line from the file
        while (getline(file, line))
        {
            //Create a string stream from the line
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
//Define the clear function to clear the linked list of books
    void clear()
    {
        //Set the current book to the head of the linked list
        while (head)
        
        {
            //Set the temporary book to the current book and the current book to the next book
            Book *temp = head;
            head = head->next;
            delete temp;
        }
    }

//Define the mergeSort function to sort the books by title using merge sort
    Book *mergeSort(Book *head)
    //Define the quickSort function to sort the books by title using quick sort
    {
        //If the head is null or the next book is null, return the head
        if (!head || !head->next)
            return head;

        Book *slow = head;
        Book *fast = head->next;

        while (fast && fast->next)
        
        {
            //Set the slow book to the next book and the fast book to the next next book
            slow = slow->next;
            fast = fast->next->next;
        }

//Set the mid book to the slow book's next and the slow book's next to null
        Book *mid = slow->next;
        slow->next = nullptr;

//Return the result of merging the mergeSort of the head and the mergeSort of the mid
        return merge(mergeSort(head), mergeSort(mid));
    }

//Define the merge function to merge two sorted linked lists
    Book *merge(Book *left, Book *right)
    {
        //Create a dummy book and set the current book to the dummy book
        Book dummy("", "", "", 0, 0);
        Book *curr = &dummy;

        while (left && right)
        {
            //If the left book's title is less than the right book's title, set the current book's next to the left book and the left book to the next book
            if (left->title < right->title)
            {
                curr->next = left;
                left = left->next;
            }
            else
            //Set the current book's next to the right book and the right book to the next book
            {
                curr->next = right;
                right = right->next;
            }
            //Set the current book to the next book
            {
                curr->next = right;
                right = right->next;
            }
            //Set the current book to the next book
            curr = curr->next;
        }

//Set the current book's next to the left book if the left book is not null, otherwise set it to the right book
        curr->next = left ? left : right;
        return dummy.next;
    }

//Define the quickSort function to sort the books by title using quick sort
    Book *quickSort(Book *start, bool byTitle)
    {
        //If the start book is null or the next book is null, return the start book
        if (!start || !start->next)
            return start;

//Set the pivot book to the start book, the left book to null, the right book to null and the current book to the start book's next
        Book *pivot = start;
        Book *left = nullptr;
        Book *right = nullptr;
        Book *curr = start->next;

//While the current book is not null
        while (curr)
        {
            //Set the next book to the current book's next
            Book *next = curr->next;
            if ((byTitle && curr->title > pivot->title) ||
                (!byTitle && curr->ISBN < pivot->ISBN))
            {
                //
                curr->next = left;
                left = curr;
            }
            //Set the current book to the next book
            else
            
            {
                curr->next = right;
                right = curr;
            }
            
            curr = next;
        }

//Set the left book to the result of the quickSort function with the left book and the byTitle flag
        left = quickSort(left, byTitle);
        right = quickSort(right, byTitle);

//Set the pivot book's next to the right book
        pivot->next = right;
        if (!left)
            return pivot;

//Set the current book to the left book
        Book *temp = left;
        while (temp->next)
            temp = temp->next;
        temp->next = pivot;
        return left;
    }
};

//Define the OrderManager class to manage the orders in the system
class OrderManager
{
public:
//Define the head of the linked list of orders
    Order *head;

//Define the constructor and destructor of the OrderManager class
    OrderManager() : head(nullptr) { loadOrders(); }
    ~OrderManager() { clear(); }

//Define the addOrder function to add a new order to the linked list
    void addOrder(Order *newOrder)
    {
        //If the head is null, set the head to the new order
        if (!head)
            head = newOrder;
        else
        //Set the temporary order to the head of the linked list
        {
            
            Order *temp = head;
            while (temp->next)
                temp = temp->next;
            temp->next = newOrder;
        }
        //Save the orders to the orders.txt file
        saveOrders();
    }

//Define the displayOrders function to display all the orders in the linked list
    void displayOrders() const
    {
        //Set the current order to the head of the linked list
        Order *current = head;
        while (current)
        {
            //Display the current order
            current->display();
            current = current->next;
        }
    }

//
    void deleteOrder(const string &orderId)
    {
        //Set the current order to the head of the linked list and the previous order to null
        Order *curr = head, *prev = nullptr;
        while (curr)
        {
            //If the current order's ID matches the given order ID
            if (curr->orderId == orderId)
            {
                //If the previous order is not null, set the previous order's next to the current order's next
                if (prev)
                    prev->next = curr->next;
                else
                    head = curr->next;
                delete curr;
                //Display a message that the order was deleted
                cout << GREEN << "Order deleted!\n";
                saveOrders();
                return;
            }
            //Set the previous order to the current order and the current order to the next order
            prev = curr;
            curr = curr->next;
        }
        cout << RED << "Order not found!\n";
    }

//Define the saveOrders function to save the orders to the orders.txt file
    void saveOrders()
    {
        //Open the orders.txt file for writing
        ofstream file(ORDER_FILE);
        Order *current = head;
        while (current)
        {
            //Write the current order's ID, customer ID, book ISBNs, total price and date to the file
            file << current->orderId << "," << current->customerId << ",";
            for (size_t i = 0; i < current->bookISBNs.size(); ++i)
            {
                //Write the current book's ISBN to the file
                file << current->bookISBNs[i];
                //If the current book is not the last book, write a pipe character to the file
                if (i != current->bookISBNs.size() - 1)
                    file << "|";
            }
            //Write the current order's total price and date to the file
            file << ";" << current->total << "," << current->date.toString() << "\n";
            current = current->next;
        }
    }


private:
//Define the loadOrders function to load the orders from the orders.txt file
    void loadOrders()
    {
        //Open the orders.txt file for reading
        ifstream file(ORDER_FILE);
        string line;
        while (getline(file, line))
        {
            //Create a string stream from the line
            stringstream ss(line);
            string orderId, customerId, dateStr, totalStr;
            vector<string> isbns;

            //Read the order ID, customer ID, book ISBNs, total price and date from the file

            getline(ss, orderId, ',');
            getline(ss, customerId, ',');

            //

            string isbnList;
            getline(ss, isbnList, ';');
            stringstream iss(isbnList);
            string isbn;

            //Read the book ISBNs from the file
            while (getline(iss, isbn, '|'))
            {
                isbns.push_back(isbn);
            }

            //Read the total price and date from the file

            getline(ss, totalStr, ',');
            getline(ss, dateStr, ',');

            //Create a date object from the date string

            Date orderDate;
            stringstream dss(dateStr);
            string part;
            getline(dss, part, '/');
            orderDate.day = stoi(part);
            getline(dss, part, '/');
            orderDate.month = stoi(part);
            getline(dss, part, '/');
            orderDate.year = stoi(part);

            //Add the order to the linked list of orders

            addOrder(new Order(orderId, customerId, isbns, stod(totalStr), orderDate));
        }
    }


//Define the clear function to clear the linked list of orders
    void clear()
    {

        //Set the current order to the head of the linked list
        while (head)
        {

            //Set the temporary order to the current order and the current order to the next order
            Order *temp = head;

            //Set the head to the next order
            head = head->next;

            //Delete the temporary order
            delete temp;
        }
    }
};


//Define the CustomerManager class to manage the customers in the system
class CustomerManager
{
public:

//Define the head of the linked list of customers and the admin
    Customer *head;
    Admin *admin;


//Define the constructor and destructor of the CustomerManager class
    CustomerManager() : head(nullptr), admin(nullptr)
    {

        //Load the customers and the admin from the files
        loadCustomers();
        loadAdmin();
    }

//Define the destructor of the CustomerManager class
    ~CustomerManager()
    {

        //Clear the customers and the admin
        clearCustomers();
        delete admin;
    }

//Define the registerCustomer function to register a new customer
    Customer *registerCustomer()
    {
        //Create a new customer and set the name, email and password
        Customer *newCustomer = new Customer();
        cout << "Enter name: ";
        cin.ignore();

        //Read the name, email and password from the user
        getline(cin, newCustomer->name);
        cout << "Enter email: ";
        cin >> newCustomer->email;
        cout << "Enter password: ";
        cin >> newCustomer->password;

//If the head is null, set the head to the new customer, otherwise add the new customer to the end of the linked list
        if (!head)
            head = newCustomer;
        else
        //Set the temporary customer to the head of the linked list
        {

            //Add the new customer to the end of the linked list
            Customer *temp = head;

            //Set the temporary customer to the next customer
            while (temp->next)
                temp = temp->next;
            temp->next = newCustomer;
        }
        //Save the customers to the customers.txt file
        saveCustomers();
        return newCustomer;
    }

//Define the login function to login a customer by email and password
    Customer *login(const string &email, const string &password)
    {
        //Set the current customer to the head of the linked list
        Customer *current = head;
        while (current)
        {
            //If the current customer's email and password match the given email and password, return the current customer
            if (current->email == email && current->password == password)
            {
                return current;
            }
            current = current->next;
        }

        //Return null if the customer is not found
        return nullptr;
    }

//Define the deleteCustomer function to delete a customer
    void deleteCustomer(Customer *customer)
    {

        //Set the current customer to the head of the linked list and the previous customer to null
        Customer *curr = head, *prev = nullptr;
        while (curr)
        {
            if (curr == customer)
            {
                //If the previous customer is not null, set the previous customer's next to the current customer's next
                if (prev)
                    prev->next = curr->next;
                else
                    head = curr->next;
                delete curr;
                cout << GREEN << "Account deleted!\n";
                saveCustomers();
                return;
            }
            //Set the previous customer to the current customer and the current customer to the next customer
            prev = curr;
            curr = curr->next;
        }
    }

//Define the saveCustomers function to save the customers to the customers.txt file
    void saveCustomers()
    {
        //Open the customers.txt file for writing
        ofstream file(CUSTOMER_FILE);
        //Set the current customer to the head of the linked list
        Customer *current = head;
        //Write the customers to the file
        while (current)
        {
            //Write the current customer's ID, name, email and password to the file
            file << current->id << "," << current->name << ","
                 << current->email << "," << current->password << "\n";
            current = current->next;
        }
    }

private:
//Define the loadCustomers function to load the customers from the customers.txt file
    void loadCustomers()
    {
        //Open the customers.txt file for reading
        ifstream file(CUSTOMER_FILE);
        string line;
        //Read each line from the file
        while (getline(file, line))
        //Create a string stream from the line
        {
            //Create a string stream from the line
            stringstream ss(line);
            string id, name, email, password;
            //Read the customer's ID, name, email and password from the file
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, email, ',');
            getline(ss, password, ',');


            //Create a new customer and set the ID, name, email and password
            Customer *newCustomer = new Customer();
            newCustomer->id = id;
            newCustomer->name = name;
            newCustomer->email = email;
            newCustomer->password = password;

//If the head is null, set the head to the new customer, otherwise add the new customer to the end of the linked list
            if (!head)
                head = newCustomer;
            else
            {
                //Set the temporary customer to the head of the linked list
                Customer *temp = head;

                //Add the new customer to the end of the linked list
                while (temp->next)
                //Set the temporary customer to the next customer
                    temp = temp->next;
                temp->next = newCustomer;
            }
        }
    }

    void loadAdmin()
    {
        //Open the admins.txt file for reading
        ifstream file(ADMIN_FILE);
        string line;
        //Read the first line from the file
        if (getline(file, line))
        {
            //Create a string stream from the line
            stringstream ss(line);
            //Read the admin's ID, name, email and password from the file
            string id, name, email, password;
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, email, ',');
            getline(ss, password, ',');

            //Create a new admin and set the ID, name, email and password
            admin = new Admin();

            //Set the admin's ID, name, email and password
            admin->id = id;
            admin->name = name;
            admin->email = email;
            admin->password = password;
        }
    }

    void clearCustomers()
    {
        //Set the current customer to the head of the linked list
        while (head)
        {
            //Set the temporary customer to the current customer and the current customer to the next customer
            Customer *temp = head;
            head = head->next;
            delete temp;
        }
    }
};

class BookstoreApp
{
    //Define the BookManager, CustomerManager and OrderManager objects
    BookManager books;
    CustomerManager customers;
    OrderManager orders;

public:
    void run()
    //Define the run function to run the bookstore application
    {
        //Seed the random number generator
        srand(time(0));
        showWelcome();
        //Get the role of the user
        int role = getRole();

        //If the role is 1, run the customer flow, if the role is 2, run the admin flow, otherwise display an error message
        if (role == 1)
            customerFlow();
        else if (role == 2)
            adminFlow();
        else
            cout << RED << "Invalid selection!\n";
    }

private:
    //Define the searchBookByISBN function to search for a book by its ISBN
    void searchBookByISBN()
    {

        //Read the ISBN from the user
        string isbn;
        cout << "Enter ISBN to search (0 to cancel): ";
        cin >> isbn;
        //If the ISBN is 0, return
        if (isbn == "0")
            return;
    //Get the sorted books by ISBN for efficient searching
        vector<Book *> booksVec = books.getBooksVector();

        //Sort the books by ISBN using bubble sort
        books.bubbleSortByISBN(booksVec);

        //Search for the book by its ISBN using binary search
        Book *foundBook = books.binarySearchByISBN(booksVec, isbn);
        
        //If the book is found, display the book, otherwise display an error message
        if (foundBook)
        {
            foundBook->display();
        }
        else
        {
            cout << RED << "Book not found!\n";
        }
    }

    //Define the showWelcome function to display the welcome message
    void showWelcome()
    {
        cout << YELLOW << "************************************************\n"
             << "------------------------------------------------\n"
             << "*****WELCOME TO ONLINE BOOK SHOPPING SYSTEM*****\n"
             << "------------------------------------------------\n"
             << "************************************************\n"
             << RESET;
    }

    int getRole()
    //Define the getRole function to get the role of the user
    {
        int role;

        //Read the role from the user
        cout << "\n1. Customer\n2. Admin\nChoose Role: ";
        cin >> role;
        return role;
    }

    void customerFlow()
    //Define the customerFlow function to run the customer flow
    {
        //Display the customer menu
        int choice;
        do
        {
            //Read the choice from the user
            cout << YELLOW << "\n1. Register\n2. Login\n3. Exit\nChoice: ";
            cin >> choice;

            //If the choice is 1, register a new customer, if the choice is 2, login the customer, otherwise exit

            if (choice == 1)
            {
                Customer *customer = customers.registerCustomer();
                customerMenu(customer);
            }
            else if (choice == 2)
            {
                //Read the email and password from the user
                string email, password;
                cout << "Email: ";
                cin >> email;
                cout << "Password: ";
                cin >> password;

                //Login the customer using the email and password
                Customer *customer = customers.login(email, password);
                if (customer)
                {

                    //Display a message that the login was successful and show the customer menu
                    cout << GREEN << "Login successful!\n";
                    customerMenu(customer);
                }
                else
                {

                    //Display a message that the login failed
                    cout << RED << "Invalid credentials!\n";
                }
            }

                //Display a message that the customer flow is exiting
        } while (choice != 3);
    }

    //Define the customerMenu function to display the customer menu
    void customerMenu(Customer *customer)
    {

        //Display the customer menu
        int choice;
        do
        {

            //Read the choice from the user
            cout << YELLOW << "\nCustomer Menu\n"
                 << "1. View Profile\n"
                 << "2. View Books\n"
                 << "3. View Order History\n"
                 << "4. Place Order\n"
                 << "5. Edit Profile\n"
                 << "6. Delete Account\n"
                 << "7. Logout\nChoice: ";
            cin >> choice;


            //If the choice is 1, display the customer's profile, if the choice is 2, display the books, if the choice is 3, display the order history, if the choice is 4, place an order, if the choice is 5, update the profile, if the choice is 6, delete the account, otherwise logout
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
            //Display a message that the customer is logging out
        } while (choice != 7);
    }

    //Define the adminFlow function to run the admin flow
    void adminFlow()
    {

        //Read the email and password from the user
        string email, password;
        cout << "Admin Email: ";
        cin >> email;
        cout << "Password: ";
        cin >> password;


        //If the admin's email and password match the given email and password, show the admin menu, otherwise display an error message
        if (customers.admin && customers.admin->email == email &&
            customers.admin->password == password)
        {
            //Display a message that the admin login was successful and show the admin menu
            adminMenu();
        }
        else
        //Display a message that the admin login failed
        {
            cout << RED << "Admin login failed!\n";
        }
    }

    //Define the adminMenu function to display the admin menu
    void adminMenu()
    {
        //Display the admin menu
        int choice;
        do
        //Read the choice from the user
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


/*If the choice is 1, display the customers, if the choice is 2, display the orders, if the choice is 3, 
display the books, if the choice is 4, add a book, if the choice is 5, edit a book, if the choice is 6, 
delete a book, if the choice is 7, generate a report, otherwise logout*/
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

            //Display a message that the admin is logging out
        } while (choice != 8);
    }

    void displayBooks()

    //Define the displayBooks function to display the books
    {
        //Display the books
        cout << "\nBooks:\n";
        books.displayBooks(); // Original list

        // Optional: Show sorted versions without modifying the original list
        cout << "Choose sorting algorithm (0 to skip):\n"
             << "1. Merge Sort by Title (A-Z)\n"
             << "2. Quick Sort by Title (Z-A)\nChoice: ";

             //Read the sorting choice from the user
        int sortChoice;
        cin >> sortChoice;

        //If the sort choice is 1 or 2, create a temporary sorted list for display
        if (sortChoice == 1 || sortChoice == 2)
        {
            // Create a temporary sorted list for display
            BookManager tempBooks;

            // Copy the books to the temporary list
            Book *current = books.head;
            while (current)
            {

                // Add the current book to the temporary list and set the current book to the next book
                tempBooks.addBook(new Book(current->ISBN, current->title, current->author, current->price, current->stock));
                // Set the current book to the next book
                current = current->next;
            }
            // Sort the temporary list by title using the chosen algorithm
            if (sortChoice == 1)

                //Merge sort by title
                tempBooks.mergeSortByTitle();
            else

            //Quick sort by title
                tempBooks.quickSortByTitle();

                //Display the sorted books
            tempBooks.displayBooks();
        }
    }

    //Define the displayOrderHistory function to display the order history of a customer
    void displayOrderHistory(Customer *customer)

    {

        //Display the order history of the customer
        cout << YELLOW << "\nOrder History:\n";

        //Set the current order to the order history of the customer
        Order *current = customer->orderHistory;
        while (current)
        {
            //Display the current order
            current->display();

            //Set the current order to the next order
            current = current->next;
        }
    }

    //Define the displayCustomers function to display the customers
    void displayCustomers()
    {
        //Display the customers
        cout << YELLOW << "\nRegistered Customers:\n";

        //Set the current customer to the head of the linked list
        Customer *current = customers.head;

        //Display the customers
        while (current)
        {

            //Display the current customer
            current->display();
            //Set the current customer to the next customer
            current = current->next;
        }
    }

    //Define the getCurrentDate function to get the current date
    Date getCurrentDate()
    {
        //Get the current date
        time_t now = time(0);

        //Create a local time object
        tm *ltm = localtime(&now);

        //Return the current date
        return {ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year};
    }

//Define the placeOrder function to place an order for a customer
void placeOrder(Customer *customer) {

    //Set the ISBNs and total price to empty and 0
    vector<string> isbns;

    //Set the total price to 0
    double total = 0;

    // Get sorted books by ISBN for efficient searching
    vector<Book*> booksVec = books.getBooksVector();

    // Sort the books by ISBN using bubble sort
    books.bubbleSortByISBN(booksVec);

    cout << "\nAvailable Books:\n";
    books.displayBooks();
    
    //Read the ISBNs to purchase from the user
    cout << "Enter ISBNs to purchase (separated by spaces, 0 to finish):\n";

    //Read the ISBNs from the user
    string isbn;
    while (true) {
        //Read the ISBN from the user
        cin >> isbn;

        //If the ISBN is 0, break
        if (isbn == "0") break;
        
        //Search for the book by its ISBN using binary search
        Book *book = books.binarySearchByISBN(booksVec, isbn);

        //If the book is found and in stock, add the book to the order
        if (book && book->stock > 0) {
            
            //Add the ISBN to the list of ISBNs
            isbns.push_back(isbn);

            //Add the price of the book to the total price and decrement the stock
            total += book->price;

            //Decrement the stock of the book
            book->stock--;
        } else {

            //Display an error message if the book is not found or out of stock
            cout << RED << "Invalid ISBN or out of stock!\n";
        }
    }


    //If the total price is 0, return
    if (!isbns.empty()) {

        //Create a new order and add it to the orders
        Order *newOrder = new Order(

            //Create a new order with a random order ID, the customer's ID, the list of ISBNs, the total price and the current date
            "ORD" + to_string(rand() % 9000 + 1000),
            customer->id,

            //Set the list of ISBNs, the total price and the current date
            isbns,
            total,

            //Set the current date
            getCurrentDate()
        );

        //Add the order to the orders and the customer's order history
        orders.addOrder(newOrder);

        //Add the order to the customer's order history
        customer->addOrder(newOrder);

        //Save the orders to the orders.txt file
        books.saveToFile();

        //Display a message that the order was placed successfully
        cout << GREEN << "Order placed successfully! Total: $" << total << "\n";
    }
}

    //Define the addBook function to add a new book
    void addBook()
    {
        //Read the book details from the user
        string ISBN, title, author;
        double price;
        int stock;

        //Read the book details from the user
        cout << "Enter ISBN: ";
        cin >> ISBN;

        //Check if the book already exists
        cout << "Enter title: ";
        cin.ignore();

        //Read the title, author, price and stock from the user
        getline(cin, title);
        cout << "Enter author: ";

        //Read the author from the user
        getline(cin, author);
        cout << "Enter price: ";
        cin >> price;

        //Read the price from the user
        cout << "Enter stock: ";
        cin >> stock;

        //Add the book to the list of books
        books.addBook(new Book(ISBN, title, author, price, stock));

        //Save the books to the books.txt file
        books.saveToFile();

        //Display a message that the book was added successfully
        cout << GREEN << "Book added successfully!\n";
    }


    //Define the editBook function to edit a book
    void editBook()
    {

        //Read the ISBN from the user
        string ISBN;
        cout << "Enter ISBN to edit: ";
        cin >> ISBN;

        //Search for the book by its ISBN
        Book *book = books.findBook(ISBN);
        if (book)
        {
            //Read the new details from the user
            cout << "Enter new price (" << book->price << "): ";
            cin >> book->price;

            //Read the new price from the user
            cout << "Enter new stock (" << book->stock << "): ";
            cin >> book->stock;

            //Read the new stock from the user
            books.saveToFile();
            cout << GREEN << "Book updated!\n";
        }
        else

        //Display an error message if the book is not found
        {
            cout << RED << "Book not found!\n";
        }
    }

    //Define the deleteBook function to delete a book
    void deleteBook()
    {
        //Read the ISBN from the user
        string ISBN;
        cout << "Enter ISBN to delete: ";
        cin >> ISBN;

        //Delete the book by its ISBN
        books.deleteBook(ISBN);
    }

    //Define the generateReport function to generate a sales report
    void generateReport()
    {
        //Generate a sales report
        ofstream report(SALES_FILE);

        //Set the current order to the head of the linked list
        Order *current = orders.head;
        while (current)
        {
            //Write the order ID, total price and date to the report
            report << "Order ID: " << current->orderId
                   << ", Total: $" << current->total
                   << ", Date: " << current->date.toString() << "\n";

                   //Set the current order to the next order
            current = current->next;
        }
        cout << GREEN << "Sales report generated!\n";
    }
};


    //Define the main function to run the bookstore application
int main()
{
    //Create a new BookstoreApp object and run the application
    BookstoreApp app;
    app.run();
    return 0;
}