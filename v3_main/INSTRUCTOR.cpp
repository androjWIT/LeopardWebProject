
#include "INSTRUCTOR.h"
#include <iostream>
#include <sqlite3.h>


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
    do
    {

        std::cout << "Welcome Instructor " << instructorID << ", choose an option:\n";
        std::cout << "1. Search Courses\n";
        std::cout << "2. Search Courses with Parameters\n";
        std::cout << "3. Print Teaching Schedule\n";
        std::cout << "4. Search Course Roster\n";
        std::cout << "5. Print Course Roster\n";
        std::cout << "0. Exit\n";
        std::cin >> input;
        std::cin.ignore();

        switch (input) {
        case 1: searchCourse(); break;
        case 2: searchCourseParam(std::cin); break;
        case 3: printSchedule(); break;
        case 4: searchRoster(std::cin); break;
        case 5: printRoster(std::cin); break;
        default: std::cout << "Invalid choice.\n"; break;
        }
    }
    while (input != 0);
    std::cout << "good bye Instructor/n";
}

// Funcion to search for courses in DB

bool INSTRUCTOR::searchCourse() {

    if (!db) {
        std::cerr << "Database not available.\n";
        return false;
    }

    std::string sql = "SELECT * FROM COURSE WHERE INSTRUCTOR = ?";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, instructorID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_finalize(stmt);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::cout << "CRN: " << sqlite3_column_text(stmt, 0) << "\n"
            << "Title: " << sqlite3_column_text(stmt, 1) << "\n"
            << "Department: " << sqlite3_column_text(stmt, 2) << "\n\n";
    }
    return true;
}

// Fucntion to search for courses with parameters

bool INSTRUCTOR::searchCourseParam(std::istream& input) {
    if (!db)
    {
        std::cerr << "Failed to open database.\n";
        return false;
    }

    int choice;
    std::string value;
    std::string sql;
    sqlite3_stmt* stmt;

    std::cout << "Search by:\n1. CRN\n2. Title\n3. Department\n";
    std::cin >> choice;
    std::cin.ignore();
    std::cout << "Enter value: ";
    input >> choice;

    switch (choice) {
    case 1: sql = "SELECT * FROM COURSE WHERE CRN = ? AND INSTRUCTOR = ?"; break;
    case 2: sql = "SELECT * FROM COURSE WHERE TITLE = ? AND INSTRUCTOR = ?"; break;
    case 3: sql = "SELECT * FROM COURSE WHERE DEPARTMENT = ? AND INSTRUCTOR = ?"; break;
    default: std::cout << "Invalid.\n"; return false;
    }

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, value.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, instructorID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_finalize(stmt);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::cout << "CRN: " << sqlite3_column_text(stmt, 0) << "\n"
            << "Title: " << sqlite3_column_text(stmt, 1) << "\n\n";
    }
    return true;
}

//print schedule
bool INSTRUCTOR::printSchedule() {
    if (!db) {
        std::cerr << "Database not available.\n";
        return false;
    }

    std::string sql = "SELECT CRN, TITLE, DEPARTMENT, TIME, DAYS_OF_WEEK, SEMESTER, YEAR FROM COURSE WHERE INSTRUCTOR = ?";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare schedule query: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, instructorID.c_str(), -1, SQLITE_TRANSIENT);

    std::cout << "\n--- Your Teaching Schedule ---\n";
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
        std::cout << "Year: " << sqlite3_column_text(stmt, 6) << "\n\n";
    }

    sqlite3_finalize(stmt);

    if (!found) {
        std::cout << "You are not assigned to any courses.\n";
        return false;
    }

    return true;
}






// Print Schedule Function
bool INSTRUCTOR::printRoster(std::istream& Crn) {

    if (!db)
    {
        std::cerr << "Database not available.\n";
        return false;
    }

    std::string crn;
    std::cout << "Enter CRN to view roster for that course: \n";
    std::getline(Crn, crn);

    std::string sql =
        "SELECT STUDENT.StudentID, STUDENT.FirstName, STUDENT.LastName "
        "FROM STUDENT "
        "JOIN ENROLLMENT ON STUDENT.ID = ENROLLMENT.StudentID "
        "WHERE ENROLLMENT.CRN = ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {

        std::cerr << "Error: Failed to prepare roster query: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, crn.c_str(), -1, SQLITE_TRANSIENT);

    std::cout << "Class Roster for CRN: "<< crn << "\n";
    bool hasRows = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        hasRows = true;
        std::string id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string fname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string lname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::cout << "Student ID: " << id << ", Name: " << fname << " " << lname << "\n";
    }

    if (rc != SQLITE_DONE && !hasRows) {
        std::cerr << "Error during query execution: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);

    if (!hasRows) {
        std::cout << "No students enrolled in this course.\n";
    }
    return hasRows;
}

//Search The Coarse Roster

bool INSTRUCTOR::searchRoster(std::istream& Crn) {
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
        return true;
    }

    sqlite3_finalize(stmt);
}





//Print The Course Roaster
/*******
 *
 *
 *
 * I dont think i need this here jsut in case
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
*************/



