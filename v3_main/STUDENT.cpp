

#include "STUDENT.h"
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>
#include "Student.h"

using namespace std;

// Callback function for displaying results
static int callback(void*, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\n";
    }
    cout << endl;
    return 0;
}



void STUDENT::studentMenu() {
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
        cin.ignore();

        switch (choice) {
        case 1:
            searchCourses(std::cin);
            break;
        case 2:
            searchCoursesByParam(std::cin);
            break;
        case 3:
            addCourseToSchedule(std::cin);
            break;
        case 4:
            removeCourseFromSchedule(std::cin);
            break;
        case 5:
            printSchedule();
            break;
        case 0:
            cout << "Logging out...\n";
            break;
        default:
            cout << "Please enter a valid option.\n";
        }
    } while (choice != 0);
}

bool STUDENT::searchCourses(std::istream& Crn) {
        std::string course_crn;
        std::cout << "What Courses would you like to search: \n";
        std::cout << "Enter course CRN here: \n";
        std::getline(Crn, course_crn);

        std::string sql = "SELECT * FROM COURSE WHERE CRN = ?";
        sqlite3_stmt* stmt;

        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement.\n";
            return false;
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

bool STUDENT::searchCoursesByParam(std::istream& input) {
      if (!db)
    {
        std::cerr << "Failed to open database.\n";
        return false;
    }

    int choice;
    std::cout << "Enter the parameter you would like to search: \n";
    std::cout << "1. CRN:  \n";
    std::cout << "2. Title:  \n";
    std::cout << "3. Department:  \n";
    input  >> choice;
    input.ignore();

    std::string paramvalue;
    std::string sql; //store the sql query string

    sqlite3_stmt* stmt;

    switch (choice){
    case 1:
        std::cout << "Enter CRN: \n";
        //std::cin.ignore(); // this handles the previous new line and clears leftover newline characters in the input buffer from previous
        std::getline(input, paramvalue);
        sql = "SELECT * FROM COURSE WHERE CRN = ?";
        break;

    case 2:
        std::cout << "Enter Title: \n";
        std::getline(input, paramvalue);
        sql = "SELECT * FROM COURSE WHERE TITLE = ?";
        break;

    case 3:
        std::cout << "Enter department: \n";
        std::getline(input, paramvalue);
        sql = "SELECT * FROM COURSE WHERE DEPARTMENT = ?";
        break;

    default:
        std::cout << "invalid choice. \n";
        return false;

    }

    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement.\n";
        return false;
    }

    sqlite3_bind_text(stmt, 1, paramvalue.c_str(), -1, SQLITE_TRANSIENT);

    bool found = false;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        found = true;
        std::cout << "\n";
        std::cout << "CRN: " << sqlite3_column_text(stmt, 0) << "\n";
        std::cout << "Title: " << sqlite3_column_text(stmt, 1) << "\n";
        std::cout << "Department: " << sqlite3_column_text(stmt, 2) << "\n";
        std::cout << "Time: " << sqlite3_column_text(stmt, 3) << "\n";
        std::cout << "Days: " << sqlite3_column_text(stmt, 4) << "\n";
        std::cout << "Semester: " << sqlite3_column_text(stmt, 5) << "\n";
        std::cout << "Year: " << sqlite3_column_text(stmt, 6) << "\n";
        std::cout << "Instructor: " << sqlite3_column_text(stmt, 7) << "\n";
        std::cout << "\n";
    }
    sqlite3_finalize(stmt);

    if (!found) {
        std::cout << "No matching courses found.\n";
        return false;
    }
    return true;

}

bool STUDENT::addCourseToSchedule( std::istream& Crn) {
    string crn;
    cout << "Enter CRN to add: ";
    std::getline(Crn, crn);

    // Check if CRN exists in COURSE
    string checkSQL = "SELECT 1 FROM COURSE WHERE CRN = ?";
    sqlite3_stmt* checkStmt;
    sqlite3_prepare_v2(db, checkSQL.c_str(), -1, &checkStmt, nullptr);
    sqlite3_bind_text(checkStmt, 1, crn.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(checkStmt) != SQLITE_ROW) {
        cout << "CRN not found.\n";
        sqlite3_finalize(checkStmt);
        return false;
    }
    sqlite3_finalize(checkStmt);

	// Checks if it conflicts with existing schedule
    if (conflictExists(Crn)) {
        cout << "The added course conflicts with an existing course.\n";
        return false;
    }
    // Insert into schedule
    string sql = "INSERT INTO ENROLLMENT (StudentID, CRN) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, studentId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, crn.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_finalize(stmt);

    if (sqlite3_step(stmt) == SQLITE_DONE)
    {
        cout << "Course sucessfully added to schedule.\n";
        return true;
    }else
    {
        cout << "Error course not added." << sqlite3_errmsg(db) << "\n";
        return false;
    }
}


