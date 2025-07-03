
#include "INSTRUCTOR.h"
#include <iostream>




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


// Code By Joseph Andro

// Instructor Menu:

void INSTRUCTOR::menu() {
    int input;
    std::cout << "Welcome Instructor " << instructorID << ", choose an option:\n";
    std::cout << "1. Search Courses\n";
    std::cout << "2. Search Courses with Parameters\n";
    std::cout << "3. Print Teaching Schedule\n";
    std::cout << "4. Search Course Roster\n";
    std::cout << "5. Print Course Roster\n";
    std::cin >> input;

    switch (input) {
    case 1: searchCourse(); break;
    case 2: searchCourseParam(); break;
    case 3: printSchedule(); break;
    case 4: searchRoster(); break;
    case 5: printRoster(); break;
    default: std::cout << "Invalid choice.\n"; break;
    }
}

// Funcion to search for courses in DB

void INSTRUCTOR::searchCourse() {
    std::string sql = "SELECT * FROM COURSE WHERE InstructorID = ?";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, instructorID.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::cout << "CRN: " << sqlite3_column_text(stmt, 0) << "\n"
            << "Title: " << sqlite3_column_text(stmt, 1) << "\n"
            << "Department: " << sqlite3_column_text(stmt, 2) << "\n\n";
    }

    sqlite3_finalize(stmt);
}

// Fucntion to search for courses with parameters

void INSTRUCTOR::searchCourseParam() {
    int choice;
    std::string value;
    std::string sql;
    sqlite3_stmt* stmt;

    std::cout << "Search by:\n1. CRN\n2. Title\n3. Department\n";
    std::cin >> choice;
    std::cin.ignore();
    std::cout << "Enter value: ";
    std::getline(std::cin, value);

    switch (choice) {
    case 1: sql = "SELECT * FROM COURSE WHERE CRN = ? AND InstructorID = ?"; break;
    case 2: sql = "SELECT * FROM COURSE WHERE TITLE = ? AND InstructorID = ?"; break;
    case 3: sql = "SELECT * FROM COURSE WHERE DEPARTMENT = ? AND InstructorID = ?"; break;
    default: std::cout << "Invalid.\n"; return;
    }

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, value.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, instructorID.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::cout << "CRN: " << sqlite3_column_text(stmt, 0) << "\n"
            << "Title: " << sqlite3_column_text(stmt, 1) << "\n\n";
    }

    sqlite3_finalize(stmt);
}

// Print Schedule Function

void INSTRUCTOR::printSchedule() {
    std::string sql = "SELECT CRN, TITLE, TIME, DAYS_OF_WEEK FROM COURSE WHERE InstructorID = ?";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, instructorID.c_str(), -1, SQLITE_TRANSIENT);

    std::cout << "--- Teaching Schedule ---\n";
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::cout << "CRN: " << sqlite3_column_text(stmt, 0)
            << " | Title: " << sqlite3_column_text(stmt, 1)
            << " | Time: " << sqlite3_column_text(stmt, 2)
            << " | Days: " << sqlite3_column_text(stmt, 3) << "\n";
    }

    sqlite3_finalize(stmt);
}

//Search The Coarse Roster

void INSTRUCTOR::searchRoster() {
    std::string crn;
    std::cout << "Enter CRN to view roster: ";
    std::cin >> crn;

    std::string sql = "SELECT STUDENT.ID, STUDENT.NAME, STUDENT.SURNAME FROM ENROLLMENT "
        "JOIN STUDENT ON ENROLLMENT.StudentID = STUDENT.ID "
        "WHERE ENROLLMENT.CRN = ?";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, crn.c_str(), -1, SQLITE_TRANSIENT);

    std::cout << "--- Roster ---\n";
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::cout << "ID: " << sqlite3_column_text(stmt, 0)
            << " | Name: " << sqlite3_column_text(stmt, 1)
            << " " << sqlite3_column_text(stmt, 2) << "\n";
    }

    sqlite3_finalize(stmt);
}

//Print The Coarse Roaster

void INSTRUCTOR::printRoster() {
    std::string sql = "SELECT CRN FROM COURSE WHERE InstructorID = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, instructorID.c_str(), -1, SQLITE_TRANSIENT);

    std::cout << "--- All Rosters ---\n";
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string crn = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::cout << "\nCRN: " << crn << "\n";
        INSTRUCTOR::searchRoster(); // reuse function
    }

    sqlite3_finalize(stmt);
}