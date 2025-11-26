#include <iostream>     // For standard input and output (cin, cout)
#include <fstream>      // For file handling (reading/writing stock.dat and history.log)
#include <vector>       // For using the vector container to store stock items 
#include <iomanip>      // For formatting output using setw, left, etc.
#include <algorithm>    // For searching, sorting, or other STL algorithms (optional)
#include <ctime>        // For date and time functions (used in logging with timestamps)
#include <limits>       // For input validation (e.g. clearing cin buffer with numeric_limits)
#include <map>         // For using map to count categories
using namespace std;

// Enhanced structure with better data management
struct StockItem {
    int productID;
    string name;
    string category;
    int quantity;
    double lastPrice;
    time_t dateAdded;
    
    StockItem() : productID(0), quantity(0), lastPrice(0.0), dateAdded(time(0)) {}
};

// Global variables
vector<StockItem> stock;
vector<string> historyLog;
vector<string> categoryOptions = {"Fruits", "Vegetables", "Snacks", "Beverages", "Dairy", "Meat", "Bakery", "Frozen Foods", "Other"};
double grandTotalSales = 0.0;

// Function declarations
void clearScreen();                 // Clears the console screen (platform-dependent)
void displayMainMenu();            // Displays the main menu to the user
void loadStockFromFile();          // Loads stock data from file into memory
void saveStockToFile();            // Saves current stock data from memory to file
void logAction(const string& action); // Logs an action (add, update, delete, sale) with timestamp
void viewStockHistory();           // Displays the history log of stock actions
void addNewItem();                 // Adds a new item to the stock
void viewAllItems();               // Displays all stock items in a table
void updateItem();                 // Updates the details of an existing item
void deleteItem();                 // Deletes an item from stock by ID
void searchItem();                 // Searches for items by ID, name, or category
void lowStockAlert();              // Shows items with quantity below a threshold
void makeSale();                   // Handles a sale, reduces stock, and updates revenue

// Helper functions
void loadGrandTotalFromFile();     // Loads total revenue from file
void saveGrandTotalToFile();       // Saves updated total revenue to file
void pauseScreen();                // Pauses the screen until user input
bool isValidProductID(int id, int excludeIndex = -1); // Checks if product ID is unique (optional exclude during update)
void displayItemTable(const vector<StockItem>& items); // Displays a list of items in table format
bool confirmAction(const string& message); // Asks user for confirmation (yes/no)



// Main program loop: loads data, shows menu, handles user choices
int main() {

    loadStockFromFile();
    loadGrandTotalFromFile();

    int choice;
    do {
        displayMainMenu();
        cout << "Enter your choice: ";
        
        while (!(cin >> choice) || choice < 1 || choice > 9) {
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Invalid input. Please enter a number between 1-9: ";
        }

        switch (choice) {
            case 1: makeSale(); break;
            case 2: addNewItem(); break;
            case 3: viewAllItems(); break;
            case 4: updateItem(); break;
            case 5: deleteItem(); break;
            case 6: searchItem(); break;
            case 7: lowStockAlert(); break;
            case 8: viewStockHistory(); break;
            case 9:
                saveStockToFile();
                saveGrandTotalToFile();
                logAction("Program exited successfully");
                cout << "Data saved successfully. Good Bye....." << endl;
                break;
        }
    } while (choice != 9);

    return 0;
}

// Clears the console screen
void clearScreen() {
    #ifdef _WIN32
        system("cls"); // For Windows systems
    #else
        system("clear"); // For Mac/Linux systems
    #endif
}

