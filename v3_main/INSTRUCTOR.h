
#ifndef INSTRUCTOR_H
#define INSTRUCTOR_H
#include <sqlite3.h>
#include <string>

// for most of the student actions we should know how
class INSTRUCTOR {
    private:
        sqlite3* db;
        std::string instructorID;

    public:

    INSTRUCTOR(sqlite3* db, const std::string& id) : db(db), instructorID(id) {}
    void menu();
    bool searchCourse();
    bool searchCourseParam(std::istream& input);
    bool printSchedule();  //This line is required
    bool searchRoster(std::istream& Crn);
    bool printRoster(std::istream& Crn);  // Updated earlier

        //test functions


    /*
    void setInstructorID(std::istream& id);
    bool CourseWithTitle(std::istream& title);
    bool teachScheduleCount(std::istream& count);
    */

};



#endif //INSTRUCTOR_H
