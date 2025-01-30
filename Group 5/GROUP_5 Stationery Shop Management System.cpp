#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <sstream> 
#include <utility> 
#include <windows.h>
#include <vector>

using namespace std;


struct Admin //struct 1 Admin
{
    string a_name;
    string a_id;
    string adminPassword;
};

struct Item //struct 2 Item
{
    string itemcode;
    string itemname;
    int stock;
    float price;
    Item* next;
};

struct Customer //struct 3
{
	string name;
    string gender;
    string phoneNumber;
    string password;
};

struct CustomerNode // struct 4
{
    Customer data;
    CustomerNode* next;
};

class InventoryLinkedList //class Linked list
{
private:
Item* head;
    
    Item* merge(Item* left, Item* right) 
	{
	    Item* sorted = NULL;
	    Item** temp = &sorted;
	
	    while (left && right) {
	        if (left->itemcode <= right->itemcode) 
			{
	            *temp = left;
	            left = left->next;
	        } 
			else 
			{
	            *temp = right;
	            right = right->next;
	        }
	        temp = &((*temp)->next);
	    }
	
	    *temp = (left) ? left : right;
	
	    return sorted;
	}

	Item* mergeSort(Item* head) //Merge Sort
	{
	        if (!head || !head->next) 
			{
	            return head;
	        }
	
	        Item* slow = head;
	        Item* fast = head->next;
	
	        while (fast && fast->next) 
			{
	            slow = slow->next;
	            fast = fast->next->next;
	        }
	
	        Item* right = slow->next;
	        slow->next = NULL;
	
	        Item* sortedLeft = mergeSort(head);
	        Item* sortedRight = mergeSort(right);
	
	        return merge(sortedLeft, sortedRight);
	}
public:
    InventoryLinkedList() : head(NULL) 
	{
		
	}
	pair<float, int> binarySearchItem(const string& code) //Binary Search for inventory found (Cus)
	{
	    ifstream infile("inventory.txt");
	    if (!infile) 
		{
	        cerr << "Error opening inventory file." << endl;
	        return make_pair(0.0, 0);
	    }
	    string itemcode, itemname;
	    int stock;
	    float price;
	    bool found = false;
	    
	    string line;
	    while (getline(infile, line)) 
		{
	        stringstream ss(line);
	        ss >> itemcode >> itemname >> price >> stock;
	
	        if (itemcode == code) 
			{
	            found = true;
	            cout << "Item found: " << itemname << endl;
	            infile.close();
	            return make_pair(price, stock); 
	        }
	    }
	
	    infile.close();
	
	    cout << "Item with code " << code << " not found." << endl; 
	    return make_pair(0.0, 0);
	} //end of Binary Search inventory found Cus
		
	void insertItem() //Write item into file
	{
	    ofstream tempFile("temp_inventory.txt");
	    if (!tempFile) 
	    {
	        cerr << "Error creating temporary file." << endl;
	        return;
	    }
	    ifstream infile("inventory.txt");
	    if (!infile) 
	    {
	        cerr << "Error opening inventory file." << endl;
	        tempFile.close();
	        return;
	    }
	
	    string line;
	    while (getline(infile, line)) 
	    {
	        tempFile << line << endl;
	    }
	    infile.close();
	
	    Item newItem;
	    cout << "Enter Item Code: ";
	    cin >> newItem.itemcode;
	
	    ifstream checkFile("inventory.txt");
	    bool itemExists = false;
	    while (getline(checkFile, line)) 
	    {
	        stringstream ss(line);
	        string code;
	        ss >> code;
	        if (code == newItem.itemcode) 
	        {
	            cout << "Item code already exists. Cannot add item." << endl;
	            itemExists = true;
	            break;
	        }
	    }
	    checkFile.close();
	
	    if (!itemExists) 
	    {
	        cout << "Enter Item Name: ";
	        cin.ignore();
	        getline(cin, newItem.itemname);
	        cout << "Enter Price: RM ";
	        cin >> newItem.price;
	        cout << "Enter Stock: ";
	        cin >> newItem.stock;
	
	        tempFile << setw(4) << left << newItem.itemcode << setw(40) << left << newItem.itemname << setw(8) << fixed << setprecision(2) << newItem.price << setw(5) << newItem.stock << endl;
	        cout << endl;
	        cout << "Item added successfully" << endl;
	    }
	
	    tempFile.close();
	    remove("inventory.txt");
	    rename("temp_inventory.txt", "inventory.txt");
	} // end of add item


