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



int main(int argc, char** argv){

    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("assignment3.db", &DB);

    if (exit) {
        std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
        return (-1);
    }
    else
        std::cout << "Opened Database Successfully!" << std::endl;



    menu(DB);
    sqlite3_close(DB);


    return 0;
}



int menu(sqlite3* db){
    cout << "Menu of actions to do: \n";
    cout << "1. add a student: \n";
    cout << "2. remove an Instructor: \n";
    cout << "3.Update Admin: \n";
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

        default:
            cout << "please enter valid answer: ";
    }
}



void addStudent(sqlite3 *db){
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
    } else {
        cout << "Student added successfully.\n";
    }

    sqlite3_finalize(stmt);

}

void removeInstructor(sqlite3* db){
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
    } else {
        // Optional check: was anything actually deleted?
        if (sqlite3_changes(db) == 0) {
            cout << "No instructor with that ID was found.\n";
        } else {
            cout << "Instructor successfully deleted.\n";
        }
    }


    sqlite3_finalize(stmt);
}

