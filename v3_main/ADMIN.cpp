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




// menu function that calls the different options to do as the admin
void ADMIN::menu(){
    int input;
    do{
        std::cout << "Welcome Admin, please choose what you would like to do: \n";
        std::cout << "1. Search Course \n";
        std::cout << "2. Search course on parameters \n";
        std::cout << "3. Add course \n";
        std::cout << "4. Delete course\n";
        std::cout << "5. Add Student \n";
        std::cout << "6. Add Instructor \n";
        std::cout << "7. Link Instructor \n";
        std::cout << "8. Unlink Instructor \n";
        std::cout << "9. Add student to a course \n";
        std::cout << "10. Remove student from course \n";
        std::cout << "0. Exit\n";
        std::cout << "Please enter the number on which action you take: \n";
        std::cin >> input;
        std::cin.ignore();
        switch (input) {
        case 0:
            break;

        case 1:
            searchCourse(std::cin);
            break;
        case 2:
            SC_user(std::cin);
            break;
        case 3:
            addCourse(std::cin,
                std::cin,
                std::cin,
                std::cin,
                std::cin,
                std::cin,
                std::cin,
                std::cin);
            break;

        case 4:
            removeCourse(std::cin);
            break;
        case 5:
            addStudent(std::cin,
                std::cin,
                std::cin,
                std::cin,
                std::cin,
                std::cin,
                std::cin);
            break;
        case 6:
            addInstructor(std::cin,
                std::cin,
                std::cin,
                std::cin,
                std::cin,
                std::cin,
                std::cin,
                std::cin);
            break;
        case 7:
            linkInstructor(std::cin, std::cin);
            break;
        case 8:
            unlinkInstructor(std::cin);
            break;
        case 9:
            addStudent_course(std::cin,std::cin);
            break;
        case 10:
            removeStudent_course(std::cin, std::cin);
            break;
        default:
            std::cout << "please enter a valid answer";

        }
     }while (input != 0);
    std::cout << "Goodbye Admin\n";
}



// search course
bool ADMIN::searchCourse(std::istream& Crn){
    std::string course_crn;
    std::cout << "What Courses would you like to search: \n";
    std::cout << "Enter course CRN here: \n";
    std::getline(Crn,course_crn );

    std::string sql = "SELECT * FROM COURSE WHERE CRN = ?";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false ;
    } else {
        std::cout << "Course Found.\n";
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
            return true;
        } else {
            std::cout << "Course not found.\n";
            return false;
        }
    }
    sqlite3_finalize(stmt);
}





