
#ifndef STUDENT_H
#define STUDENT_H
#include <sqlite3.h>
#include <string>

// User login
class  STUDENT{
public:

    // stores the id so that there inst any other accees for other users cannot use
    STUDENT(sqlite3* db, const std::string& id) : db(db), studentId(id) {}
    void studentMenu();

    bool searchCourses(std::istream& Crn);

    bool searchCoursesByParam(std::istream& input);

    bool addCourseToSchedule(std::istream& Crn);

    bool removeCourseFromSchedule(std::istream& Crn);

    bool printSchedule();

    bool conflictExists(std::istream& Crn);

private:
   sqlite3* db;
    std::string studentId;



};




#endif //STUDENT_H
