// Lab1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
class user {
public:
    int ID;
    std::string first_name;
    std::string last_name;
    void name_print() {
        std::cout << first_name, last_name;
        std::cout << "Name_print function called sucessfully.\n";

    }
};

class student : public user {
public:
    void course_search() {
        std::cout << "course_search function called sucessfully.\n";

    }
    void add() {
        std::cout << "add function called sucessfully.\n";

    }
    void drop() {
        std::cout << "drop function called sucessfully.\n";

    }
    void print_schedule() {
        std::cout << "print_schedule function called sucessfully.\n";

    }
};

class instructor : public user {
public:
    void course_search() {
        std::cout << "course_search function called sucessfully.\n";

    }
    void print_classlist() {
        std::cout << "print_classlist function called sucessfully.\n";

    }
    void print_schedule() {
        std::cout << "print_schedule function called sucessfully.\n";

    }
};

class admin : public user {
public:
    void add_course() {
        std::cout << "add_course function called sucessfully.\n";

    }
    void remove_course() {
        std::cout << "remove_course function called sucessfully.\n";

    }
    void add_remove_users() {
        std::cout << "add_remove_users function called sucessfully.\n";

    }
    void add_remove_student() {
        std::cout << "add_remove_student function called sucessfully.\n";

    }
    void course_search() {
        std::cout << "course_search function called sucessfully.\n";

    }
    void roster_search() {
        std::cout << "roster_search function called sucessfully.\n";

    }
    void course_print() {
        std::cout << "course_print function called sucessfully.\n";

    }
    void roster_print() {
        std::cout << "roster_print function called sucessfully.\n";

    }
};
int main()
{
    std::cout << "Hello World!\n";
    user Testuser;
        student Teststudent;
        instructor Testinstructor;
        admin Testadmin;
        Testuser.name_print();
        Teststudent.add();
        Teststudent.drop();
        Teststudent.course_search();
        Teststudent.print_schedule();
        Testinstructor.course_search();
        Testinstructor.print_classlist();
        Testinstructor.print_schedule();
        Testadmin.course_search();
        Testadmin.add_course();
        Testadmin.add_remove_student();
        Testadmin.add_remove_users();
        Testadmin.course_print();
        Testadmin.course_search();
        Testadmin.remove_course();
        Testadmin.roster_print();
        Testadmin.roster_search();


}