// search course by parameter
bool ADMIN::SC_user(std::istream& input){
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

bool ADMIN::addCourse(std::istream& Crn,
                      std::istream& Title,
                      std::istream& Dep,
                      std::istream& Time,
                      std::istream& Days,
                      std::istream& Semester,
                      std::istream& Year,
                      std::istream& Instructor)
{
    std::string crn, title, dep, time, days, semester, year, instructor;

    std::cout << "Please enter the Course you want to add: \n";
    std::cout << "Enter CRN number: \n";
    std::getline(Crn,crn );
    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clears the buffer until it sees the \n
    std::cout <<  "Enter the title:  \n";
    std::getline(Title, title);
    std::cout <<  "Enter the Department:  \n";
    std::getline(Dep, dep);
    std::cout << "Enter the Time slot of the course: \n";
    std::getline(Time, time);
    std::cout <<  "Enter the days of the week the course will take place in: \n";
    std::getline(Days, days);
    std::cout <<  "Enter the semester the course will take place in: \n";
    std::getline(Semester, semester);
    std::cout << "Enter the year: \n";
    std::getline(Year, year);
    std::cout <<  "Enter the Instructor: \n";
    std::getline(Instructor, instructor);
    // add
    std::string sql = "INSERT INTO COURSE( CRN, TITLE, DEPARTMENT, TIME, DAYS_OF_WEEK, SEMESTER, YEAR, INSTRUCTOR) VALUES (?,?,?,?,?,?,?,?)";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    //bind pramaters
    sqlite3_bind_text(stmt, 1, crn.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, dep.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, time.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, days.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, semester.c_str(), -1, SQLITE_TRANSIENT);
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




bool ADMIN::removeCourse(std::istream& Crn)
{
    std::string crn;
    std::cout << "Enter the CRN of the course you want to remove: ";
    std::getline(Crn, crn);

    // Validate course exists
    std::string checkSql = "SELECT * FROM COURSE WHERE CRN = ?";
    sqlite3_stmt* checkStmt;

    int rc = sqlite3_prepare_v2(db, checkSql.c_str(), -1, &checkStmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare check statement.\n";
        return false;
    }

    sqlite3_bind_text(checkStmt, 1, crn.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(checkStmt) != SQLITE_ROW) {
        std::cout << "Course record not found.\n";
        sqlite3_finalize(checkStmt);
        return false;
    }
    sqlite3_finalize(checkStmt);

    // Delete course
    std::string sql = "DELETE FROM COURSE WHERE CRN = ?";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare delete statement.\n";
        return false;
    }

    sqlite3_bind_text(stmt, 1, crn.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Delete failed: " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    std::cout << "Successfully removed course.\n";
    return true;
}



bool ADMIN::addStudent(std::istream& Id,
    std::istream& FNAME,
    std::istream& LNAME,
    std::istream& GradYear,
    std::istream& Major,
    std::istream& Email,
    std::istream& Pass)
{
    std::string ID, FName, Lname, gradYear, major, email, pass;
    std::cout << "Hello please enter Student information below: \n";
    std::cout << "Enter ID: \n";
    std::getline(Id, ID);
    std::cout << "Enter first name: \n";
    std::getline(FNAME, FName);
    std::cout << "Enter last name: \n";
    std::getline(LNAME, Lname);
    std::cout << "Enter expected graduation year: \n";
    std::getline(GradYear, gradYear);
    std::cout << "Enter major: \n";
    std::getline(Major, major);
    std::cout << "Enter email: \n";
    std::getline(Email, email);
    std::cout << "Create a password for Login: \n";
    std::getline(Pass, pass);

    std::string sql = "INSERT INTO STUDENT( ID, NAME, SURNAME, GRADYEAR, MAJOR, EMAIL) VALUES (?,?,?,?,?,?)";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare insert statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    //bind pramaters
    sqlite3_bind_text(stmt, 1, ID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, FName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, Lname.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, gradYear.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, major.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, email.c_str(), -1, SQLITE_TRANSIENT);


    // error checking for the database
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

// inserts into a login database when new student is created
    std::string sql2 = "INSERT INTO LOGIN( EMAIL, ID, PASSWORD) VALUES (?,?,?)";
    sqlite3_stmt* stmt2;

    rc = sqlite3_prepare_v2(db, sql2.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare insert statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, ID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, pass.c_str(), -1, SQLITE_TRANSIENT);

    // error checking for the database
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    std::cout << "Student added successfully.\n";
    return true;
}

bool ADMIN::addInstructor(std::istream& Id,
    std::istream& FNAME,
    std::istream& LNAME,
    std::istream& Title,
    std::istream& Dept,
    std::istream& Email,
    std::istream& Pass,
    std::istream& HireYear)
{
    std::string ID, FName, Lname, title, dept, email, pass, hireYear;
    std::cout << "Hello please enter Instructor information below: \n";
    std::cout << "Enter ID: \n";
    std::getline(Id, ID);
    std::cout << "Enter first name: \n";
    std::getline(FNAME, FName);
    std::cout << "Enter last name: \n";
    std::getline(LNAME, Lname);
    std::cout << "Enter title of Instructor: \n";
    std::getline(Title, title);
    std::cout << "Enter the department of the instructor: \n";
    std::getline(Dept, dept);
    std::cout << "Enter email: \n";
    std::getline(Email, email);
    std::cout << "Create password for Login: \n";
    std::getline(Pass, pass);
    std::cout << "Enter the hire year: \n";
    std::getline(HireYear, hireYear);


    std::string sql = "INSERT INTO INSTRUCTOR( ID, NAME, SURNAME, TITLE, HIREYEAR,DEPT, EMAIL) VALUES (?,?,?,?,?,?,?)";


    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare insert statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }


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
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

// adds the new instructor into the login database
    std::string sql2 = "INSERT INTO LOGIN(EMAIL, ID, PASSWORD) VALUES (?,?,?)";
    sqlite3_stmt* stmt2;
    rc = sqlite3_prepare_v2(db, sql2.c_str(), -1, &stmt2, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare insert statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }


    sqlite3_bind_text(stmt2, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt2, 2, ID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt2, 3, pass.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt2);
    sqlite3_finalize(stmt2);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }


    std::cout << "Instructor added successfully.\n";
    return true;

}