    void editItem() //update txt file inventory
	{
        ofstream tempFile("temp_inventory.txt");
        if (!tempFile) 
		{
            cerr << "Error creating temporary file." << endl;
            return;
        }

        ifstream infile("inventory.txt");
        if (!infile) 
		{
            cerr << "Error opening inventory file." << endl;
            tempFile.close();
            return;
        }

        string line;
        string code;
        cout << "Enter Item Code to edit: ";
        cin >> code;

        bool found = false;
        while (getline(infile, line)) 
		{
            stringstream ss(line);
            string tempCode;
            ss >> tempCode;

            if (tempCode == code) 
			{
                found = true;
                pair<float, int> searchResult = binarySearchItem(code); //Binary Search

                int editChoice;
                cout << endl;
                cout << "What would you like to edit?" << endl;
                cout << "1. Item Name" << endl;
                cout << "2. Price" << endl;
                cout << "3. Quantity" << endl;
                cout << "Enter your choice: ";
                cin >> editChoice;
                cout << endl;

                switch (editChoice) 
				{
                    case 1: 
					{
                        string newName;
                        cout << endl;
                        cout << "Enter New Item Name: ";
                        cin.ignore(); 
                        getline(cin, newName);
 					    // Read the original name and stock                       
                        float price;
                        int stock;
                        string itemName;
                        ss >> itemName >> price >> stock;
                        tempFile << setw(4) << left << code << setw(40) << left << newName << setw(8) << fixed << setprecision(2) << price << setw(5) << stock << endl;

                        break;
                    }
                    case 2: // Edit by Price
					{
					    float newPrice;
					    cout << endl;
					    cout << "Enter New Price: ";
					    cin >> newPrice;
					    // Read the original name and stock
					    string itemName;
					    float price;
					    int stock;
					    ss >> itemName >> price >> stock;
					    // Write back the original name and the edited price and stock
					    tempFile << setw(4) << left << code << setw(40) << left << itemName << setw(8) << fixed << setprecision(2) << newPrice << setw(5) << stock << endl;
					    break;
					}
                    case 3: 
					{
                        int newStock;
                        cout << endl;
                        cout << "Enter New Stock: ";
                        cin >> newStock;
                        string itemName;
                        float price;
                        ss >> itemName >> price;
                        tempFile << setw(4) << left << code << setw(40) << left << itemName << setw(8) << fixed << setprecision(2) << price << setw(5) << newStock << endl;
                        break;
                    }
                    default:
                        cout << "Invalid choice." << endl;
                        
                        tempFile << line << endl;
                        break;
                }
            } 
			else 
			{
                
                tempFile << line << endl;
            }
        }
        infile.close();
        tempFile.close(); //end of update inventory txt file

        if (!found) 
		{
            cout << "Item with code " << code << " not found." << endl;
            
            remove("temp_inventory.txt");
            return;
        }

        remove("inventory.txt");
       
        rename("temp_inventory.txt", "inventory.txt");

		cout << endl;
        cout << "Item edited successfully" << endl;
    } //end of updating inventory txt file

    void deleteItem() //delete inventory from txt file
	{
        
        ofstream tempFile("temp_inventory.txt");
	    if (!tempFile) 
		{
	        cerr << "Error creating temporary file." << endl;
	        return;
	    }
	
	    ifstream infile("inventory.txt");
	    if (!infile) 
		{
	        cerr << "Error opening inventory file." << endl;
	        tempFile.close();
	        return;
	    }
	
	    string line;
	    string code;
	    cout << "Enter Item Code to delete: ";
	    cin >> code;
	
	    bool found = false;
	    while (getline(infile, line)) 
		{
	        stringstream ss(line);
	        string tempCode;
	        ss >> tempCode;
	
	        if (tempCode == code) 
			{
	            found = true;
	            continue; 
	        }
	        
	        tempFile << line << endl;
	    }
	
	    infile.close();
	    tempFile.close();
	
	    if (!found) 
		{
	        cout << "Item with code " << code << " not found." << endl;
	        
	        remove("temp_inventory.txt");
	        return;
	    }
	
	    remove("inventory.txt");
	 
	    rename("temp_inventory.txt", "inventory.txt");
	
		cout << endl;
	    cout << "Item deleted successfully!" << endl;
	}//end of deleting inventory txt file
	
	void insertion_sort_record(vector<string> &arr) //insertion sort for manage records
	{
	    int n = arr.size();
	    for (int i = 1; i < n; i++) 
		{
	        string key = arr[i];
	        int j = i - 1;
	        // Compare item names instead of item codes
	        while (j >= 0 && getItemName(arr[j]) > getItemName(key)) 
			{
	            arr[j + 1] = arr[j];
	            j = j - 1;
	        }
	        arr[j + 1] = key;
	    }
	} //end of sort for records

	// Function to extract item name from item record from order file
	string getItemName(const string& itemRecord) 
	{
	    stringstream ss(itemRecord);
	    string itemName;
	    ss >> itemName;
	    ss >> itemName;
	    return itemName;
	} 
	
