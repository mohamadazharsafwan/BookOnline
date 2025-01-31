#include <iostream>
#include <vector>
#include <string>


#define YELLOW  "\033[33m"
#define RED     "\033[31m"

using namespace std;

// Structure for Book Record
struct Book {
    int id;
    string title;
    string author;
    double price;
};

// Structure for Order
struct Order {
    int bookId;
    string username;
    bool delivered;
};

// Structure for User (Customer/Staff) using Linked List
struct User {
    string username;
    string password;
    string role; // "customer" or "staff"
    User* next;
};

class UserList {
private:
    User* head;
public:
    UserList() : head(nullptr) {} // Constructor to initialize
    
    void registerUser(string username, string password, string role) {
        User* newUser = new User{username, password, role, nullptr};
        if (!head) {
            head = newUser;
        } else {
            User* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newUser;
        }
        cout << "User registered successfully!\n";
    }
    
    User* login(string username, string password) {
        User* temp = head;
        while (temp) {
            if (temp->username == username && temp->password == password) {
                cout << "Login successful!\n";
                return temp;
            }
            temp = temp->next;
        }
        cout << "Invalid credentials!\n";
        return nullptr;
    }
};

// Vector to store books
vector<Book> books = {
    {101, "The Girl Who Lived", "Christopher Greyson", "Thriller", "Barnes & Noble", "8.99", "159"},
    {102, "The Silent Corner", "Dean Koontz", "Thriller", "Books-A-Million", "8.99", "161"},
    {103, "The Whispering Room", "Dean Koontz", "Thriller", "Powell's Books", "8.99", "163"},
    {104, "The Crooked Staircase", "Dean Koontz", "Thriller", "Waterstones", "8.99", "165"},
    {105, "The Forbidden Door", "Dean Koontz", "Thriller", "Barnes & Noble", "8.99", "167"},
    {106, "The Night Window", "Dean Koontz", "Thriller", "Books-A-Million", "8.99", "169"},
    {107, "The Outsider", "Stephen King", "Horror", "Powell's Books", "8.99", "171"},
    {108, "The Institute", "Stephen King", "Horror", "Waterstones", "8.99", "173"},
    {109, "If It Bleeds", "Stephen King", "Horror", "Barnes & Noble", "8.99", "175"},
    {110, "The Stand", "Stephen King", "Horror", "Books-A-Million", "8.99", "177"},
    {111, "The Shining", "Stephen King", "Horror", "Powell's Books", "8.99", "179"},
    {112, "Doctor Sleep", "Stephen King", "Horror", "Waterstones", "8.99", "181"},
    {113, "Carrie", "Stephen King", "Horror", "Barnes & Noble", "8.99", "183"},
    {114, "Misery", "Stephen King", "Horror", "Books-A-Million", "8.99", "185"},
    {115, "It", "Stephen King", "Horror", "Powell's Books", "8.99", "187"},
    {116, "Pet Sematary", "Stephen King", "Horror", "Waterstones", "8.99", "189"},
    {117, "The Green Mile", "Stephen King", "Horror", "Barnes & Noble", "8.99", "191"},
    {118, "11/22/63", "Stephen King", "Science Fiction", "Books-A-Million", "8.99", "193"},
    {119, "Under the Dome", "Stephen King", "Science Fiction", "Powell's Books", "8.99", "195"},
    {120, "The Dark Tower", "Stephen King", "Fantasy", "Waterstones", "8.99", "197"},
    {121, "The Gunslinger", "Stephen King", "Fantasy", "Barnes & Noble", "8.99", "199"},
    {122, "The Drawing of the Three", "Stephen King", "Fantasy", "Books-A-Million", "8.99", "201"},
    {123, "The Waste Lands", "Stephen King", "Fantasy", "Powell's Books", "8.99", "203"},
    {124, "Wizard and Glass", "Stephen King", "Fantasy", "Waterstones", "8.99", "205"},
    {125, "Wolves of the Calla", "Stephen King", "Fantasy", "Barnes & Noble", "8.99", "207"},
    {126, "Song of Susannah", "Stephen King", "Fantasy", "Books-A-Million", "8.99", "209"},
    {127, "The Dark Tower", "Stephen King", "Fantasy", "Powell's Books", "8.99", "211"},
    {128, "The Wind Through the Keyhole", "Stephen King", "Fantasy", "Waterstones", "8.99", "213"},
    {129, "The Hunger Games", "Suzanne Collins", "Dystopian", "Barnes & Noble", "8.99", "215"},
    {130, "Catching Fire", "Suzanne Collins", "Dystopian", "Books-A-Million", "8.99", "217"},
    {131, "Mockingjay", "Suzanne Collins", "Dystopian", "Powell's Books", "8.99", "219"},
    {132, "Divergent", "Veronica Roth", "Dystopian", "Waterstones", "8.99", "221"},
    {133, "Insurgent", "Veronica Roth", "Dystopian", "Barnes & Noble", "8.99", "223"},
    {134, "Allegiant", "Veronica Roth", "Dystopian", "Books-A-Million", "8.99", "225"},
    {135, "Four", "Veronica Roth", "Dystopian", "Powell's Books", "8.99", "227"},
    {136, "The Maze Runner", "James Dashner", "Dystopian", "Waterstones", "8.99", "229"},
    {137, "The Scorch Trials", "James Dashner", "Dystopian", "Barnes & Noble", "8.99", "231"},
    {138, "The Death Cure", "James Dashner", "Dystopian", "Books-A-Million", "8.99", "233"},
    {139, "The Kill Order", "James Dashner", "Dystopian", "Powell's Books", "8.99", "235"},
    {140, "The Fever Code", "James Dashner", "Dystopian", "Waterstones", "8.99", "237"},
    {141, "The Giver", "Lois Lowry", "Dystopian", "Barnes & Noble", "8.99", "239"},
    {142, "Gathering Blue", "Lois Lowry", "Dystopian", "Books-A-Million", "8.99", "241"},
    {143, "Messenger", "Lois Lowry", "Dystopian", "Powell's Books", "8.99", "243"},
    {144, "Son", "Lois Lowry", "Dystopian", "Waterstones", "8.99", "245"},
    {145, "Ender's Game", "Orson Scott Card", "Science Fiction", "Barnes & Noble", "8.99", "247"},
    {146, "Speaker for the Dead", "Orson Scott Card", "Science Fiction", "Books-A-Million", "8.99", "249"},
    {147, "Xenocide", "Orson Scott Card", "Science Fiction", "Powell's Books", "8.99", "251"},
    {148, "Children of the Mind", "Orson Scott Card", "Science Fiction", "Waterstones", "8.99", "253"},
    {149, "Ender's Shadow", "Orson Scott Card", "Science Fiction", "Barnes & Noble", "8.99", "255"},
    {150, "Shadow of the Hegemon", "Orson Scott Card", "Science Fiction", "Books-A-Million", "8.99", "257"}
};

