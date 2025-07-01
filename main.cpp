#include <iostream>
#include "ADMIN.h"
#include "StudentCode.h"
#include <string>
#include <sqlite3.h>
#include <stdio.h>

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

// this is used for the main file where we will call all of our classes and objects
int main() {


    sqlite3* db = openDatabase("assignment3.db");
    if (!db) {
        std::cerr << "Failed to open database.\n";
        return 1;
    }

    std::string email;
    std::cout << "Enter email: ";
    std::cin >> email;
	// does email exist in STUDENTLOGINS table?
    std::string checkStudentSql = "SELECT * FROM STUDENTLOGINS WHERE USERSEMAIL = ?";
    sqlite3_stmt* checkStmt;
    sqlite3_prepare_v2(db, checkStudentSql.c_str(), -1, &checkStmt, nullptr);
    sqlite3_bind_text(checkStmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(checkStmt) != SQLITE_ROW) {
        // Create ADMIN object and run menu
        ADMIN admin(db);
        admin.menu();
    }
    else {
		// Create STUDENT object and run menu
        Studentmain(db, email);
    }
    sqlite3_finalize(checkStmt);
  
   

    // Close the database connection
    sqlite3_close(db);
    std::cout << "Database closed.\n";

    return 0;
}