#include <iostream>
#include <mysql/mysql.h>
using namespace std;

MYSQL* conn;
MYSQL_ROW row;
MYSQL_RES* res;

// Connect to MySQL database
void connectDatabase() {
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "localhost", "contact_user", "cms123", "contact_db", 3306, NULL, 0);
    if (conn) {
        cout << "Connected to database successfully!\n";
    } else {
        cout << "Connection failed: " << mysql_error(conn) << endl;
        exit(1);
    }
}

// Add a new contact
void addContact() {
    string name, phone, email, address;
    cout << "Enter Name: "; cin >> ws; getline(cin, name);
    cout << "Enter Phone: "; getline(cin, phone);
    cout << "Enter Email: "; getline(cin, email);
    cout << "Enter Address: "; getline(cin, address);

    string query = "INSERT INTO contacts(name, phone, email, address) VALUES('"
                    + name + "','" + phone + "','" + email + "','" + address + "')";
    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Contact added successfully!\n";
    } else {
        cout << "Error: " << mysql_error(conn) << endl;
    }
}

// View all contacts
#include <iomanip>  // add at top

// View all contacts
void viewContacts() {
    if (mysql_query(conn, "SELECT * FROM contacts")) {
        cout << "Query Failed: " << mysql_error(conn) << endl;
        return;
    }
    res = mysql_store_result(conn);

    // Print header with fixed widths
    cout << left << setw(5)  << "ID"
         << setw(20) << "Name"
         << setw(15) << "Phone"
         << setw(25) << "Email"
         << setw(30) << "Address" << "\n";

    cout << string(95, '-') << "\n"; // separator line

    // Print rows
    while ((row = mysql_fetch_row(res))) {
        cout << left << setw(5)  << row[0]
             << setw(20) << row[1]
             << setw(15) << row[2]
             << setw(25) << row[3]
             << setw(30) << row[4] << "\n";
    }
}

// Search contact by name
void searchContact() {
    string searchName;
    cout << "Enter name to search: "; cin >> ws; getline(cin, searchName);
    string query = "SELECT * FROM contacts WHERE name LIKE '%" + searchName + "%'";
    if (mysql_query(conn, query.c_str())) {
        cout << "Query failed: " << mysql_error(conn) << endl;
        return;
    }
    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res))) {
        cout << "ID: " << row[0] << ", Name: " << row[1] 
             << ", Phone: " << row[2] << ", Email: " << row[3] 
             << ", Address: " << row[4] << "\n";
    }
}

// Update a contact
void updateContact() {
    int id;
    cout << "Enter ID of contact to update: "; cin >> id;
    string name, phone, email, address;
    cout << "Enter new Name: "; cin >> ws; getline(cin, name);
    cout << "Enter new Phone: "; getline(cin, phone);
    cout << "Enter new Email: "; getline(cin, email);
    cout << "Enter new Address: "; getline(cin, address);

    string query = "UPDATE contacts SET name='" + name + "', phone='" + phone + 
                   "', email='" + email + "', address='" + address + "' WHERE id=" + to_string(id);
    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Contact updated successfully!\n";
    } else {
        cout << "Error: " << mysql_error(conn) << endl;
    }
}

// Delete a contact
void deleteContact() {
    int id;
    cout << "Enter ID of contact to delete: "; cin >> id;
    string query = "DELETE FROM contacts WHERE id=" + to_string(id);
    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Contact deleted successfully!\n";
    } else {
        cout << "Error: " << mysql_error(conn) << endl;
    }
}

int main() {
    connectDatabase();
    int choice;
    do {
        cout << "\n--- Contact Management System ---\n";
        cout << "1. Add Contact\n2. View Contacts\n3. Search Contact\n4. Update Contact\n5. Delete Contact\n6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addContact(); break;
            case 2: viewContacts(); break;
            case 3: searchContact(); break;
            case 4: updateContact(); break;
            case 5: deleteContact(); break;
            case 6: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 6);

    mysql_close(conn);
    return 0;
}