// Displays the main menu and summary info
void displayMainMenu() {
    clearScreen();
    cout << "=== STOCK MANAGEMENT SYSTEM ===" << endl;
    cout << "1. Make a Sale" << endl;
    cout << "2. Add New Item" << endl;
    cout << "3. View All Items" << endl;
    cout << "4. Update Item" << endl;
    cout << "5. Delete Item" << endl;
    cout << "6. Search Item by Name" << endl;
    cout << "7. Low Stock Alert" << endl;
    cout << "8. View Stock History" << endl;
    cout << "9. Exit" << endl;
    cout << "===============================" << endl;
    cout << "Total Revenue: $" << fixed << setprecision(2) << grandTotalSales 
         << " | Items in Stock: " << stock.size() << endl;
    cout << "===============================" << endl;
}

// Pauses and waits for user to press Enter
void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Checks if a product ID is valid and unique (optionally excluding an index)
bool isValidProductID(int id, int excludeIndex) {
    if (id <= 0) return false;
    for (size_t i = 0; i < stock.size(); ++i) {
        if ((int)i != excludeIndex && stock[i].productID == id) {
            return;
        }
    }
    return true;
}

// Prompts user for Y/N confirmation
bool confirmAction(const string& message) {
    char choice;
    cout << message << " (Y/N): ";
    cin >> choice;
    return (toupper(choice) == 'Y');
}

// Displays a formatted table of stock items
void displayItemTable(const vector<StockItem>& items) {
    if (items.empty()) {
        cout << "No items to display." << endl;
        return;
    }
    
    cout << left << setw(8) << "ID" << setw(25) << "Product Name" << setw(15) << "Category"
         << setw(8) << "Qty" << setw(12) << "Last Price" << setw(8) << "Status" << endl;
    cout << string(80, '-') << endl;

    for (const auto& item : items) {
        cout << left << setw(8) << item.productID 
             << setw(25) << (item.name.length() > 23 ? item.name.substr(0, 22) + "..." : item.name)
             << setw(15) << (item.category.length() > 13 ? item.category.substr(0, 12) + "..." : item.category)
             << setw(8) << item.quantity
             << setw(12) << (item.lastPrice > 0 ? "$" + to_string(item.lastPrice).substr(0, 8) : "Not Set");
        
        if (item.quantity == 0) cout << "OUT";
        else if (item.quantity < 15) cout << "LOW";
        else cout << "OK";
        
        cout << endl;
    }
    cout << string(80, '-') << endl;
}

// Loads the grand total sales from file
void loadGrandTotalFromFile() {
    ifstream file("grand_total.dat");
    if (file.is_open()) {
        file >> grandTotalSales;
        file.close();
    }
}

// Saves the grand total sales to file
void saveGrandTotalToFile() {
    ofstream file("grand_total.dat");
    if (file.is_open()) {
        file << grandTotalSales;
        file.close();
    }
}

// Loads stock data from file into memory
void loadStockFromFile() {
    ifstream file("stock.dat");
    if (!file.is_open()) {
        cout << "No existing stock file found. Starting fresh." << endl;
        pauseScreen();
        return;
    }
    
    stock.clear();
    StockItem item;
    while (file >> item.productID && file.ignore() &&
           getline(file, item.name) &&
           getline(file, item.category) &&
           file >> item.quantity >> item.lastPrice >> item.dateAdded && file.ignore()) {
        stock.push_back(item);
    }
    file.close();
    logAction("Stock data loaded successfully (" + to_string(stock.size()) + " items)");
}

// Saves current stock data to file
void saveStockToFile() {
    ofstream file("stock.dat");
    if (!file.is_open()) {
        cerr << "Error saving stock data!" << endl;
        return;
    }
    
    for (const auto& item : stock) {
        file << item.productID << endl
             << item.name << endl
             << item.category << endl
             << item.quantity << endl
             << item.lastPrice << endl
             << item.dateAdded << endl;
    }
    file.close();
}

// Logs an action to the history and memory
void logAction(const string& action) {
    time_t now = time(0);
    char* dt = ctime(&now);
    string timestamp(dt);
    timestamp.erase(timestamp.length()-1);
    
    ofstream historyFile("history.log", ios::app);
    if (historyFile.is_open()) {
        historyFile << "[" << timestamp << "] " << action << endl;
        historyFile.close();
    }
    
    historyLog.push_back("[" + timestamp + "] " + action);
}