	void records() //records sorted 
	{
	    std::ifstream infile("order.txt");
	    if (!infile) 
		{
	        std::cerr << "Error opening order file." << std::endl;
	        return;
	    }
	
	    vector<string> arr;
	    string code, itemName, qty_str;
	    float price;
	    while (infile >> code >> itemName >> qty_str >> price) 
		{
	        stringstream ss;
	        ss << code << " " << itemName << " " << qty_str << " " << price;
	        arr.push_back(ss.str());
	    }
	    infile.close();
	
	    insertion_sort_record(arr); //sorted
	
	    std::cout << std::left << std::setw(15) << "Item Code" << std::setw(35) << "Item Name" << std::setw(15) << "Quantity" << std::setw(15) << "Price(RM)" << std::endl;
	
	    std::string line;
	    float totalSales = 0.0;
	
	    for (int i = 0; i < arr.size(); i++)  //sorting
		{
	        stringstream ss(arr[i]);
	        std::string itemcode, itemname, qty;
	        float price;
	        ss >> itemcode >> itemname >> qty >> price;
	
	        int quantity_int;
	        std::istringstream(qty) >> quantity_int;
	
	        float subtotal = price * quantity_int;
	        totalSales += subtotal;
	
	        std::cout << std::left << std::setw(15) << itemcode << std::setw(35) << itemname << std::setw(15) << qty << std::fixed << std::setprecision(2) << std::setw(15) << price << std::endl;
	    }
	
	    cout << endl;
	    cout << "- POPULAR STATIONARY SHOP -" << endl << endl;
	    std::cout << "TOTAL SALES : RM " << std::fixed << std::setprecision(2) << totalSales << std::endl;
	    cout << endl;
	} // end of sorted records
 
	void insertion_sort_code(vector<string> &arr) //insertion sort for inventory
	{
	    int n = arr.size();
	    for (int i = 1; i < n; i++) 
		{
	        string key = arr[i];
	        int j = i - 1;
	        // Compare item codes
	        while (j >= 0 && getItemCode(arr[j]) > getItemCode(key)) 
			{
	            arr[j + 1] = arr[j];
	            j = j - 1;
	        }
	        arr[j + 1] = key;
	    }
	}// end of insertion sort for inventory
	
	string getItemCode(const string& item) 
	{
	    stringstream ss(item);
	    string code;
	    ss >> code; // Assuming the item code is the first token in the string
	    return code;
	}

    void display() //function to display inventory
	{
	    ifstream infile("inventory.txt");
	    if (!infile) 
		{
	        cerr << "Error opening inventory file." << endl;
	        return;
    	}

		cout << "Item Code\tItem Name\t\t\t\tPrice\tStock" << endl;
	    vector<string> arr;
	    string line;
	    while (getline(infile, line)) 
		{
	        arr.push_back(line);
	    }
	    infile.close(); // Close inventory file after reading its contents
	
	    insertion_sort_code(arr);
	
	    // Iterate over sorted items and display each one
	    for (size_t i = 0; i < arr.size(); ++i)
		{
		    stringstream ss(arr[i]);
		    string itemcode, itemname, price, stock;
		    ss >> itemcode; 
		    getline(ss, itemname, '\t'); 
			getline(ss, itemname); 
			
			ss >> price >> stock;
			cout << left << setw(15) << itemcode << setw(40) << itemname << setw(10) <<fixed << setprecision(2)<< price << setw(5) << stock << endl;
		}
	} //end of function inventory
	
    ~InventoryLinkedList() //destructor for class InventoryLinkedList
	{
        Item* current = head;
        while (current) 
		{
            Item* next = current->next;
            delete current;
            current = next;
        }
    } //end of destructor for class InventoryLinked List
}; //end of class Inventory LinkedList

/***************************************Customer modules*********************************************/
class CustomerList //class for customer
{
private:
    CustomerNode* head;

public:
    CustomerList() : head(NULL) {}
	
    void addCustomer() // Function for add customer to linked list
	{
		ofstream tempFile("temp_customer.txt");
	    if (!tempFile) 
		{
	        cerr << "Error creating temporary file." << endl;
	        return;
	    }
	    
	    ifstream infile("customer.txt");
	    if (!infile) 
		{
	        cerr << "Error opening customer file." << endl;
	        tempFile.close();
	        return;
	    }
	    
	    string line;
	    while (getline(infile, line)) 
		{
	        tempFile << line << endl;
	    }
	    
	    infile.close();
	    
	    Customer newc;
	    /******* Register **************/
	    cout << "Enter Your NickName: ";
	    cin.ignore();
		getline(cin , newc.name);
	    cout << "Enter Your Gender [M/F]? : ";
	    getline(cin, newc.gender);
	    cout << "Enter Your Phone Number [000-0000000]:";
	    cin >> newc.phoneNumber;
	    cout << "Enter New Password: ";
	    cin >> newc.password;
	    
		tempFile << left  << newc.name <<" "<< setw(20) << newc.gender << setw(20) << left << newc.phoneNumber << setw(15) << left << newc.password << endl;
	    tempFile.close();
	    remove("customer.txt");
	    rename("temp_customer.txt", "customer.txt");
	
		cout << endl;
	    cout << "Customer added successfully" << endl;
    } //end of function add customer to Linked list
   
