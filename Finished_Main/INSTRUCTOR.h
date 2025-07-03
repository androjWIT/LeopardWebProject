
#ifndef INSTRUCTOR_H
#define INSTRUCTOR_H
#include <sqlite3.h>
#include <string>


class INSTRUCTOR {
    private:
        sqlite3* db;
        std::string instructorID;

    public:
        INSTRUCTOR(sqlite3* database) { db = database;}
        void menu();
        void searchCourse();
        void searchCourseParam();
        void printSchedule();
        void searchRoster();
        void printRoster();

};



#endif //INSTRUCTOR_H
