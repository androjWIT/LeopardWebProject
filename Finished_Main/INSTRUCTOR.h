#ifndef INSTRUCTOR_H
#define INSTRUCTOR_H

#include <string>
#include <sqlite3.h>

class INSTRUCTOR {
private:
    sqlite3* db;
    std::string instructorID;

public:
    INSTRUCTOR(sqlite3* dbConn);
    void menu();
    void searchCourse();
    void searchCourseParam();
    void printSchedule();
    void searchRoster();
    void printRoster();
};


#endif //INSTRUCTOR_H
