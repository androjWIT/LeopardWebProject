#include <iostream>
#include "ADMIN.h"
#include <string>
#include <sqlite3.h>
#include <stdio.h>
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

// this is used for the main file where we will call all of our classes and objects
int main() {

    sqlite3* db;

    // Open the database
    int rc = sqlite3_open("assignment3.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    } else {
        std::cout << "Opened database successfully.\n";
    }

    // Create ADMIN object and run menu
    ADMIN admin(db);
    admin.menu();

    // Close the database connection
    sqlite3_close(db);
    std::cout << "Database closed.\n";

    return 0;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.