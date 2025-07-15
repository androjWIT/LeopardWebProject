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
    void menu();
    void searchCourse();
    void SC_user(); // searches coures based on user specifications params
    void addCourse();   // add courses to the system
    void removeCourse();
    void addStudent(); // add students to the system
    void addInstructor(); // adds instructor to the system
    void linkInstructor();  //links the instructor to a course
    void unlinkInstructor();    // unliks the instrucotr to a course
    void addStudent_course();   // Adds student to a course
    void removeStudent_course();    // removes student to a course.


    // for testing the functions
    bool addCourse_test(const std::string& crn,const std::string& title, const std::string& dep,
        const std::string& time, const std::string& days, const std::string& semester, const std::string& year,
        const std::string& instructor);
    bool searchCourseByCRN_test(const std::string& crn, std::string& outTitle);
    bool searchCourseByTitle_test(const std::string& title, std::string& outCRN);
    bool searchCourseByDep_test(const std::string& dep, std::string& outTitle);
    bool removeCourseByCRN_test(const std::string& crn);

private:
    sqlite3* db;
};


#endif //MAIN_ADMIN_H
