

#include "STUDENT.h"
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>
#include "Student.h"

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
bool conflictExists(sqlite3* db, const string& email, const string& crn);
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



void studentMenu(sqlite3* db, const string& email) {
    int choice;
    do {
        cout << "\n--- Student Menu ---\n";
        cout << "1. Search all courses\n";
        cout << "2. Search courses by department\n";
        cout << "3. Add course to schedule\n";
        cout << "4. Remove course from schedule\n";
        cout << "5. View schedule\n";
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
        case 0:
            cout << "Logging out...\n";
            break;
        default:
            cout << "Error.\n";
        }
    } while (choice != 0);
}
//Use for testing to add students
void addStudent(sqlite3* db) {
    string ID, FName, Lname, gradYear, major, email;
    cout << "Enter studnet information below: \n";
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
        cerr << "Add student failed: " << sqlite3_errmsg(db) << endl;
    }
    else {
        cout << "Student added to the database.\n";
    }
    sqlite3_finalize(stmt);
}


void searchCourses(sqlite3* db) {
        std::string course_crn;
        std::cout << "What Courses would you like to search: \n";
        std::cout << "Enter course CRN here: \n";
        std::cin >> course_crn;

        std::string sql = "SELECT * FROM COURSE WHERE CRN = ?";
        sqlite3_stmt* stmt;

        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement.\n";
            return;
        }

        sqlite3_bind_text(stmt, 1, course_crn.c_str(), -1, SQLITE_TRANSIENT);

        rc = sqlite3_step(stmt);

        if (rc == SQLITE_ROW) {
            // Display formatted output
            std::cout << "CRN: " << sqlite3_column_text(stmt, 0) << "\n";
            std::cout << "Title: " << sqlite3_column_text(stmt, 1) << "\n";
            std::cout << "Department: " << sqlite3_column_text(stmt, 2) << "\n";
            std::cout << "Time: " << sqlite3_column_text(stmt, 3) << "\n";
            std::cout << "Days: " << sqlite3_column_text(stmt, 4) << "\n";
            std::cout << "Semester: " << sqlite3_column_text(stmt, 5) << "\n";
            std::cout << "Year: " << sqlite3_column_text(stmt, 6) << "\n";
            std::cout << "Instructor: " << sqlite3_column_text(stmt, 7) << "\n";
        }
        else {
            std::cout << "Course not found.\n";
        }

        sqlite3_finalize(stmt);
}

void searchCoursesByParam(sqlite3* db) {
    int choice;
    std::cout << "Enter the parameter you would like to search: \n";
    std::cout << "1. CRN:  \n";
    std::cout << "2. Title:  \n";
    std::cout << "3. Department:  \n";
    std::cin >> choice;

    std::string paramvalue;
    std::string sql; //store the sql query string
    sqlite3_stmt* stmt;

    switch (choice) {
    case 1:
        std::cout << "Enter CRN: \n";
        std::cin.ignore(); // this handles the previous new line and clears leftover newline characters in the input buffer from previous
        std::getline(std::cin, paramvalue);
        sql = "SELECT * FROM COURSE WHERE CRN = ?";
        break;

    case 2:
        std::cout << "Enter Title: \n";
        std::cin.ignore();
        std::getline(std::cin, paramvalue);
        sql = "SELECT * FROM COURSE WHERE TITLE = ?";
        break;

    case 3:
        std::cout << "Enter department: \n";
        std::cin.ignore();
        std::getline(std::cin, paramvalue);
        sql = "SELECT * FROM COURSE WHERE DEPARTMENT = ?";
        break;

    default:
        std::cout << "invalid choice. \n";
        return;

    }

    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement.\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, paramvalue.c_str(), -1, SQLITE_TRANSIENT);

    bool found = false;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        found = true;
        std::cout << "-------------------\n";
        std::cout << "CRN: " << sqlite3_column_text(stmt, 0) << "\n";
        std::cout << "Title: " << sqlite3_column_text(stmt, 1) << "\n";
        std::cout << "Department: " << sqlite3_column_text(stmt, 2) << "\n";
        std::cout << "Time: " << sqlite3_column_text(stmt, 3) << "\n";
        std::cout << "Days: " << sqlite3_column_text(stmt, 4) << "\n";
        std::cout << "Semester: " << sqlite3_column_text(stmt, 5) << "\n";
        std::cout << "Year: " << sqlite3_column_text(stmt, 6) << "\n";
        std::cout << "Instructor: " << sqlite3_column_text(stmt, 7) << "\n";
    }
    if (!found) {
        std::cout << "No matching courses found.\n";
    }
    sqlite3_finalize(stmt);
}