// Handles the sale of items, updates stock and sales
void makeSale() {
    clearScreen();
    cout << "=== MAKE A SALE ===" << endl;

    if (stock.empty()) {
        cout << "No items in stock to sell." << endl;
        pauseScreen();
        return;
    }

    double sessionTotal = 0.0;
    int salesCount = 0;
    string again;
    
    do {
        clearScreen();
        cout << "=== MAKE A SALE ===" << endl;
        cout << "Session Total: $" << fixed << setprecision(2) << sessionTotal 
             << " | Sales Made: " << salesCount << endl;
        cout << string(70, '-') << endl;
        
        displayItemTable(stock);

        int choice = 0;
        cout << "\nSelect item number to sell (0 to finish): ";
        while (!(cin >> choice) || choice < 0 || choice > (int)stock.size()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please enter 0-" << stock.size() << ": ";
        }
        
        if (choice == 0) {
            if (salesCount > 0) {
                cout << "\nSale session completed!" << endl;
                cout << "Items sold: " << salesCount << " | Session total: $" << sessionTotal << endl;
            } else {
                cout << "No sales made." << endl;
            }
            break;
        }

        auto& item = stock[choice - 1];
        if (item.quantity == 0) {
            cout << "\n" << item.name << " is out of stock!" << endl;
            pauseScreen();
            continue;
        }
        
        cout << "\nSelected: " << item.name << " (Available: " << item.quantity << ")" << endl;
        
        int sellQty;
        do {
            cout << "Enter quantity to sell (0 to cancel): ";
            if (!(cin >> sellQty)) {
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid number." << endl;
                continue;
            }
            if (sellQty == 0) {
                cout << "Sale cancelled." << endl;
                 continue;
            }
            if (sellQty < 0 || sellQty > item.quantity) {
                cout << "Invalid quantity. Available: " << item.quantity << endl;
                continue;
            }
            break;
        } while (true);
        
        double price;
        do {
            cout << "Enter price per unit";
            if (item.lastPrice > 0) {
                cout << " (last: $" << item.lastPrice << ")";
            }
            cout << ": $";
            
            if (!(cin >> price) || price < 0) {
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid price." << endl;
                continue;
            }
            break;
        } while (true);
        
        double total = price * sellQty;
        sessionTotal += total;
        grandTotalSales += total;
        item.quantity -= sellQty;
        item.lastPrice = price;
        salesCount++;
        
        saveStockToFile();
        saveGrandTotalToFile();
        
        logAction("SALE: " + to_string(sellQty) + "x " + item.name + " @ $" + 
                 to_string(price) + " each = $" + to_string(total) + 
                 " (Remaining: " + to_string(item.quantity) + ")");
        
        cout << "\nSale recorded successfully!" << endl;
        cout << "Sale amount: $" << total << " | Remaining stock: " << item.quantity << endl;
        
        continue_sales:
        cout << "\nContinue selling? (Y/N): ";
        cin.ignore();
        getline(cin, again);
        
    } while (again == "Y" || again == "y");
    
    pauseScreen();
}