   void displayCustomers() 
   {
	    ifstream infile("customer.txt");
	    if (!infile) {
	        cerr << "Error opening customer file." << endl;
	        return;
	    }
	    
	    cout << left << setw(26) << "Name" << setw(20) << "Gender" << setw(20) << "Phone Number" << setw(20) << "Password" << endl;
	    string line;
	    
	    while (getline(infile, line)) 
		{
	        stringstream ss(line);
	        string name, gender, phoneNumber, password;
	        ss >> name;
	        getline(ss, gender, '\t');
	        getline(ss, gender);
	        ss >> phoneNumber >> password;
	        
	        cout << left << setw(25) << name << setw(10) << left << gender << setw(20) << phoneNumber << setw(15) << password << endl;
	    }
	    infile.close();
	}//end of function display all customers
    
    void addCart() //add to cart function
    {
    	InventoryLinkedList inv;
    	system("cls");
    	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "\t\t\tYOUR SHOPPING CART DETAILS\t\t\t" << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        
        ifstream cartFile("cart.txt"); // Read cart details from txt file
		if (!cartFile) 
		{
		    cerr << "Error opening cart file." << endl;
		    return;
		}
		
        cout << left << setw(20) << "Item Code" << setw(30) << "Item Name" << setw(15) << "Quantity" << setw(15) << "Subtotal (RM)" << endl;
        
		string code, itemName;
	    int qty;
	    float price, grand = 0.0;
	
	    while (cartFile >> code >> itemName >> qty >> price)
	    {
	        cout << fixed << setprecision(2);  // Output item details from cart.txt
	        float subtotal = price * qty; // Calculate subtotal
	        
	        cout << left << setw(20) << code << setw(30) << itemName << setw(15) << qty << setw(15) << subtotal << endl;
	        grand += subtotal; // Calculate grand total
	    }
	
	    cartFile.close(); 
	    
        char choice;
        int option;
        
        cout << endl;
        cout << "Do you want to make any changes before proceed to your checkout? [Y/N]: ";
        cin >> choice;
        
        if(choice == 'Y'|| choice == 'y')
        {
        	/******* Checkout **************/
        	cout << endl;
        	cout << "Choose your choice by entering the number [1/2/3] "<<endl;
        	cout << "1. Add New Item"<<endl;
        	cout << "2. Edit Item"<< endl;
        	cout << "3. Delete Item"<< endl;
        	cout << "4. Checkout" << endl;
        	cout << endl;
        	cout << "Enter the Choice :";
        	cin >> option;
        	
        	switch(option)
        	{
        		case 1: //add new item
        		{
					ofstream tempFile("temp_cart.txt");
				    if (!tempFile) 
					{
				        cerr << "Error creating temporary file." << endl;
				        return;
				    }
				    ifstream infile("cart.txt");
				    if (!infile) 
					{
				        cerr << "Error opening cart file." << endl;
				        tempFile.close();
				        return;
				    }
				    string line;
				    while (getline(infile, line)) 
					{
				        tempFile << line << endl;
				    }
				    
				    infile.close();
				    
				    do
					{
					    string code, itemName;
					    int quantity;
					    float totalPrice = 0.0;
					    
					    inv.display(); //display the inventory list
	
						cout << endl;
					    cout << "What would you like to add?" << endl;
					    cout << "Enter Item Code: ";
					    cin >> code;
					
					    pair<float, int> searchResult = inv.binarySearchItem(code); //Binary Search
					    float price = searchResult.first;
					    int stock = searchResult.second;
					
					    if (stock > 0)
					    {
					        ifstream inventoryFile("inventory.txt");  // Get the item name with the item code
					        if (!inventoryFile)
					        {
					            cerr << "Error opening inventory file." << endl;
					            return;
					        }
					
					        string itemCode, itemName;
					        float itemPrice;
					        int itemStock;
					        bool found = false;
					        while (inventoryFile >> itemCode >> itemName >> itemPrice >> itemStock)
					        {
					            if (itemCode == code)
					            {
					                found = true;
					                break;
					            }
					        }
					        inventoryFile.close();
					
					        if (!found)
					        {
					            cout << "Item with code " << code << " not found in inventory." << endl;
					            continue; 
					        }
					
							cout << endl;
					        cout << "Enter Quantity: ";
					        cin >> quantity;
					
					        ofstream cartFile("cart.txt", ios::app);  // Write order details to cart.txt file
					        if (!cartFile)
					        {
					            cerr << "Error opening cart file." << endl;
					            return;
					        }
					        cartFile << left << setw(20) << code << setw(30) << itemName << setw(10) << quantity << setw(10) << price <<endl; // Write item name instead of code
					
					        totalPrice += price * quantity; // Calculate total price
					
					        ifstream infile("inventory.txt"); // Update stock in inventory.txt file
					        if (!infile)
					        {
					            cerr << "Error opening inventory file." << endl;
					            return ;
					        }
					
					        ofstream tempFile("temp_inventory.txt");
					        if (!tempFile)
					        {
					            cerr << "Error creating temporary file." << endl;
					            return;
					        }
					
					        string line;
					        while (getline(infile, line)) // Updating stock
					        {
					            stringstream ss(line);
					            string itemcode, itemname;
					            float itemprice;
					            int itemStock;
					            ss >> itemcode >> itemname >> itemprice >> itemStock;
					
					            if (itemcode == code)
					            {
					                itemStock -= quantity;
					            }
					
					            tempFile << setw(4) << left << itemcode << setw(40) << itemname << setw(8) << itemprice << setw(5) << itemStock << endl;
					        }
					
					        infile.close();
					        tempFile.close();
					
					        remove("inventory.txt");
					        rename("temp_inventory.txt", "inventory.txt");
					    }
					    else
					    {
					        cout << "OUT OF STOCK. Please come again later." << endl;
					    }
					    cout << "Is there anything else? (Y/N): ";
					    cin >> choice;
					    
					} while (choice == 'Y' || choice == 'y');
						break; //end of do-while loop
					} //end of case 1 add new item
				
				case 2: // Edit cart item
				{
				    ofstream tempFile("temp_cart.txt");
				    if (!tempFile) 
				    {
				        cerr << "Error creating temporary file." << endl;
				        return;
				    }
				    
				    ifstream infile("cart.txt");
				    if (!infile) 
				    {
				        cerr << "Error opening cart file." << endl;
				        tempFile.close();
				        return;
				    }
				    
				    string line;
				    string code;
				    cout << "Enter Item Code to edit: ";
				    cin >> code;
				    
				    bool found = false;
				    while (getline(infile, line)) 
				    {
				        stringstream ss(line);
				        string tempCode;
				        ss >> tempCode;
				        
					if (tempCode == code) 
					{
					    found = true;
					    pair<float, int> searchResult = inv.binarySearchItem(code); // Search for the item details
					
					    int newqty;
					    cout << endl;
					    cout << "YOU ARE ONLY ALLOWED TO EDIT THE QUANTITY OF THE ITEM" << endl << endl;
					    cout << "Enter the new quantity: ";
					    cin >> newqty;
					
					    // Write back the original line from the input file with the updated quantity
					    tempFile << setw(4) << left << code << setw(40) << left << itemName << setw(8)  << newqty << setw(5) << fixed << setprecision(2)<< price << endl;
					} 
				        else 
				        {
				            tempFile << line << endl;
				        }
				    }
				    
				    infile.close();
				    tempFile.close(); // End of update cart.txt file
				    
				    if (!found) 
				    {
				        cout << "Item with code " << code << " not found." << endl;
				        
				        remove("temp_cart.txt");
				        return;
				    }
				    
				    remove("cart.txt");
				    rename("temp_cart.txt", "cart.txt");
				    
				    cout << endl;
				    cout << "Your quantity updated successfully" << endl << endl;
				    
				    // Display the updated cart details
				    ifstream cartFile("cart.txt"); 
				    if (!cartFile) 
				    {
				        cerr << "Error opening cart file." << endl;
				        return;
				    }
				    
				    cout << left << setw(20) << "Item Code" << setw(30) << "Item Name" << setw(15) << "Quantity" << setw(15) << "Subtotal (RM)" << endl;
				    
				    string itemName;
				    int qty;
				    float itemPrice, grand = 0.0;
				    
				    while (cartFile >> code >> itemName >> qty >> itemPrice)
				    {
				        cout << fixed << setprecision(2);
				        float subtotal = itemPrice * qty; // Calculate subtotal
				        
				        cout << left << setw(20) << code << setw(30) << itemName << setw(15) << qty << setw(15) << subtotal << endl;
				        grand += subtotal; // Calculate grand total
				    }
				    
				    cartFile.close();
				    break; 
				} // End of case 2: edit cart item
				
				case 3: //delete cart item
					{
			        ofstream tempFile("temp_cart.txt");
				    if (!tempFile) 
					{
				        cerr << "Error creating temporary file." << endl;
				        return;
				    }
				
				    ifstream infile("cart.txt");
				    if (!infile) 
					{
				        cerr << "Error opening cart file." << endl;
				        tempFile.close();
				        return;
				    }
				
				    string line;
				    string code;
				    cout << "Enter Item Code to delete: ";
				    cin >> code;
				
				    bool found = false;
				    while (getline(infile, line)) 
					{
				        stringstream ss(line);
				        string tempCode;
				        ss >> tempCode;
				
				        if (tempCode == code) 
						{
				            found = true;
				            continue; 
				        }
				        
				        tempFile << line << endl;
				    }
				
				    infile.close();
				    tempFile.close();
				
				    if (!found) 
					{
				        cout << "Item with code " << code << " not found." << endl;
				        
				        remove("temp_cart.txt");
				        return;
				    }
				
				    remove("cart.txt");
				 
				    rename("temp_cart.txt", "cart.txt");
				
				    cout << "Item deleted successfully" << endl<<endl;
				    
				    ifstream cartFile("cart.txt"); // Read cart details from txt file
					if (!cartFile) 
					{
					    cerr << "Error opening cart file." << endl;
					    return;
					}
					
			        cout << left << setw(20) << "Item Code" << setw(30) << "Item Name" << setw(15) << "Quantity" << setw(15) << "Subtotal (RM)" << endl;
			        
					string  itemName;
				    int qty;
				    float price, grand = 0.0;
				
				    while (cartFile >> code >> itemName >> qty >> price)
				    {
				        cout << fixed << setprecision(2);
				        float subtotal = price * qty; // Calculate subtotal
				        
				        cout << left << setw(20) << code << setw(30) << itemName << setw(15) << qty << setw(15) << subtotal << endl;
				        grand += subtotal; // Calculate grand total
				    }
				
				    cartFile.close();
				}//end of case 3 delete item
					break;
				case 4:
					receipt();
					break;
				default:
					cout << "Invalid choice." << endl;
					break;
			} // end of switch
		} //end of if
    } //end add new cart
    
