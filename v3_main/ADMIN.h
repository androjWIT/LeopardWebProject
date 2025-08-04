//
// Created by luis martinez on 6/17/25.
//
//
#ifndef MAIN_ADMIN_H
#define MAIN_ADMIN_H
#include <sqlite3.h>
#include <string>

class ADMIN {

public:
    ADMIN(sqlite3* database) { db = database; } // constructor
    void menu();    // no need to test the menu since it only chooses

    bool searchCourse(std::istream& Crn);

    bool SC_user(std::istream& input); // searches coures based on user specifications params

    bool addCourse(std::istream& Crn,
        std::istream& Title,
        std::istream& Dep,
        std::istream& Time,
        std::istream& Days,
        std::istream& Semester,
        std::istream& Year,
        std::istream& Instructor);   // add courses to the system

    bool removeCourse(std::istream& Crn);

    bool addStudent(std::istream& Id,
    std::istream& FNAME,
    std::istream& LNAME,
    std::istream& GradYear,
    std::istream& Major,
    std::istream& Email, // add students to the system
    std::istream& Pass);

    bool addInstructor(std::istream& Id,
    std::istream& FNAME,
    std::istream& LNAME,
    std::istream& Title,
    std::istream& Dept,
    std::istream& Email,
    std::istream& Pass,
    std::istream& HireYear); // adds instructor to the system

    bool linkInstructor(std::istream& Crn, std::istream& InsId);  //links the instructor to a course

    bool unlinkInstructor(std::istream& Crn);    // unliks the instrucotr to a course

    bool addStudent_course(std::istream& StudId, std::istream& Crn);   // Adds student to a course

    bool removeStudent_course(std::istream& StudId, std::istream& Crn);    // removes student to a course.

    // login testing
    bool login_test(const std::string& username);  // Returns true if admin is able to login
    bool logout_test();  // Returns true if logout clears the session





private:
    sqlite3* db;
};


#endif