// Displays the stock history log and summary
void viewStockHistory() {
    clearScreen();
    cout << "=== STOCK HISTORY LOG ===" << endl;

    ifstream historyFile("history.log");
    if (!historyFile.is_open()) {
        cout << "No history log found." << endl;
        pauseScreen();
        return;
    }
    
    vector<string> allLogs;
    string line;
    int salesCount = 0, addCount = 0, updateCount = 0, deleteCount = 0;
    
    while (getline(historyFile, line)) {
        allLogs.push_back(line);
        if (line.find("SALE:") != string::npos) salesCount++;
        else if (line.find("Added") != string::npos || line.find("NEW ITEM") != string::npos) addCount++;
        else if (line.find("Updated") != string::npos || line.find("UPDATE") != string::npos) updateCount++;
        else if (line.find("Deleted") != string::npos || line.find("DELETE") != string::npos) deleteCount++;
    }
    historyFile.close();
    
    cout << "SUMMARY: " << allLogs.size() << " total actions | " 
         << salesCount << " sales | " << addCount << " additions | "
         << updateCount << " updates | " << deleteCount << " deletions" << endl;
    cout << string(80, '-') << endl;
    
    // Show recent entries (last 20)
    int startIndex = max(0, (int)allLogs.size() - 20);
    cout << "Recent Activities (last " << (allLogs.size() - startIndex) << " entries):" << endl;
    cout << string(80, '-') << endl;
    
    for (size_t i = startIndex; i < allLogs.size(); ++i) {
        cout << allLogs[i] << endl;
    }
    
    pauseScreen();
}

// Adds a new item to the stock, or restocks existing
void addNewItem() {
    clearScreen();
    cout << "=== ADD NEW ITEM ===" << endl;

    string again;
    int itemsAdded = 0;

    do {
        clearScreen();
        cout << "=== ADD NEW ITEM ===" << endl;
        cout << "Items added this session: " << itemsAdded << endl;
        cout << string(50, '-') << endl;

        StockItem newItem;
        newItem.lastPrice = 0.0;
        newItem.dateAdded = time(0);

        // Product ID with validation
        do {
            cout << "Enter product ID: ";
            if (!(cin >> newItem.productID)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid number." << endl;
                continue;
            }
            if (!isValidProductID(newItem.productID)) {
                cout << "Product ID must be positive and unique." << endl;
                continue;
            }
            break;
        } while (true);

        cin.ignore();

        // Item name with duplicate checking
        do {
            cout << "Enter item name: ";
            getline(cin, newItem.name);
            if (newItem.name.empty()) {
                cout << "Item name cannot be empty." << endl;
                continue;
            }

            // Check for duplicate names
            auto it = find_if(stock.begin(), stock.end(), [&](const StockItem& item) {
                return item.name == newItem.name;
            });

            if (it != stock.end()) {
                cout << "Item '" << newItem.name << "' already exists!" << endl;
                if (confirmAction("Add more quantity to existing item?")) {
                    int addQty;
                    cout << "Current stock: " << it->quantity << endl;
                    cout << "Enter quantity to add: ";
                    while (!(cin >> addQty) || addQty < 0) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Please enter a positive number: ";
                    }
                    it->quantity += addQty;
                    saveStockToFile();
                    logAction("RESTOCK: Added " + to_string(addQty) + " units to " + it->name +
                              " (New total: " + to_string(it->quantity) + ")");
                    cout << "Stock updated! New quantity: " << it->quantity << endl;
                    itemsAdded++;
                    goto ask_continue;
                }
                continue;
            }
            break;
        } while (true);

        // Category selection
        cout << "\nSelect category:" << endl;
        for (size_t i = 0; i < categoryOptions.size(); ++i) {
            cout << "  " << (i + 1) << ". " << categoryOptions[i] << endl;
        }

        int catChoice;
        do {
            cout << "Enter category number (1-" << categoryOptions.size() << "): ";
            if (!(cin >> catChoice) || catChoice < 1 || catChoice > (int)categoryOptions.size()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid choice." << endl;
                continue;
            }
            break;
        } while (true);

        newItem.category = categoryOptions[catChoice - 1];

        // Enter initial price
        do {
            cout << "Enter initial price: $";
            if (!(cin >> newItem.lastPrice) || newItem.lastPrice < 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid price." << endl;
                continue;
            }
            break;
        } while (true);

        // FIXED: ignore leftover newline before entering quantity
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Enter initial quantity
        do {
            cout << "Enter initial quantity: ";
            if (!(cin >> newItem.quantity) || newItem.quantity < 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a non-negative number." << endl;
                continue;
            }
            break;
        } while (true);

        stock.push_back(newItem);
        saveStockToFile();
        logAction("NEW ITEM: Added " + newItem.name + " (ID: " + to_string(newItem.productID) +
                  ", Category: " + newItem.category + ", Qty: " + to_string(newItem.quantity) + ")");

        cout << "Item '" << newItem.name << "' added successfully!" << endl;
        itemsAdded++;

    ask_continue:
        cout << "\nAdd another item? (Y/N): ";
        cin.ignore();
        getline(cin, again);

    } while (again == "Y" || again == "y");

    cout << "\nSession complete! Total items added/updated: " << itemsAdded << endl;
    pauseScreen();
}

