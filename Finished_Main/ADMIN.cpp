//
// Created by luis martinez on 6/17/25.
//

#include "ADMIN.h"
#include <string>
#include <sqlite3.h>
#include <stdio.h>
#include <iostream>
#include <limits>

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




// menu function that calls the different
void ADMIN::menu(){
    int input;
    do{
        std::cout << "Welcome Admin, please choose what you would like to do: \n";
        std::cout << "1. Search Course \n";
        std::cout << "2. Search course on parameters \n";
        std::cout << "3. Add course \n";
        std::cout << "4. Add Student \n";
        std::cout << "5. Add Instructor \n";
        std::cout << "6. Link Instructor \n";
        std::cout << "7. Unlink Instructor \n";
        std::cout << "8. Add student to a course \n";
        std::cout << "9. Remove student from course \n";
        std::cout << "0. Exit\n";
        std::cout << "Please enter the number on which action you take: \n";
        std::cin >> input;

        switch (input) {
        case 0:
            break;

        case 1:
            searchCourse();
            break;
        case 2:
            SC_user();
            break;
        case 3:
            addCourse();
            break;
        case 4:
            addStudent();
            break;
        case 5:
            addInstructor();
            break;
        case 6:
            linkInstructor();
            break;
        case 7:
            unlinkInstructor();
            break;
        case 8:
            addStudent_course();
            break;
        case 9:
            removeStudent_course();
            break;
        default:
            std::cout << "please enter a valid answer";

        }
     }while (input != 0);
    std::cout << "Goodbye Admin\n";
}





// use Course database
void ADMIN::SC_user(){
    int choice;
    std::cout << "Enter the parameter you would like to search: \n";
    std::cout << "1. CRN:  \n";
    std::cout << "2. Title:  \n";
    std::cout << "3. Department:  \n";
    std::cin >> choice;

    std::string paramvalue;
    std::string sql; //store the sql query string
    sqlite3_stmt* stmt;

    switch (choice){
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
    if (!found) {
        std::cout << "No matching courses found.\n";
    }
    sqlite3_finalize(stmt);
}



// Use Course database
void ADMIN::searchCourse(){
    std::string course_crn;
    std::cout << "What Courses would you like to search: \n";
    std::cout << "Enter course CRN here: \n";
    std::cin >> course_crn;

    std::string sql = "SELECT * FROM COURSE WHERE CRN = ?";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }


    sqlite3_bind_text(stmt, 1, course_crn.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {
        // Display formatted output
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
    } else {
        std::cout << "Course not found.\n";
    }

    sqlite3_finalize(stmt);
}

// Use Course dataBase
void ADMIN::addCourse(){
    std::string crn, title, dep, time, daysofweek, sem, year, instructor;
    std::cout << "Please enter the Course you want to add: \n";
    std::cout << "Enter CRN number: \n";
    std::cin >> crn;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clears the buffer until it sees the \n
    std::cout <<  "Enter the title:  \n";
    std::getline(std::cin, title);
    std::cout <<  "Enter the Department:  \n";
    std::cin >> dep;
    std::cout << "Enter the Time slot of the course: \n";
    std::cin >> time;
    std::cout <<  "Enter the days of the week the course will take place in: \n";
    std::cin >> daysofweek;
    std::cout <<  "Enter the semester the course will take place in: \n";
    std::cin >> sem;
    std::cout << "Enter the year: \n";
    std::cin >>year;
    std::cout <<  "Enter the Instructor: \n";
    std::cin >> instructor;

    std::string sql = "INSERT INTO COURSE( CRN, TITLE, DEPARTMENT, TIME, DAYS_OF_WEEK, SEMESTER, YEAR, INSTRUCTOR) VALUES (?,?,?,?,?,?,?,?)";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    //bind pramaters
    sqlite3_bind_text(stmt, 1, crn.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, dep.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, time.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, daysofweek.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, sem.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, year.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 8, instructor.c_str(), -1, SQLITE_TRANSIENT);

    // error checking for the database
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Course added successfully.\n";
        // going to display the course the user just added
    }
    sqlite3_finalize(stmt);
}

void ADMIN::addStudent(){
    std::string ID, FName, Lname, gradYear, major, email;
    std::cout << "Hello please enter Student information below: \n";
    std::cout << "Enter ID: \n";
    std::cin >> ID;
    std::cout << "Enter first name: \n";
    std::cin >> FName;
    std::cout << "Enter last name: \n";
    std::cin >> Lname;
    std::cout << "Enter expected graduation year: \n";
    std::cin >> gradYear;
    std::cout << "Enter major: \n";
    std::cin >> major;
    std::cout << "Enter email: \n";
    std::cin >> email;

    std::string sql = "INSERT INTO STUDENT( ID, NAME, SURNAME, GRADYEAR, MAJOR, EMAIL) VALUES (?,?,?,?,?,?)";
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
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Student added successfully.\n";
    }

    sqlite3_finalize(stmt);
}

