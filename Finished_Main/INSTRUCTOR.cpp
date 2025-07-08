#include "INSTRUCTOR.h"
#include <iostream>
#include <sqlite3.h>
#include <string>

using namespace std;

// Constructor
INSTRUCTOR::INSTRUCTOR(sqlite3* dbConn) {
    db = dbConn;
    cout << "Enter your instructor ID: ";
    cin >> instructorID;
}

// Menu loop
void INSTRUCTOR::menu() {
    int input;
    do {
        cout << "\n--- Instructor Menu ---\n";
        cout << "1. Search All Courses\n";
        cout << "2. Search Courses with Parameters\n";
        cout << "3. Print Teaching Schedule\n";
        cout << "4. Search Course Roster (by CRN)\n";
        cout << "5. Print All Course Rosters\n";
        cout << "0. Logout\n> ";
        cin >> input;

        switch (input) {
        case 1: searchCourse(); break;
        case 2: searchCourseParam(); break;
        case 3: printSchedule(); break;
        case 4: searchRoster(); break;
        case 5: printRoster(); break;
        case 0: cout << "Logging out...\n"; break;
        default: cout << "Invalid choice.\n"; break;
        }
    } while (input != 0);
}

// Search all courses taught by instructor
void INSTRUCTOR::searchCourse() {
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

// Search courses by parameter
void INSTRUCTOR::searchCourseParam() {
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

// Print teaching schedule (detailed version)
void INSTRUCTOR::printSchedule() {
    std::string instructor_email;
    std::cout << "Which Instructors schedule would you like to lookup: \n";
    std::cout << "Enter course instructor email here: \n";
    std::cin >> instructor_email;

    std::string sql = "SELECT * FROM COURSE WHERE INSTRUCTOR = ?";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement.\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, instructor_email.c_str(), -1, SQLITE_TRANSIENT);

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



// Search a specific course's roster
void INSTRUCTOR::searchRoster() {
    string crn;
    cout << "Enter CRN to view roster: ";
    cin >> crn;

    string sql = "SELECT STUDENT.ID, STUDENT.NAME, STUDENT.SURNAME "
        "FROM ENROLLMENT "
        "JOIN STUDENT ON ENROLLMENT.StudentID = STUDENT.ID "
        "WHERE ENROLLMENT.CRN = ?";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, crn.c_str(), -1, SQLITE_TRANSIENT);

    cout << "\n--- Roster for CRN: " << crn << " ---\n";
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << "ID: " << sqlite3_column_text(stmt, 0)
            << " | Name: " << sqlite3_column_text(stmt, 1)
            << " " << sqlite3_column_text(stmt, 2) << "\n";
    }

    sqlite3_finalize(stmt);
}

// Print all rosters for all courses instructor teaches
void INSTRUCTOR::printRoster() {
    string sql = "SELECT CRN FROM COURSE WHERE InstructorID = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, instructorID.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string crn = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        cout << "\n--- Roster for CRN: " << crn << " ---\n";

        // Reuse searchRoster logic inline
        string innerSQL = "SELECT STUDENT.ID, STUDENT.NAME, STUDENT.SURNAME "
            "FROM ENROLLMENT "
            "JOIN STUDENT ON ENROLLMENT.StudentID = STUDENT.ID "
            "WHERE ENROLLMENT.CRN = ?";
        sqlite3_stmt* innerStmt;
        sqlite3_prepare_v2(db, innerSQL.c_str(), -1, &innerStmt, nullptr);
        sqlite3_bind_text(innerStmt, 1, crn.c_str(), -1, SQLITE_TRANSIENT);

        while (sqlite3_step(innerStmt) == SQLITE_ROW) {
            cout << "ID: " << sqlite3_column_text(innerStmt, 0)
                << " | Name: " << sqlite3_column_text(innerStmt, 1)
                << " " << sqlite3_column_text(innerStmt, 2) << "\n";
        }

        sqlite3_finalize(innerStmt);
    }

    sqlite3_finalize(stmt);
}
