#include <iostream>
#include <sqlite3.h>
#include <string>
#include <stdio.h>

using namespace std;

static int callback(void* data, int argc, char** argv, char** azColName)
{
    int i;

    for (i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");

    return 0;
}
void addStudent(sqlite3* db);
void removeInstructor(sqlite3* db);
void updateAdmin(sqlite3* db);
int menu(sqlite3* db);
sqlite3* openDatabase(const string& dbName);
void displayTable(sqlite3* db, const string& tableName);
void createCourseTable(sqlite3* db);
void addCourse(sqlite3* db);




int main(int argc, char** argv) {

    sqlite3* DB = openDatabase("assignment3.db");
    if (DB == nullptr) return -1;

    //sqlite3* DB;
    //int exit = 0;
    //exit = sqlite3_open("assignment3.db", &DB);

    //if (exit) {
    //    std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
    //    return (-1);
    //}
    //else
    //    std::cout << "Opened Database Successfully!" << std::endl;


    menu(DB);
    sqlite3_close(DB);


    return 0;
}



int menu(sqlite3* db) {
    cout << "Menu of actions to do: \n";
    cout << "1. Add a student\n";
    cout << "2. Remove an instructor\n";
    cout << "3. Update admin\n";
    cout << "4. Add a course\n";
    cout << "5. Display Tables\n";
    cout << "Please enter action: \n";

    int a;
    cin >> a;
    switch (a) {
    case 1:
        addStudent(db);
        break;
    case 2:
        removeInstructor(db);
        break;
    case 3:
        updateAdmin(db);
        break;
    case 4:
        addCourse(db);
        break;
    case 5: {
        string tableName;
        cout << "Enter table name to display (e.g., STUDENT, INSTRUCTOR, ADMIN): ";
        cin >> tableName;
        displayTable(db, tableName);
        break;
    }
    default:
        cout << "Please enter a valid option.\n";
    }
    return 0;
}





void addStudent(sqlite3* db) {
    string ID, FName, Lname, gradYear, major, email;
    cout << "Hello please enter Studnet information below: \n";
    cout << "Enter ID: \n";
    cin >> ID;
    cout << "Enter first name: \n";
    cin >> FName;
    cout << "Enter last name: \n";
    cin >> Lname;
    cout << "Enter expected graduation year: \n";
    cin >> gradYear;
    cout << "Enter major: \n";
    cin >> major;
    cout << "Enter email: \n";
    cin >> email;

    string sql = "INSERT INTO STUDENT( ID, NAME, SURNAME, GRADYEAR, MAJOR, EMAIL) VALUES (?,?,?,?,?,?)";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    //bind pramaters
    sqlite3_bind_text(stmt, 1, ID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, FName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, Lname.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, gradYear.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, major.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, email.c_str(), -1, SQLITE_TRANSIENT);


    // error checking for the database
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Insert failed: " << sqlite3_errmsg(db) << endl;
    }
    else {
        cout << "Student added successfully.\n";
    }

    sqlite3_finalize(stmt);

}

void removeInstructor(sqlite3* db) {
    sqlite3_stmt* stmt;
    string userID;

    cout << "Please enter the ID of the Instructor you would like to remove: ";
    cin >> userID;

    // DELETE SQL command with parameter placeholder
    string sql = "DELETE FROM INSTRUCTOR WHERE ID = ?";

    // Prepare the statement first
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare DELETE statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    // THEN bind the parameter AFTER stmt is prepared
    sqlite3_bind_text(stmt, 1, userID.c_str(), -1, SQLITE_TRANSIENT);

    // Execute
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Delete Instructor failed: " << sqlite3_errmsg(db) << endl;
    }
    else {
        // Optional check: was anything actually deleted?
        if (sqlite3_changes(db) == 0) {
            cout << "No instructor with that ID was found.\n";
        }
        else {
            cout << "Instructor successfully deleted.\n";
        }
    }


    sqlite3_finalize(stmt);
}

void updateAdmin(sqlite3* db) {
    string adminID, firstName, lastName, title, office, email;

    cout << "Enter the ID of the admin you want to update: ";
    cin >> adminID;

    cout << "Enter new first name: ";
    cin.ignore(); // clear newline from previous input
    getline(cin, firstName);

    cout << "Enter new last name: ";
    getline(cin, lastName);

    cout << "Enter new title: ";
    getline(cin, title);

    cout << "Enter new office: ";
    getline(cin, office);

    cout << "Enter new email: ";
    getline(cin, email);

    string sql = "UPDATE ADMIN SET FIRSTNAME = ?, LASTNAME = ?, TITLE = ?, OFFICE = ?, EMAIL = ? WHERE ID = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare UPDATE statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    // Bind new values to the prepared statement
    sqlite3_bind_text(stmt, 1, firstName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, lastName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, office.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, adminID.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Update failed: " << sqlite3_errmsg(db) << endl;
    }
    else {
        if (sqlite3_changes(db) == 0) {
            cout << "No admin found with that ID.\n";
        }
        else {
            cout << "Admin updated successfully.\n";
        }
    }

    sqlite3_finalize(stmt);
}

sqlite3* openDatabase(const string& dbName) {
    sqlite3* db;
    int rc = sqlite3_open(dbName.c_str(), &db);

    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return nullptr;
    }

    cout << "Opened database successfully.\n";

    // Create the COURSE table
    createCourseTable(db);

    return db;
}

void displayTable(sqlite3* db, const string& tableName) {
    string sql = "SELECT * FROM " + tableName + ";";
    cout << "Contents of table '" << tableName << "':\n";
    char* errMsg = nullptr;

    int rc = sqlite3_exec(db, sql.c_str(), callback, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error displaying table: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

void createCourseTable(sqlite3* db) {
    string sql = R"(
        CREATE TABLE IF NOT EXISTS COURSE (
            CRN TEXT PRIMARY KEY,
            TITLE TEXT NOT NULL,
            DEPARTMENT TEXT NOT NULL,
            TIME TEXT NOT NULL,
            DAYS TEXT NOT NULL,
            SEMESTER TEXT NOT NULL,
            YEAR TEXT NOT NULL,
            CREDITS INTEGER NOT NULL
        );
    )";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "Error creating COURSE table: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
    else {
        cout << "COURSE table created (if it did not exist).\n";
    }
}

void addCourse(sqlite3* db) {
    string crn, title, department, time, days, semester, year;
    int credits;

    cout << "Enter course information below:\n";
    cout << "CRN: ";
    cin >> crn;
    cout << "Title: ";
    cin.ignore();  // To flush newline from input buffer
    getline(cin, title);
    cout << "Department: ";
    getline(cin, department);
    cout << "Time (e.g., 10:00 AM): ";
    getline(cin, time);
    cout << "Days (e.g., MWF or TR): ";
    getline(cin, days);
    cout << "Semester (e.g., Fall): ";
    getline(cin, semester);
    cout << "Year: ";
    getline(cin, year);
    cout << "Credits: ";
    cin >> credits;

    string sql = "INSERT INTO COURSE (CRN, TITLE, DEPARTMENT, TIME, DAYS, SEMESTER, YEAR, CREDITS) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?)";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare INSERT statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    // Bind values
    sqlite3_bind_text(stmt, 1, crn.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, department.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, time.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, days.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, semester.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, year.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 8, credits);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Insert failed: " << sqlite3_errmsg(db) << endl;
    }
    else {
        cout << "Course added successfully.\n";
    }

    sqlite3_finalize(stmt);
}

