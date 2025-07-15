#include <gtest/gtest.h>
#include "/c/Users/luis_/College/programing/applied_programming_concepts/projects/Leopard_web/code/Assignment_5/ADIMN.h"
#include <sqlite3.h>

// Utility function: in-memory test DB
sqlite3* createTestDB() {
    sqlite3* db;
    sqlite3_open(":memory:", &db);

    const char* setupSQL =
        "CREATE TABLE COURSE ("
        "CRN TEXT PRIMARY KEY, "
        "TITLE TEXT, "
        "DEPARTMENT TEXT, "
        "TIME TEXT, "
        "DAYS_OF_WEEK TEXT, "
        "SEMESTER TEXT, "
        "YEAR TEXT, "
        "INSTRUCTOR TEXT);";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, setupSQL, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "DB setup error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    return db;
}

TEST(AdminTest, AddCourseAndSearchByCRN) {
    sqlite3* db = createTestDB();
    ADMIN admin(db);

    // Add a test course
    ASSERT_TRUE(admin.addCourseDirect("C101", "Linear Algebra", "MATH", "09AM", "MWF", "Fall", "2025", "Prof. Euler"));

    // Search it
    std::string foundTitle;
    ASSERT_TRUE(admin.searchCourseByCRN("C101", foundTitle));
    EXPECT_EQ(foundTitle, "Linear Algebra");

    sqlite3_close(db);
}

TEST(AdminTest, SearchCourseByTitle) {
    sqlite3* db = createTestDB();
    ADMIN admin(db);

    admin.addCourseDirect("C202", "Physics", "PHYS", "1PM", "TR", "Spring", "2026", "Dr. Newton");

    std::string foundCRN;
    ASSERT_TRUE(admin.searchCourseByTitle("Physics", foundCRN));
    EXPECT_EQ(foundCRN, "C202");

    sqlite3_close(db);
}

TEST(AdminTest, RemoveCourseByCRN) {
    sqlite3* db = createTestDB();
    ADMIN admin(db);

    admin.addCourseDirect("C303", "Signals", "ECE", "11AM", "MW", "Fall", "2025", "Prof. Nyquist");

    std::string title;
    ASSERT_TRUE(admin.searchCourseByCRN("C303", title));

    ASSERT_TRUE(admin.removeCourseByCRN("C303"));
    ASSERT_FALSE(admin.searchCourseByCRN("C303", title));  // Should be gone

    sqlite3_close(db);
}