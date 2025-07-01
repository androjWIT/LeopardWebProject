#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>

using namespace std;

// Functions
sqlite3* openDatabase(const string& dbName);
pair<int, string> login(sqlite3* db);
void studentMenu(sqlite3* db, const string& email);
void searchCourses(sqlite3* db);
void searchCoursesByParam(sqlite3* db);
void addCourseToSchedule(sqlite3* db, const string& email);
void removeCourseFromSchedule(sqlite3* db, const string& email);
void printSchedule(sqlite3* db, const string& email);
bool hasConflict(sqlite3* db, const string& email, const string& crn);
void addStudentLogin(sqlite3* db);



void addStudent(sqlite3* db);
void removeInstructor(sqlite3* db);
void updateAdmin(sqlite3* db);
int menu(sqlite3* db);
sqlite3* openDatabase(const string& dbName);
void displayTable(sqlite3* db, const string& tableName);
void createCourseTable(sqlite3* db);
void addCourse(sqlite3* db);
// Callback function for displaying results
static int callback(void*, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\n";
    }
    cout << endl;
    return 0;
}

int main(int argc, char** argv) {
    sqlite3* DB = openDatabase("assignment3.db");
    if (DB == nullptr) return -1;

    int option;
    cout << "1. Sign in\n";
    cout << "2. Add new student login\n";
    cout << "Enter your choice: ";
    cin >> option;

    if (option == 2) {
        addStudentLogin(DB);
        sqlite3_close(DB);
        return 0;
    }

    pair<int, string> loginResult = login(DB);
    int role = loginResult.first;
    string email = loginResult.second;


    switch (role) {
    case 1:
        studentMenu(DB, email);
        break;
    case 2:
        // Instructor menu would go here
        break;
    case 3:
        // Admin menu would go here
        break;
    default:
        cout << "Login failed. Exiting.\n";
        break;
    }

    sqlite3_close(DB);
    return 0;
}

sqlite3* openDatabase(const string& dbName) {
    sqlite3* db;
    if (sqlite3_open(dbName.c_str(), &db)) {
        cerr << "Can't open DB: " << sqlite3_errmsg(db) << endl;
        return nullptr;
    }

    const char* scheduleSQL =
        "CREATE TABLE IF NOT EXISTS SCHEDULE ("
        "STUDENTEMAIL TEXT, CRN TEXT,"
        "FOREIGN KEY(STUDENTEMAIL) REFERENCES STUDENT(EMAIL),"
        "FOREIGN KEY(CRN) REFERENCES COURSE(CRN));";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, scheduleSQL, 0, 0, &errMsg) != SQLITE_OK) {
        cerr << "Failed to create schedule table: " << errMsg << endl;
        sqlite3_free(errMsg);
    }

    const char* studentLoginSQL =
        "CREATE TABLE IF NOT EXISTS STUDENTLOGINS ("
        "USERSEMAIL TEXT PRIMARY KEY, "
        "USERSPASSWORD TEXT);";

    if (sqlite3_exec(db, studentLoginSQL, 0, 0, &errMsg) != SQLITE_OK) {
        cerr << "Failed to create STUDENTLOGINS table: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
    return db;
}

pair<int, string> login(sqlite3* db) {
    string email, password;
    cout << "Email: ";
    cin >> email;
    cout << "Password: ";
    cin >> password;

    const char* sql = "SELECT COUNT(*) FROM STUDENTLOGINS WHERE USERSEMAIL = ? AND USERSPASSWORD = ?";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return { 0, "" };

    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);

    int role = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0)
        role = 1;

    sqlite3_finalize(stmt);
    return { role, email };
}

void addStudentLogin(sqlite3* db) {
    string email, password;

    cout << "Enter new student email: ";
    cin >> email;
    cout << "Enter password: ";
    cin >> password;

    string sql = "INSERT INTO STUDENTLOGINS (USERSEMAIL, USERSPASSWORD) VALUES (?, ?)";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare insert: " << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Insert failed: " << sqlite3_errmsg(db) << endl;
    }
    else {
        cout << "Student login added successfully.\n";
    }

    sqlite3_finalize(stmt);
}

void studentMenu(sqlite3* db, const string& email) {
    int choice;
    do {
        cout << "\n--- Student Menu ---\n";
        cout << "1. Search all courses\n";
        cout << "2. Search courses by department\n";
        cout << "3. Add course to schedule\n";
        cout << "4. Remove course from schedule\n";
        cout << "5. View schedule\n";
        cout << "6. Add student login\n";
        cout << "0. Logout\n> ";

        cin >> choice;

        switch (choice) {
        case 1:
            searchCourses(db);
            break;
        case 2:
            searchCoursesByParam(db);
            break;
        case 3:
            addCourseToSchedule(db, email);
            break;
        case 4:
            removeCourseFromSchedule(db, email);
            break;
        case 5:
            printSchedule(db, email);
            break;
        case 6:
            addStudentLogin(db);
            break;
        case 0:
            cout << "Logging out...\n";
            break;
        default:
            cout << "Error.\n";
        }
    } while (choice != 0);
}
/*
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



*/



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






void searchCourses(sqlite3* db) {
    string sql = "SELECT * FROM COURSE ORDER BY DEPARTMENT;";
    sqlite3_exec(db, sql.c_str(), callback, nullptr, nullptr);
}

void searchCoursesByParam(sqlite3* db) {
    string dept;
    cout << "Enter department (e.g., MATH): ";
    cin >> dept;

    string sql = "SELECT * FROM COURSE WHERE DEPARTMENT = '" + dept + "';";
    sqlite3_exec(db, sql.c_str(), callback, nullptr, nullptr);
}

void addCourseToSchedule(sqlite3* db, const string& email) {
    string crn;
    cout << "Enter CRN to add: ";
    cin >> crn;

    if (hasConflict(db, email, crn)) {
        cout << "Conflict detected. Course not added.\n";
        return;
    }

    string sql = "INSERT INTO SCHEDULE (STUDENTEMAIL, CRN) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, crn.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_DONE)
        cout << "Course added.\n";
    else
        cout << "Failed to add course.\n";

    sqlite3_finalize(stmt);
}

void removeCourseFromSchedule(sqlite3* db, const string& email) {
    string crn;
    cout << "Enter CRN to remove: ";
    cin >> crn;

    string sql = "DELETE FROM SCHEDULE WHERE STUDENTEMAIL = ? AND CRN = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, crn.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_DONE)
        cout << "Course removed.\n";
    else
        cout << "Failed to remove course.\n";

    sqlite3_finalize(stmt);
}

void printSchedule(sqlite3* db, const string& email) {
    string sql =
        "SELECT COURSE.CRN, TITLE, DEPARTMENT, TIME, DAYS, SEMESTER, YEAR, CREDITS "
        "FROM COURSE INNER JOIN SCHEDULE ON COURSE.CRN = SCHEDULE.CRN "
        "WHERE STUDENTEMAIL = '" + email + "';";

    cout << "\n--- Your Schedule ---\n";
    sqlite3_exec(db, sql.c_str(), callback, nullptr, nullptr);
}

bool hasConflict(sqlite3* db, const string& email, const string& newCRN) {
    string sql =
        "SELECT 1 FROM COURSE C1, COURSE C2, SCHEDULE S "
        "WHERE C1.CRN = ? AND C2.CRN = S.CRN AND S.STUDENTEMAIL = ? "
        "AND C1.DAYS = C2.DAYS AND C1.TIME = C2.TIME;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, newCRN.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_TRANSIENT);

    bool conflict = false;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        conflict = true;

    sqlite3_finalize(stmt);
    return conflict;
}