// Shows all items in stock with statistics
void viewAllItems() {
    clearScreen();
    cout << "=== ALL STOCK ITEMS ===" << endl;

    if (stock.empty()) {
        cout << "No items in stock." << endl;
        pauseScreen();
        return;
    }
    
    // Statistics
    int totalQuantity = 0, lowStockCount = 0, outOfStockCount = 0;
    map<string, int> categoryCount;
    
    for (const auto& item : stock) {
        totalQuantity += item.quantity;
        if (item.quantity == 0) outOfStockCount++;
        else if (item.quantity < 15) lowStockCount++;
        categoryCount[item.category]++;
    }
    
    cout << "OVERVIEW: " << stock.size() << " items | Total Qty: " << totalQuantity 
         << " | Out of Stock: " << outOfStockCount << " | Low Stock: " << lowStockCount << endl;
    cout << string(80, '=') << endl;
    
    displayItemTable(stock);
    
    cout << string(80, '=') << endl;
    pauseScreen();
}

// Updates details of an existing item
void updateItem() {
    clearScreen();
    cout << "=== UPDATE ITEM ===" << endl;

    if (stock.empty()) {
        cout << "No items in stock to update." << endl;
        pauseScreen();
        return;
    }

    string searchTerm;
    cout << "Enter item name to update: ";
    cin.ignore();
    getline(cin, searchTerm);

    bool found = false;
    for (auto& item : stock) {
        if (item.name == searchTerm) {
            found = true;
            cout << "\nCURRENT DETAILS:" << endl;
            cout << "  Product ID: " << item.productID << endl;
            cout << "  Name: " << item.name << endl;
            cout << "  Category: " << item.category << endl;
            cout << "  Quantity: " << item.quantity << endl;
            cout << "  Last Price: $" << item.lastPrice << endl;
            
            cout << "\nWhat would you like to update?" << endl;
            cout << "1. Product ID" << endl;
            cout << "2. Name" << endl;
            cout << "3. Category" << endl;
            cout << "4. Quantity" << endl;
            cout << "5. Last Price" << endl;
            cout << "6. All fields" << endl;
            cout << "Choice: ";
            
            int updateChoice;
            while (!(cin >> updateChoice) || updateChoice < 1 || updateChoice > 5) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid choice (1-5): ";
            }
            cin.ignore();
            
            string oldValues = item.name + " (ID:" + to_string(item.productID) + ")";
            
            switch (updateChoice) {
                case 1: {
                    int newID;
                    do {
                        cout << "New Product ID: ";
                        if (!(cin >> newID)) {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Invalid input." << endl;
                            continue;
                        }
                        if (!isValidProductID(newID, &item - &stock[0])) {
                            cout << "ID must be positive and unique." << endl;
                            continue;
                        }
                        item.productID = newID;
                        break;
                    } while (true);
                    break;
                }
                case 2: {
                    cout << "New name: ";
                    getline(cin, item.name);
                    break;
                }
                case 3: {
                    cout << "Choose new category:" << endl;
                    for (size_t i = 0; i < categoryOptions.size(); ++i) {
                        cout << i + 1 << ". " << categoryOptions[i] << endl;
                    }
                    int catChoice;
                    cout << "Category number: ";
                    if (cin >> catChoice && catChoice >= 1 && catChoice <= (int)categoryOptions.size()) {
                        item.category = categoryOptions[catChoice - 1];
                    }
                    cin.ignore();
                    break;
                }
                case 4: {
                    cout << "New quantity: ";
                    int newQty;
                    if (cin >> newQty && newQty >= 0) {
                        item.quantity = newQty;
                    }
                    cin.ignore();
                    break;
                }
                   case 5: {
                    string input;
                    cout << "Last Price [$" << item.lastPrice << "]: ";
                    getline(cin, input);
                    if (!input.empty()) {
                        try {
                            item.lastPrice = stod(input);
                        } catch (...) {
                            cout << "Invalid price. Keeping current value." << endl;
                        }
                    }
                    break;
                }

                case 6: {
                    cout << "Enter new details (leave blank to keep current):" << endl;
                    
                    string input;
                    cout << "Product ID [" << item.productID << "]: ";
                    getline(cin, input);
                    if (!input.empty()) {
                        try {
                            int newID = stoi(input);
                            if (isValidProductID(newID, &item - &stock[0])) {
                                item.productID = newID;
                            }
                        } catch (...) {
                            cout << "Invalid Product ID. Keeping current value." << endl;
                        }
                    }

                    cout << "Name [" << item.name << "]: ";
                    getline(cin, input);
                    if (!input.empty()) item.name = input;

                    cout << "Choose new category (or press Enter to keep current: " << item.category << "):" << endl;
                    for (size_t i = 0; i < categoryOptions.size(); ++i)
                        cout << i + 1 << ". " << categoryOptions[i] << endl;
                    getline(cin, input);
                    if (!input.empty()) {
                        try {
                            int catChoice = stoi(input);
                            if (catChoice >= 1 && catChoice <= (int)categoryOptions.size())
                                item.category = categoryOptions[catChoice - 1];
                        } catch (...) {}
                    }

                    cout << "Quantity [" << item.quantity << "]: ";
                    getline(cin, input);
                    if (!input.empty()) {
                        try {
                            item.quantity = stoi(input);
                        } catch (...) {
                            cout << "Invalid quantity. Keeping current value." << endl;
                        }
                    }

                    cout << "Last Price [$" << item.lastPrice << "]: ";
                    getline(cin, input);
                    if (!input.empty()) {
                        try {
                            item.lastPrice = stod(input);
                        } catch (...) {
                            cout << "Invalid price. Keeping current value." << endl;
                        }
                    }
                    break;
                }
            }

            saveStockToFile();
            logAction("UPDATE: " + oldValues + " -> Updated successfully");
            cout << "Item updated successfully!" << endl;
            break;
        }
    }

    if (!found) {
        cout << "Item not found." << endl;
    }

    pauseScreen();
}