// Vector to store orders
vector<Order> orders;

UserList userList;

// Function to display books
void displayBooks() {
    cout << "\nBook List:\n";
    for (const auto &book : books) {
        cout << "ID: " << book.id << ", Title: " << book.title
             << ", Author: " << book.author << ", Price: $" << book.price << endl;
    }
}

// Function to add a book (Only staff can add books)
void addBook(string role) {
    if (role != "staff") {
        cout << "Permission Denied! Only staff can add books.\n";
        return;
    }
    Book newBook;
    cout << "Enter Book ID: ";
    cin >> newBook.id;
    cin.ignore();
    cout << "Enter Book Title: ";
    getline(cin, newBook.title);
    cout << "Enter Author: ";
    getline(cin, newBook.author);
    cout << "Enter Price: ";
    cin >> newBook.price;
    books.push_back(newBook);
    cout << "Book added successfully!\n";
}

// Function to search a book by ID
void searchBook() {
    int searchID;
    cout << "Enter Book ID to search: ";
    cin >> searchID;
    for (const auto &book : books) {
        if (book.id == searchID) {
            cout << "Book Found: " << book.title << " by " << book.author << " ($" << book.price << ")\n";
            return;
        }
    }
    cout << "Book not found!\n";
}

// Function to delete a book by ID (Only staff can delete books)
void deleteBook(string role) {
    if (role != "staff") {
        cout << RED <<"Permission Denied! Only staff can delete books.\n";
        return;
    }
    int deleteID;
    cout << "Enter Book ID to delete: ";
    cin >> deleteID;
    for (size_t i = 0; i < books.size(); i++) {
        if (books[i].id == deleteID) {
            books.erase(books.begin() + i);
            cout << "Book deleted successfully!\n";
            return;
        }
    }
    cout << "Book not found!\n";
}

