#include <iostream>
#include "ADMIN.h"
#include <string>
#include <sqlite3.h>
#include "INSTRUCTOR.h"
#include "STUDENT.h"
#include <stdio.h>
#include <iostream>
// main created by Luis Martinez
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

// gettting ID that user types in to know what type of they are and checks all database
int getUser(sqlite3* db, const std::string& id)
{
    sqlite3_stmt* stmt;

    // Check Admin
    const char* sqlAdmin = "SELECT ID FROM ADMIN WHERE ID = ?";
    if (sqlite3_prepare_v2(db, sqlAdmin, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            sqlite3_finalize(stmt);
            return 1; // Admin
        }
        sqlite3_finalize(stmt);
    }

    // Check Instructor
    const char* sqlInstructor = "SELECT ID FROM INSTRUCTOR WHERE ID = ?";
    if (sqlite3_prepare_v2(db, sqlInstructor, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            sqlite3_finalize(stmt);
            return 2; // Instructor
        }
        sqlite3_finalize(stmt);
    }

    // Check Student
    const char* sqlStudent = "SELECT ID FROM STUDENT WHERE ID = ?";
    if (sqlite3_prepare_v2(db, sqlStudent, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            sqlite3_finalize(stmt);
            return 3; // Student
        }
        sqlite3_finalize(stmt);
    }

    return 0; // Not found

}


// this is used for the main file where we will call all of our classes and objects
int main() {
    sqlite3* db;
    int rc = sqlite3_open("C:/Users/luis_/College/programing/applied_programming_concepts/projects/Leopard_web/code/Assignment_5/assignment3.db", &db);


    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    } else {
        std::cout << "Opened database successfully.\n";
    }

    std::string email, password, id;
    sqlite3_stmt* stmt;

    std::cout << "Welcome. Please login.\n";
    std::cout << "Email: ";
    std::cin >> email;
    std::cout << "Password: ";
    std::cin >> password;

// checks login database to check if password and email exist and is correct
    const char* sqlLogin = "SELECT ID FROM LOGIN WHERE EMAIL = ? AND PASSWORD = ?";
    if (sqlite3_prepare_v2(db, sqlLogin, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        } else {
            std::cout << "Login failed. Invalid email or password.\n";
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 0;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare login query.\n";
        sqlite3_close(db);
        return 1;
    }

    int userType = getUser(db, id);
    if (userType == 1)
    {
        std::cout << "Login successfully, Welcome admin.\n";
        ADMIN admin(db);
        admin.menu();

    }
    else if (userType == 2)
    {
        std::cout << "Login successfully, Welcome Instructor.\n";
        std::string studentID;
        std::string sql = "SELECT ID FROM INSTRUCTOR WHERE EMAIL = ?;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            studentID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        } else {
            std::cerr << "Could not finD instructor ID form email.\n";
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 1;
        }
        sqlite3_finalize(stmt);
        INSTRUCTOR instructor(db, email);
        instructor.menu();
    }
    else if (userType == 3)
    {
        std::cout << "Login successfully, Welcome Student.\n";

        std::string studentID;
        std::string sql = "SELECT ID FROM STUDENT WHERE EMAIL = ?;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            studentID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        } else {
            std::cerr << "Could not find student ID for email.\n";
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 1;
        }
        sqlite3_finalize(stmt);
        STUDENT student(db, studentID);
        student.studentMenu();
    }

    sqlite3_close(db);
    std::cout << "Database closed.\n";
    return 0;
}