// Deletes an item from the stock
void deleteItem() {
    clearScreen();
    cout << "=== DELETE ITEM ===" << endl;

    if (stock.empty()) {
        cout << "No items in stock to delete." << endl;
        pauseScreen();
        return;
    }

    string searchTerm;
    cout << "Enter item name to delete: ";
    cin.ignore();
    getline(cin, searchTerm);

    auto it = stock.begin();
    bool found = false;
    StockItem itemToDelete;

    for (; it != stock.end(); ++it) {
        if (it->name == searchTerm) {
            found = true;
            itemToDelete = *it;
            break;
        }
    }

    if (found) {
        cout << "\nITEM TO DELETE:" << endl;
        cout << "  Name: " << itemToDelete.name << endl;
        cout << "  ID: " << itemToDelete.productID << endl;
        cout << "  Category: " << itemToDelete.category << endl;
        cout << "  Quantity: " << itemToDelete.quantity << endl;
        
        if (itemToDelete.quantity > 0) {
            cout << "\nWARNING: This item has " << itemToDelete.quantity << " units in stock!" << endl;
        }
        
        if (confirmAction("\nAre you sure you want to delete this item?")) {
            stock.erase(it);
            saveStockToFile();
            logAction("DELETE: Removed " + itemToDelete.name + " (ID: " + to_string(itemToDelete.productID) + 
                     ", Had " + to_string(itemToDelete.quantity) + " units)");
            cout << "Item '" << itemToDelete.name << "' deleted successfully!" << endl;
        } else {
            cout << "Deletion cancelled." << endl;
        }
    } else {
        cout << "Item '" << searchTerm << "' not found." << endl;
    }

    pauseScreen();
}