   void insertion_sort(vector<string> &arr)  //insertion sort
   {
    int n = arr.size();
	    for (int i = 1; i < n; i++) 
		{
	        string key = arr[i];
	        int j = i - 1;
	        
	        // Compare item names instead of item codes
	        while (j >= 0 && getItemName(arr[j]) > getItemName(key)) 
			{
	            arr[j + 1] = arr[j];
	            j = j - 1;
	        }
	        arr[j + 1] = key;
	    }
	} //end of insertion sort
	
	// Function to extract item name from item record
	string getItemName(const string& itemRecord) 
	{
	    stringstream ss(itemRecord);
	    string itemName;
	    ss >> itemName;
	    ss >> itemName;
	    return itemName;
	}
	
	void printCentered(const string& text, int width) //align word to center
	{
	    int numSpaces = width - text.length();
	    int leftPadding = numSpaces / 2;
	    int rightPadding = numSpaces - leftPadding;
	    
	    cout << setw(leftPadding) << "" << text << setw(rightPadding) << "" << endl;
	} //end of align word to center
	
   void receipt() // start receipt
   {
	    Sleep(2000);
	    system("cls");
	    
	    /******* Receipt **************/
		cout << endl;
	    cout << "______________________________________________________________________________________________" << endl;
	    cout << left << setw(50);
	    printCentered("POPULAR STATIONERY SHOP", 100);
	    cout << left << setw(49);
	    printCentered("AEON MALL (GROUND FLOOR)", 99);
	    cout << left << setw(52);
	    printCentered("Lot G57, Persiaran Indahpura 7, Bandar Indahpura", 104);
	    cout << left << setw(51);
	    printCentered("81000 Kulaijaya Johor Bahru", 101);
	    cout << "______________________________________________________________________________________________" << endl << endl;
	    
	
	    ifstream cartFile("cart.txt"); // Read cart details from txt file
	    if (!cartFile) 
		{
	        cerr << "Error opening cart file." << endl;
	        return;
	    }
	
	    vector<string> arr;
	    string code, itemName;
	    int qty;
	    float price;
	    while (cartFile >> code >> itemName >> qty >> price) 
		{
	        stringstream ss;
	        ss << code << " " << itemName << " " << qty << " " << price;
	        arr.push_back(ss.str());
	    }
	    cartFile.close(); // Close cart file after reading its contents
	
	    insertion_sort(arr);
	
	    ofstream orderFile("order.txt", ios::app); // Append to order file
	    if (!orderFile) 
		{
	        cerr << "Error opening order file." << endl;
	        return;
	    }
	
	    // Display item details header
	    cout << left << setw(20) << "Item Code" << setw(30) << "Item Name" << setw(15) << "Quantity" << setw(15) << "Subtotal (RM)" << endl;
	
	    float grand = 0.0;
	
	    // Iterate over sorted items
	    for (int i = 0; i < arr.size(); i++) 
		{
	        stringstream ss(arr[i]);
	        ss >> code >> itemName >> qty >> price;
	
	        // Output item details to console
	        cout << fixed << setprecision(2);
	        float subtotal = price * qty; // Calculate subtotal
	        
	        cout << left << setw(20) << code << setw(30) << itemName << setw(15) << qty << setw(15) << subtotal << endl;
	        grand += subtotal; // Calculate grand total
	
	        // Write item details to order.txt
	        orderFile << left << setw(20) << code << setw(30) << itemName << setw(10) << qty << setw(10) << price << endl;
	    }
	
	    orderFile.close(); // Close order file
	
	    // Display grand total
	    cout << endl;
	    cout << "Grand Total: RM " << grand << endl;
	    cout << endl;
	} // end of receipt

    
	void clearCartFile() //clean cart file
	{
	    ofstream cartFile("cart.txt", ios::trunc);
	    if (!cartFile) 
		{
	        cerr << "Error opening cart file." << endl;
	        return;
	    }
	    cartFile.close(); 
	} // end clean cart file
	
