#ifndef OLYMPIAN
#define OLYMPIAN

#include <string>
#include <cstring>

using namespace std;

class Olympian {
private:
    string name;
    char state[2];
    string event;
    char gender;
    int height;
    int age;

public:
    Olympian() { name = "", event = "", gender = '\0', height = 0; age = 0; }
    Olympian(string n, string e, char g, int h, int a) { name = n; event = e; gender = g; height = h; age = a; }

    void setName(string n) { name = n; }
    void setState(char *s) { strcpy(state, s); }
    void setEvent(string e) { event = e; }
    void setGender(char g) { gender = g; }
    void setHeight(int h) { height = h; }
    void setAge(int a) { age = a; }

    string getName() { return name; }
    string getEvent() { return event; }
    char setGender() { return gender; }
    int getHeight() { return height; }
    int getAge() { return age; }
};

#endif