//
// Created by luis martinez on 6/17/25.
//

#ifndef MAIN_ADMIN_H
#define MAIN_ADMIN_H
#include <sqlite3.h>
#include <string>

class ADMIN {

public:
    ADMIN(sqlite3* database) { db = database; } // constructor
    void menu();
    void searchCourse();
    void SC_user(); // searches coures based on user specifications params
    void addCourse();   // add courses to the system
    void addStudent(); // add students to the system
    void addInstructor(); // adds instructor to the system
    void linkInstructor();  //links the instructor to a course
    void unlinkInstructor();    // unliks the instrucotr to a course
    void addStudent_course();   // Adds student to a course
    void removeStudent_course();    // removes student to a course.

private:
    sqlite3* db;
};


#endif //MAIN_ADMIN_H
