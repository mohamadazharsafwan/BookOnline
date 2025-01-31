#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

void customersetup();

// LOGIC: Customer Login and Registration System using C++ OOPs Concept and Linked List

class Customer
{
public:
    string id;
    string name;
    string email;
    string password;
    Customer *next; // Pointer to next customer
};

// From customer.txt file, we will read the data and store it in the linked list of customers

class CustomerList
{
public:
    Customer *head;
    Customer *temp;

    CustomerList()
    {
        head = NULL;
        temp = NULL;
    }

    void addCustomer(string id, string name, string email, string password)
    {
        Customer *newCustomer = new Customer();
        newCustomer->id = id;
        newCustomer->name = name;
        newCustomer->email = email;
        newCustomer->password = password;
        newCustomer->next = NULL;

        if (head == NULL)
        {
            head = newCustomer;
            temp = newCustomer;
        }
        else
        {
            temp->next = newCustomer;
            temp = newCustomer;
        }
    }

    void displayCustomers()
    {
        Customer *current = head;
        while (current != NULL)
        {
            cout << "Name: " << current->name << endl;
            cout << "Email: " << current->email << endl;
            cout << "Password: " << current->password << endl;
            cout << "--------------------------" << endl;
            current = current->next;
        }
    }

    // to recheck soon
    Customer *searchCustomer(string email)
    {
        Customer *current = head;
        while (current != NULL)
        {
            if (current->email == email)
            {
                return current;
            }
            current = current->next;
        }
        return NULL;
    }

    // login function
    bool login(string email, string password)
    {
        Customer *current = head;
        while (current != NULL)
        {
            if (current->email == email && current->password == password)
            {
                return true;
            }
            current = current->next;
        }
        return false;
    }
};

void customersetup(){
// insert data customer into linked list (node)
    CustomerList cusL;
    string line;
    // read data from file customer.txt
    ifstream CustomerFile("customer.txt");
    string id, name, email, password;
    while (getline(CustomerFile, line))
    {
        stringstream ss(line);
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, password, ',');
        getline(ss, email, ',');
        cusL.addCustomer(id, name, email, password);
    }

    // login system

    cout << "Enter email: ";
    cin >> email;
    cout << "Enter password: ";
    cin >> password;
    cusL.login(email, password) ? cout << "Login Success" : cout << "Login Failed";
}

int main()
{
 customersetup();

    

    return 0;
}