void ADMIN::addInstructor(){
    std::string ID, FName, Lname, title, dept, email, hireYear;
    std::cout << "Hello please enter Instructor information below: \n";
    std::cout << "Enter ID: \n";
    std::cin >> ID;
    std::cout << "Enter first name: \n";
    std::cin >> FName;
    std::cout << "Enter last name: \n";
    std::cin >> Lname;
    std::cin.ignore(5, '\n');
    std::cout << "Enter title of Instructor: \n";
    std::getline(std::cin, title);
    std::cout << "Enter the hire year: \n";
    std::cin >> hireYear;
    std::cout << "Enter the department of the instructor: \n";
    std::cin >> dept;
    std::cout << "Enter email: \n";
    std::cin >> email;


    std::string sql = "INSERT INTO INSTRUCTOR( ID, NAME, SURNAME, TITLE, HIREYEAR,DEPT, EMAIL) VALUES (?,?,?,?,?,?,?)";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    //bind pramaters
    sqlite3_bind_text(stmt, 1, ID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, FName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, Lname.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, hireYear.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, dept.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, email.c_str(), -1, SQLITE_TRANSIENT);


    // error checking for the database
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Student added successfully.\n";
    }

    sqlite3_finalize(stmt);
}

// uses course Database
void ADMIN::linkInstructor(){
    std::string crn, instructorID;

    std::cout << "Enter the CRN of the course you want to link an instructor to: \n";
    std::cin >> crn;

    std::cout << "Enter the ID of the instructor to link: \n";
    std::cin >> instructorID;

    std::string sql = "UPDATE COURSE SET INSTRUCTOR = ? WHERE CRN = ?";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << "\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, instructorID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, crn.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Update failed: " << sqlite3_errmsg(db) << "\n";
    } else {
        std::cout << "Instructor linked to course successfully.\n";
    }

    sqlite3_finalize(stmt);
}


//uses course database
void ADMIN::unlinkInstructor(){
    std::string crn;
    std::cout << "Enter the CRN of the course to unlink the instructor: \n";
    std::cin >> crn;

    std::string sql = "UPDATE COURSE SET INSTRUCTOR = NULL WHERE CRN = ?";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement.\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, crn.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Unlink failed: " << sqlite3_errmsg(db) << "\n";
    } else {
        std::cout << "Instructor unlinked successfully.\n";
    }

    sqlite3_finalize(stmt);
}

// use course database
void ADMIN::addStudent_course(){
    std::string studentID, crn;
    std::cout << "Enter Student ID to add: ";
    std::cin >> studentID;
    std::cout << "Enter CRN of course: ";
    std::cin >> crn;

    // Validate Student exists
    std::string checkStudentSql = "SELECT * FROM STUDENT WHERE ID = ?";
    sqlite3_stmt* checkStmt;
    sqlite3_prepare_v2(db, checkStudentSql.c_str(), -1, &checkStmt, nullptr);
    sqlite3_bind_text(checkStmt, 1, studentID.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(checkStmt) != SQLITE_ROW) {
        std::cout << "Student ID not found.\n";
        sqlite3_finalize(checkStmt);
        return;
    }
    sqlite3_finalize(checkStmt);

    // Validate Course exists
    std::string checkCourseSql = "SELECT * FROM COURSE WHERE CRN = ?";
    sqlite3_prepare_v2(db, checkCourseSql.c_str(), -1, &checkStmt, nullptr);
    sqlite3_bind_text(checkStmt, 1, crn.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(checkStmt) != SQLITE_ROW) {
        std::cout << "Course CRN not found.\n";
        sqlite3_finalize(checkStmt);
        return;
    }
    sqlite3_finalize(checkStmt);

    // Insert into ENROLLMENT
    std::string sql = "INSERT INTO ENROLLMENT (StudentID, CRN) VALUES (?,?)";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare insert statement.\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, studentID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, crn.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << "\n";
    } else {
        std::cout << "Student successfully added to course.\n";
    }

    sqlite3_finalize(stmt);
}

// use database course
void ADMIN::removeStudent_course(){
    std::string studentID, crn;
    std::cout << "Enter Student ID to remove: ";
    std::cin >> studentID;
    std::cout << "Enter CRN of course: ";
    std::cin >> crn;

    // Validate enrollment exists
    std::string checkSql = "SELECT * FROM ENROLLMENT WHERE StudentID = ? AND CRN = ?";
    sqlite3_stmt* checkStmt;
    sqlite3_prepare_v2(db, checkSql.c_str(), -1, &checkStmt, nullptr);
    sqlite3_bind_text(checkStmt, 1, studentID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(checkStmt, 2, crn.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(checkStmt) != SQLITE_ROW) {
        std::cout << "Enrollment record not found.\n";
        sqlite3_finalize(checkStmt);
        return;
    }
    sqlite3_finalize(checkStmt);

    // Delete enrollment
    std::string sql = "DELETE FROM ENROLLMENT WHERE StudentID = ? AND CRN = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare delete statement.\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, studentID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, crn.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Delete failed: " << sqlite3_errmsg(db) << "\n";
    } else {
        std::cout << "Student successfully removed from course.\n";
    }

    sqlite3_finalize(stmt);
    
}