// Searches for items by name or category
void searchItem() {
    clearScreen();
    cout << "=== SEARCH ITEMS ===" << endl;

    if (stock.empty()) {
        cout << "No items in stock to search." << endl;
        pauseScreen();
        return;
    }

    string again;
    cin.ignore();
    
    do {
        string searchTerm;
        cout << "\nEnter search term (name or category): ";
        getline(cin, searchTerm);
        
        // Convert to lowercase for case-insensitive search
        transform(searchTerm.begin(), searchTerm.end(), searchTerm.begin(), ::tolower);

        vector<StockItem> results;
        
        for (const auto& item : stock) {
            string itemName = item.name;
            string itemCategory = item.category;
            transform(itemName.begin(), itemName.end(), itemName.begin(), ::tolower);
            transform(itemCategory.begin(), itemCategory.end(), itemCategory.begin(), ::tolower);
            
            if (itemName.find(searchTerm) != string::npos || 
                itemCategory.find(searchTerm) != string::npos) {
                results.push_back(item);
            }
        }

        cout << "\nSEARCH RESULTS for '" << searchTerm << "':" << endl;
        
        if (results.empty()) {
            cout << "No items found matching your search." << endl;
        } else {
            cout << "Found " << results.size() << " item(s)" << endl;
            cout << string(70, '-') << endl;
            displayItemTable(results);
        }

        cout << "\nSearch for another item? (Y/N): ";
        getline(cin, again);

    } while (again == "Y" || again == "y");

    pauseScreen();
}

// Alerts for items with low or zero stock
void lowStockAlert() {
    clearScreen();
    cout << "=== LOW STOCK ALERT ===" << endl;
    
    int threshold = 15;
    cout << "Current threshold: " << threshold << " units" << endl;
    
    if (confirmAction("Change threshold?")) {
        cout << "Enter new threshold: ";
        while (!(cin >> threshold) || threshold < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a positive number: ";
        }
    }
    
    cout << "\nITEMS WITH STOCK BELOW " << threshold << " UNITS:" << endl;

    vector<StockItem> lowStockItems;
    vector<StockItem> outOfStockItems;
    
    for (const auto& item : stock) {
        if (item.quantity == 0) {
            outOfStockItems.push_back(item);
        } else if (item.quantity < threshold) {
            lowStockItems.push_back(item);
        }
    }
    
    if (!outOfStockItems.empty()) {
        cout << "\nCRITICAL - OUT OF STOCK (" << outOfStockItems.size() << " items):" << endl;
        cout << string(65, '-') << endl;
        displayItemTable(outOfStockItems);
    }
    
    if (!lowStockItems.empty()) {
        cout << "\nLOW STOCK (" << lowStockItems.size() << " items):" << endl;
        cout << string(70, '-') << endl;
        displayItemTable(lowStockItems);
    }
    
    if (lowStockItems.empty() && outOfStockItems.empty()) {
        cout << "All items are well stocked! No alerts." << endl;
    } else {
        cout << "\nSUMMARY:" << endl;
        cout << "Out of stock: " << outOfStockItems.size() << " items" << endl;
        cout << "Low stock: " << lowStockItems.size() << " items" << endl;
        cout << "Action needed for: " << (lowStockItems.size() + outOfStockItems.size()) << " items" << endl;
    }

    pauseScreen();
}