    // Destructor to free memory
    ~CustomerList() 
	{
        CustomerNode* temp;
        while (head) 
		{
            temp = head;
            head = head->next;
            delete temp;
        }
    } //end of destructor customer list
}; //end of class customer 


/***************************************Admin modules*********************************************/
void admin() // admin function
{
	InventoryLinkedList inventory;
    CustomerList ctm;
    
	string inputId;
    string inputPassword;
    
    cout<<endl;
    cout << "Enter Your ID number: ";
    cin >> inputId;
    
	ifstream adminFile("admin.txt");
    if (!adminFile) 
	{
        cerr << "Error opening admin file." << endl;
        return;
    }
    string adminId, adminName, adminPassword;
    bool foundId = false;
    while (adminFile >> adminId >> adminName >> adminPassword) 
	{
        if (adminId == inputId) 
		{
            foundId = true;
            cout << "Enter Your Password: ";
            cin >> inputPassword;
            if (adminPassword == inputPassword) 
			{
            	cout<<endl;
                cout << "Welcome Back , " << adminName << endl;
                break;
            } else 
			{
                cout << "Incorrect password. Please enter the correct password." << endl;
                admin();
                break; 
            }
        }
        
    }

    adminFile.close(); // end of read admin file
	if (!foundId) 
	{
        cout << "Entered ID doesn't exist. Please enter again." << endl<<endl;
        admin();
        return;
    }
    

    char continueChoice = 'Y'; 

    do
    {
        /******* MainMenu for Admin Modules **************/
        Sleep(1000);
        system("cls");
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "\t\t\t\t\tMAIN MENU\t\t\t\t\t" << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "1. Manage Inventory" << endl;
        cout << "2. View Member" << endl;
        cout << "3. View Records" << endl;
        cout << "4. Exit" << endl;
        cout << endl;

        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        cout << endl;

        switch (choice)
        {
        case 1:
            inventory.display(); // Sorted inventory by ID 
            int invChoice;

            /********* SubMenu for Manage Inventory ************/
            cout << endl;
            cout << "Please choose an option:" << endl;
            cout << "1. Add Item" << endl;
            cout << "2. Edit Item" << endl;
            cout << "3. Delete Item" << endl;
            cout << "4. Back" << endl;
            cout << endl;
            cout << "Enter your choice: ";
            cin >> invChoice;

            switch (invChoice)
            {
            case 1:
                inventory.insertItem();
                break;
            case 2:
                inventory.editItem();
                break;
            case 3:
                inventory.deleteItem();
                break;
            case 4:
                break;
            default:
                cout << "Invalid choice." << endl;
            }
            break;
        case 2:
            ctm.displayCustomers();
            break;
        case 3:
        	inventory.records();
        	break;
        case 4:
            cout << "Exiting..." << endl;
            return;
        default:
            cout << "Invalid choice." << endl;
        }
		cout<<endl;
        cout << "Do you want to continue? (Y/N): ";
        cin >> continueChoice;
        system("cls");

    } while (continueChoice == 'Y' || continueChoice == 'y');
} //end of admin function

