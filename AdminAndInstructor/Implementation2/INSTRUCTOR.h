#ifndef INSTRUCTOR_H
#define INSTRUCTOR_H

#include <sqlite3.h>
#include <string>

class INSTRUCTOR {
private:
    sqlite3* db;
    std::string instructorID;

public:
    INSTRUCTOR(sqlite3* db, const std::string& id);
    void menu();
    void searchCourse();
    void searchCourseParam();
    void printSchedule();
    void searchRoster();
    void printRoster();
};

#endif
