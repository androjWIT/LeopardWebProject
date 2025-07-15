
#ifndef STUDENT_H
#define STUDENT_H
#include <sqlite3.h>
#include <string>

// User login
std::pair<int, std::string> login(sqlite3* db);
void addStudentLogin(sqlite3* db);
int Studentmain(sqlite3* db, const std::string& email);

//void Studentmain(sqlite3* db, const std::string& email);
void studentMenu(sqlite3* db, const std::string& email);
void searchCourses(sqlite3* db);
void searchCoursesByParam(sqlite3* db);
void addCourseToSchedule(sqlite3* db, const std::string& email);
void removeCourseFromSchedule(sqlite3* db, const std::string& email);
void printSchedule(sqlite3* db, const std::string& email);
bool hasConflict(sqlite3* db, const std::string& email, const std::string& crn);



#endif //STUDENT_H