// Function to add to cart (place an order)
void addToCart(string username) {
    int bookID;
    cout << "Enter Book ID to order: ";
    cin >> bookID;
    for (const auto &book : books) {
        if (book.id == bookID) {
            orders.push_back({bookID, username, false});
            cout << "Order placed successfully!\n";
            return;
        }
    }
    cout << "Book not found!\n";
}

// Function to display orders
void displayOrders() {
    cout << "\nOrder List:\n";
    for (const auto &order : orders) {
        cout << "Book ID: " << order.bookId << ", Customer: " << order.username
             << ", Delivered: " << (order.delivered ? "Yes" : "No") << endl;
    }
}

void admin() {
    cout << "Admin\n";
}

// Function to update delivery status (Only staff can update)
void updateDeliveryStatus(string role) {
    if (role != "staff") {
        cout << RED <<"Permission Denied! Only staff can update order status.\n";
        return;
    }
    int bookID;
    cout << "Enter Book ID to mark as delivered: ";
    cin >> bookID;
    for (auto &order : orders) {
        if (order.bookId == bookID) {
            order.delivered = true;
            cout << "Order marked as delivered!\n";
            return;
        }
    }
    cout << "Order not found!\n";
}
               

// Main Menu
void menu(User* user) {
    int choice;
    do {
        cout << YELLOW << "\n--- Book Store System ---\n";
        cout << "1. Display Books\n";
        cout << "2. Add Book (Staff Only)\n";
        cout << "3. Search Book\n";
        cout << "4. Delete Book (Staff Only)\n";
        cout << "5. Add to Cart\n";
        cout << "6. Display Orders\n";
        cout << "7. Update Order Delivery Status (Staff Only)\n";
        cout << "8. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: displayBooks(); break;
            case 2: addBook(user->role); break;
            case 3: searchBook(); break;
            case 4: deleteBook(user->role); break;
            case 5: addToCart(user->username); break;
            case 6: displayOrders(); break;
            case 7: updateDeliveryStatus(user->role); break;
            case 8: cout << "Exiting...\n"; break;
            default: cout << RED <<"Invalid choice!\n";
        }
    } while (choice != 8);
}
// Admin Home Menu
void adminMenu(User* user) {
    int choice;
    string username, password;
    string role = user->role;
    do {
        cout << YELLOW <<"\n--- Admin Menu ---\n";
        cout << "1. Register New Staff\n";
        cout << "2. Manage Books\n";
        cout << "3. View Orders\n";
        cout << "4. Update Order Status\n";
        cout << "5. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: 
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                userList.registerUser(username, password, "staff");
                break;
            case 2: 
                cout << "Redirecting to Book Management...\n";
                displayBooks();
                break;
            case 3: displayOrders(); break;
            case 4: updateDeliveryStatus(user->role); break;
            case 5: cout << "Logging out...\n"; return;
            default: cout << RED << "Invalid choice!\n";
        }
        } while (true);
    }

int main() {
    // Existing registration and login logic
    int choice;
    do {
        cout << YELLOW <<"\n--- Welcome to the Book Store ---\n";

        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        string username, password, role;
        User* user = nullptr;

        switch (choice) {
            case 1:
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                cout << "Enter role (customer/staff): ";
                cin >> role;
                userList.registerUser(username, password, role);
                break;
            case 2:
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                user = userList.login(username, password);
                if (user) {
                    menu(user);
                }
                break;
            case 3:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 3);
    return 0;
}