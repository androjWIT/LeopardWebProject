
#ifndef MAIN_StudentCode_H
#define MAIN_StudentCode_H
#include <sqlite3.h>
#include <string>


// Database functions
sqlite3* openDatabase(const std::string& dbName);

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

// Admin/Instructor functions
void addStudent(sqlite3* db);
void removeInstructor(sqlite3* db);
void updateAdmin(sqlite3* db);
void displayTable(sqlite3* db, const std::string& tableName);
void addCourse(sqlite3* db);

// C-style callback for sqlite3_exec
int callback(void*, int, char**, char**);

#endif // STUDENTCODE_H