// uses course Database
bool ADMIN::linkInstructor(std::istream& Crn, std::istream& InsEmail){
    std::string crn, instructor_Email;

    std::cout << "Enter the CRN of the course you want to link an instructor to: \n";
    std::getline(Crn, crn);

    std::cout << "Enter the ID of the instructor to link: \n";
    std::getline(InsEmail, instructor_Email);

    std::string sql = "UPDATE COURSE SET INSTRUCTOR = ? WHERE CRN = ?";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    sqlite3_bind_text(stmt, 1, instructor_Email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, crn.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Update failed: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    std::cout << "Instructor linked to course successfully.\n";
    return true;

}


//uses course database
bool ADMIN::unlinkInstructor(std::istream& Crn){
    std::string crn;
    std::cout << "Enter the CRN of the course to unlink the instructor: \n";
    std::getline(Crn, crn);

    std::string sql = "UPDATE COURSE SET INSTRUCTOR = NULL WHERE CRN = ?";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement.\n";
        return false;
    }

    sqlite3_bind_text(stmt, 1, crn.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE)
    {
        std::cerr << "Unlink failed: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    std::cout << "Instructor unlinked successfully.\n";
    return true;
}

// use course database
bool ADMIN::addStudent_course(std::istream& StudId, std::istream& Crn){
    std::string studentID, crn;

    std::cout << "Enter Student ID to add: ";
    std::getline(StudId, studentID);
    std::cout << "Enter CRN of course: ";
    std::getline(Crn, crn);

    // Validate Student exists
    std::string checkStudentSql = "SELECT * FROM STUDENT WHERE ID = ?";
    sqlite3_stmt* checkStmt;

    int rc = sqlite3_prepare_v2(db, checkStudentSql.c_str(), -1, &checkStmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare student check statement.\n";
        return false;
    }

    sqlite3_bind_text(checkStmt, 1, studentID.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(checkStmt) != SQLITE_ROW) {
        std::cout << "Student ID not found.\n";
        sqlite3_finalize(checkStmt);
        return false;
    }
    sqlite3_finalize(checkStmt);

    // checks if Course exists
    std::string checkCourseSql = "SELECT * FROM COURSE WHERE CRN = ?";
    rc = sqlite3_prepare_v2(db, checkCourseSql.c_str(), -1, &checkStmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare course check statement.\n";
        return false;
    }

    sqlite3_bind_text(checkStmt, 1, crn.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(checkStmt) != SQLITE_ROW) {
        std::cout << "Course CRN not found.\n";
        sqlite3_finalize(checkStmt);
        return false;
    }
    sqlite3_finalize(checkStmt);

    // Insert into ENROLLMENT
    std::string sql = "INSERT INTO ENROLLMENT (StudentID, CRN) VALUES (?,?)";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare insert statement.\n";
        return false;
    }

    sqlite3_bind_text(stmt, 1, studentID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, crn.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    std::cout << "Student successfully added to course.\n";
    return true;
}

// use database course
bool ADMIN::removeStudent_course(std::istream& StudId, std::istream& Crn){
    std::string studentID, crn;
    std::cout << "Enter Student ID to remove: ";
    std::getline(StudId, studentID);
    std::cout << "Enter CRN of course: ";
    std::getline(Crn, crn);


    // Validate enrollment exists
    std::string checkSql = "SELECT * FROM ENROLLMENT WHERE StudentID = ? AND CRN = ?";
    sqlite3_stmt* checkStmt;
    sqlite3_prepare_v2(db, checkSql.c_str(), -1, &checkStmt, nullptr);
    sqlite3_bind_text(checkStmt, 1, studentID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(checkStmt, 2, crn.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(checkStmt) != SQLITE_ROW) {
        std::cout << "Enrollment record not found.\n";
        sqlite3_finalize(checkStmt);
        return 0;
    }
    sqlite3_finalize(checkStmt);

    // Delete enrollment
    std::string sql = "DELETE FROM ENROLLMENT WHERE StudentID = ? AND CRN = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare delete statement.\n";
        return 0;
    }

    sqlite3_bind_text(stmt, 1, studentID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, crn.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Delete failed: " << sqlite3_errmsg(db) << "\n";

    }
        std::cout << "Student successfully removed from course.\n";
        return true;

    sqlite3_finalize(stmt);
    
}


/*
 *******************************************************************************************************************
 *******************************************************************************************************************
 *******************************************************************************************************************
 *******************************************************************************************************************
 *******************************************************************************************************************
 *******************************************************************************************************************
*/