void addCourseToSchedule(sqlite3* db, const string& email) {
    string crn;
    cout << "Enter CRN to add: ";
    cin >> crn;

    // Check if CRN exists in COURSE
    string checkSQL = "SELECT 1 FROM COURSE WHERE CRN = ?";
    sqlite3_stmt* checkStmt;
    sqlite3_prepare_v2(db, checkSQL.c_str(), -1, &checkStmt, nullptr);
    sqlite3_bind_text(checkStmt, 1, crn.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(checkStmt) != SQLITE_ROW) {
        cout << "CRN not found.\n";
        sqlite3_finalize(checkStmt);
        return;
    }
    sqlite3_finalize(checkStmt);

	// Checks if it conflicts with existing schedule
    if (conflictExists(db, email, crn)) {
        cout << "The added course conflicts with an existing course.\n";
        return;
    }
    // Insert into schedule
    string sql = "INSERT INTO SCHEDULE (STUDENTEMAIL, CRN) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, crn.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(stmt) == SQLITE_DONE)
        cout << "Course sucessfully added to schedule.\n";
    else
        cout << "Error course not added." << sqlite3_errmsg(db) << "\n";
    sqlite3_finalize(stmt);
}


void removeCourseFromSchedule(sqlite3* db, const string& email) {
    string crn;
    cout << "Enter CRN of the course you want to remove: ";
    cin >> crn;

    string sql = "DELETE FROM SCHEDULE WHERE STUDENTEMAIL = ? AND CRN = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, crn.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        if (sqlite3_changes(db) > 0)
            cout << "Course sucessfully removed.\n";
        else
            cout << "Course not found.\n";
    }
    else {
        cout << "Error, failed to remove course: " << sqlite3_errmsg(db) << "\n";
    }

    sqlite3_finalize(stmt);
}


void printSchedule(sqlite3* db, const string& email) {
    string sql =
        "SELECT COURSE.CRN, TITLE, DEPARTMENT, TIME, DAYS, SEMESTER, YEAR, CREDITS "
        "FROM COURSE INNER JOIN SCHEDULE ON COURSE.CRN = SCHEDULE.CRN "
        "WHERE STUDENTEMAIL = ?;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);

    cout << "\n--- Current Schedule for " << email << " ---\n";

    bool found = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        found = true;
        cout << "-------------------\n";
        cout << "CRN: " << sqlite3_column_text(stmt, 0) << "\n";
        cout << "Title: " << sqlite3_column_text(stmt, 1) << "\n";
        cout << "Department: " << sqlite3_column_text(stmt, 2) << "\n";
        cout << "Time: " << sqlite3_column_text(stmt, 3) << "\n";
        cout << "Days: " << sqlite3_column_text(stmt, 4) << "\n";
        cout << "Semester: " << sqlite3_column_text(stmt, 5) << "\n";
        cout << "Year: " << sqlite3_column_text(stmt, 6) << "\n";
        cout << "Credits: " << sqlite3_column_int(stmt, 7) << "\n";
    }

    if (!found) {
        cout << "Your schedule is empty.\n";
    }

    sqlite3_finalize(stmt);
}

bool conflictExists(sqlite3* db, const string& email, const string& newCRN) {
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