int main() //main function
{
    CustomerList ctm;
    ctm.clearCartFile();
    
    bool continueShopping = true;

    do 
    {
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "\t\t\tWELCOME TO POPULAR STATIONERY SHOP\t\t\t" << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

        string user;
        cout << "Are you a customer or admin [C/A]: ";
        cin >> user;

        if (user == "C" || user == "c") 
        {
            string choice;
            do 
            {
                cout << endl;
                cout << "For new customers please register your account before proceed to login. Thank You and enjoy your shopping." <<endl;
                cout << "1. Register" << endl;
                cout << "2. Login" << endl;
                cout << endl;
                cout << "Choose an option: ";
                cin >> choice;

                if (choice == "1") //customer register
                {
                    CustomerList ctm;
                    ctm.addCustomer(); 
                } 
                else if (choice == "2") //customer login
                {
                    string username;
                    cout << "Enter Username: ";
                    cin.ignore();
                    getline(cin, username);

                    ifstream infile("customer.txt");
                    if (!infile) 
                    {
                        cerr << "Error opening customer file." << endl;
                        return 1;
                    }

                    string line;
                    bool found = false;
                    
                    while (getline(infile, line)) 
                    {
                        stringstream ss(line);
                        string name, gender, phonenum, pass;
                        ss >> name >> gender >> phonenum >> pass;

                        if (name == username) 
                        {
                            found = true;
                            cout << "Enter Your Password: ";
                            string inputPassword;
                            cin >> inputPassword;

                            if (inputPassword == pass) 
                            {
                                cout << "Login Successful" << endl;
                                cout << endl;

                                InventoryLinkedList inv;
                                inv.display(); //display the inventory list

                                float totalPrice = 0.0;
                                char choice;
                                ofstream cartFile("cart.txt", ios::app);
                                do
                                {
                                    string code, itemName;
                                    int quantity;
                                    cout << endl;
                                    cout << "What would you like to buy?" << endl;
                                    cout << "Enter Item Code: ";
                                    cin >> code;

                                    pair<float, int> searchResult = inv.binarySearchItem(code); //Binary Search
                                    float price = searchResult.first;
                                    int stock = searchResult.second;

                                    if (stock > 0)
                                    {
                                        ifstream inventoryFile("inventory.txt");  // Get the item name with the item code
                                        if (!inventoryFile)
                                        {
                                            cerr << "Error opening inventory file." << endl;
                                            return 1;
                                        }

                                        string itemCode, itemName;
                                        float itemPrice;
                                        int itemStock;
                                        bool found = false;
                                        while (inventoryFile >> itemCode >> itemName >> itemPrice >> itemStock)
                                        {
                                            if (itemCode == code)
                                            {
                                                found = true;
                                                break;
                                            }
                                        }
                                        inventoryFile.close();

                                        if (!found)
                                        {
                                            cout << "Item with code " << code << " not found in inventory." << endl;
                                            continue; 
                                        }

                                        cout << endl;
                                        cout << "Enter Quantity: ";
                                        cin >> quantity;
                                        cout << endl;

                                        if (quantity > stock)
                                        {
                                            cout << "Requested quantity is more than available stock. Available stock: " << stock << endl;
                                            continue;
                                        }

                                        ofstream cartFile("cart.txt", ios::app);  // Write order details to cart.txt file
                                        if (!cartFile)
                                        {
                                            cerr << "Error opening cart file." << endl;
                                            return 1;
                                        }
                                        cartFile << left << setw(30) << code << setw(40) << itemName << setw(10) << quantity << setw(10) << price << endl; // Write item name instead of code

                                        totalPrice += price * quantity; // Calculate total price

                                        ifstream infile("inventory.txt"); // Update stock in inventory.txt file
                                        if (!infile)
                                        {
                                            cerr << "Error opening inventory file." << endl;
                                            return 1;
                                        }

                                        ofstream tempFile("temp_inventory.txt");
                                        if (!tempFile)
                                        {
                                            cerr << "Error creating temporary file." << endl;
                                            return 1;
                                        }

                                        string line;
                                        while (getline(infile, line)) // Updating stock
                                        {
                                            stringstream ss(line);
                                            string itemcode, itemname;
                                            float itemprice;
                                            int itemStock;
											ss >> itemcode;
    										ss.ignore(); // Ignore the space after the item code
    										getline(ss, itemname, '\t'); // Read the item name including spaces
                                            ss >> /*itemcode >> itemname >>*/ itemprice >> itemStock;

											cout << "DEBUG: Read item - Code: " << itemcode << ", Name: " << itemname << ", Price: " << itemprice << ", Stock: " << itemStock << endl;

                                            if (itemcode == code)
                                            {
                                                itemStock -= quantity;
												cout << "DEBUG: Updated stock for item " << itemcode << " to " << itemStock << endl;
                                            }

                                            tempFile << setw(4) << left << itemcode << setw(40) << itemname << setw(8) << itemprice << setw(5) << itemStock << endl;
                                        }

                                        infile.close();
                                        tempFile.close();

                                        remove("inventory.txt");
                                        rename("temp_inventory.txt", "inventory.txt");
                                    }
                                    else
                                    {
                                        cout << "OUT OF STOCK..." << endl;
                                    }

                                    cout << "Is there anything else? (Y/N): ";
                                    cin >> choice;

                                } while (choice == 'Y' || choice == 'y');


                                cartFile.close(); //close order txt

                                ctm.addCart();
                                ctm.receipt();
                                cout << "Thank You and Come Again" << endl;

                                continueShopping = false;
                                break; 
                            } 
                            else 
                            {
                                cout << "Invalid Password Entered." << endl;
                                break; 
                            }
                        }
                    }
                    infile.close();

                    if (!found) 
                    {
                        char tryAgain;
                        cout << "Username not found. Try again. (Y/N): ";
                        cin >> tryAgain;

                        if (tryAgain == 'N' || tryAgain == 'n') 
                        {
                            continueShopping = false;
                            break; 
                        }
                    }
                } 
                else 
                {
                    cout << "Invalid choice." << endl;
                }
            } 
            while (continueShopping); 

        } 
        else if (user == "A" || user == "a") 
        {
            admin(); 
            continueShopping = false;
        } 
        else 
        {
            cout << "Try again" << endl;
        }

    } 
    while (continueShopping); 

    return 0;
} //end of main
