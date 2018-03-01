#ifndef OLYMPIAN_H
#define OLYMPIAN_H

#include <string>
#include <cstring>

class Olympian 
{
public:
    Olympian() { name = "", event = "", gender = '\0', height = 0; age = 0; }
    Olympian(std::string n, std::string e, char g, int h, int a) { name = n; event = e; gender = g; height = h; age = a; }

    void setName(std::string n) { name = n; }
    void setState(char *s) { strcpy(state, s); } //be careful here!
    void setEvent(std::string e) { event = e; }
    void setGender(char g) { gender = g; }
    void setHeight(int h) { height = h; }
    void setAge(int a) { age = a; }

    std::string getName() const { return name; }
    std::string getEvent() const { return event; }
    char setGender() const { return gender; }
    int getHeight() const { return height; }
    int getAge() const { return age; }

private:
    std::string name;
    char state[3];
    std::string event;
    char gender;
    int height;
    int age;

};

#endif //