bool STUDENT::removeCourseFromSchedule(std::istream& Crn) {
    string crn;
    cout << "Enter CRN of the course you want to remove: ";
    cin >> crn;

    string sql = "DELETE FROM SCHEDULE WHERE StudentID = ? AND CRN = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, studentId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, crn.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_finalize(stmt);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        if (sqlite3_changes(db) > 0)
        {
            cout << "Course sucessfully removed.\n";
            return true;
        }
        else
        {
            cout << "Course not found.\n";
            return false;
        }
    }
    else {
        cout << "Error, failed to remove course: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
}


bool STUDENT::printSchedule() {
    if (!db) {
        std::cerr << "Database not available.\n";
        return false;
    }


    if (studentId.empty()) {
        std::cerr << "Student ID not set!\n";
        return false;
    }

    std::cout << "Debug: Looking for Student ID: '" << studentId << "'\n";

    std::string checkSql = "SELECT CRN FROM ENROLLMENT WHERE StudentID = ?";
    sqlite3_stmt* checkStmt;

    int rc = sqlite3_prepare_v2(db, checkSql.c_str(), -1, &checkStmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare enrollment check: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(checkStmt, 1, studentId.c_str(), -1, SQLITE_TRANSIENT);

    std::cout << "Debug: Checking enrollments...\n";
    bool hasEnrollments = false;
    while (sqlite3_step(checkStmt) == SQLITE_ROW) {
        hasEnrollments = true;
        std::cout << "Debug: Found enrollment for CRN: " << sqlite3_column_text(checkStmt, 0) << "\n";
    }
    sqlite3_finalize(checkStmt);

    if (!hasEnrollments) {
        std::cout << "Debug: No enrollments found for Student ID: " << studentId << "\n";
        return false;
    }


    std::string sql =
        "SELECT COURSE.CRN, TITLE, DEPARTMENT, TIME, DAYS_OF_WEEK, SEMESTER, YEAR, INSTRUCTOR "
        "FROM COURSE INNER JOIN ENROLLMENT ON COURSE.CRN = ENROLLMENT.CRN "
        "WHERE ENROLLMENT.StudentID = ?";

    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare schedule query: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, studentId.c_str(), -1, SQLITE_TRANSIENT);

    std::cout << "\n--- Current Schedule for " << studentId << " ---\n";

    bool found = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        found = true;
        std::cout << "-------------------\n";
        std::cout << "CRN: " << sqlite3_column_text(stmt, 0) << "\n";
        std::cout << "Title: " << sqlite3_column_text(stmt, 1) << "\n";
        std::cout << "Department: " << sqlite3_column_text(stmt, 2) << "\n";
        std::cout << "Time: " << sqlite3_column_text(stmt, 3) << "\n";
        std::cout << "Days: " << sqlite3_column_text(stmt, 4) << "\n";
        std::cout << "Semester: " << sqlite3_column_text(stmt, 5) << "\n";
        std::cout << "Year: " << sqlite3_column_text(stmt, 6) << "\n";
        std::cout << "Credits: " << sqlite3_column_int(stmt, 7) << "\n";
    }

    sqlite3_finalize(stmt);

    if (!found) {
        std::cout << "Your schedule is empty.\n";
        return false;
    }

    return true;
}

bool STUDENT::conflictExists(std::istream& Crn)
{
    if (!db) {
        std::cerr << "Database not available.\n";
        return false;
    }


    std::string crn;
    std::cout << "Enter course CRN: ";
    std::getline(Crn, crn);

    std::string sql =
        "SELECT 1 FROM COURSE C1, COURSE C2, ENROLLMENT S "
        "WHERE C1.CRN = ? AND C2.CRN = S.CRN AND S.StudentID = ? "
        "AND C1.DAYS = C2.DAYS AND C1.TIME = C2.TIME";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare conflict check statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }


    sqlite3_bind_text(stmt, 1, crn.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, studentId.c_str(), -1, SQLITE_TRANSIENT);

    bool conflict = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        conflict = true;
        std::cout << "Schedule conflict detected!\n";
        sqlite3_finalize(stmt);
        return conflict;
    }
}