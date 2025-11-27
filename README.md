📦 Stock Management System (STMS)

The Stock Management System (STMS) is a console-based application developed in C++ to manage inventory efficiently. This system allows users to add, update, delete, and search products, track sales, generate reports, and maintain stock alerts. It demonstrates core programming skills including object-oriented programming (OOP), file handling, and basic data management, making it an excellent project for learning and showcasing C++ skills.

✨ Features
🛒 Product Management

Add Products: Enter product details such as ID, name, category, price, and quantity.

Update Products: Modify product information at any time.

Delete Products: Remove products from inventory.

Search Products: Find products by name or ID efficiently.

📈 Sales Management

Record Sales: Input sales transactions and update stock automatically.

Track Revenue: Calculate total revenue from sales.

Generate Sales Reports: View sales history and product performance.

⚠️ Stock Alerts

Low-Stock Notifications: Automatically alert when stock for any product is below a defined threshold.

Stock Monitoring: Helps prevent stockouts and ensures timely restocking.

📝 Action Logging

Maintains a log of all actions including adding, updating, deleting products, and sales operations.

Provides a history that can be reviewed to understand changes in the inventory.

📊 Reports and Analytics

Product List: Display all products with details.

Stock Report: Shows current quantity of products.

Revenue Report: Tracks total earnings from sales.

Optional enhancements: Category-wise reports, low-stock reports.

🛠️ Technologies Used

C++ – Core language for logic and functionality

Object-Oriented Programming (OOP) – Classes, objects, inheritance (if used), and functions

File Handling – Persistent storage of products, sales, and logs

STL Basics – Strings, vectors, and arrays for data management

Console Interface – Text-based UI for simplicity and learning purposes

📂 Project Structure (Example)
/STMS
   /data
       products.txt      # Stores product data
       sales.txt         # Stores sales history
       logs.txt          # Stores action logs
   STMS.cpp              # Main program
   README.md             # Project documentation

🚀 How to Run the Project
1. Compile the code

Use a C++ compiler such as g++:

g++ STMS.cpp -o STMS

2. Run the program
./STMS       # Linux / Mac
STMS.exe     # Windows

3. Use the menu

Follow on-screen instructions to manage products, record sales, or view reports.

Data is saved automatically in text files for persistence.

🎯 Purpose of the Project

Strengthen C++ fundamentals, including functions, classes, and file handling.

Understand inventory and stock management processes.

Practice OOP design and modular coding.

Build a real-world console application suitable for portfolio or internship projects.

Serve as a foundation for creating GUI or database-integrated versions in the future.

📌 Future Improvements

User Authentication – Add login system for different users (admin, staff).

Category-wise Management – Filter products by category.

Enhanced Reports – Include sales by date, category, or revenue trends.

GUI Version – Use Qt, WinForms, or C# for graphical interface.

Database Integration – Switch from text files to SQL/NoSQL database.

Advanced Stock Alerts – Automatic notifications for reordering.

📌 Additional Notes

Currently, the system is console-based and non-networked.

All data is stored in plain text files; security features can be added in advanced versions.

This project is ideal for beginners and intermediate C++ learners who want to practice real-world applications.
