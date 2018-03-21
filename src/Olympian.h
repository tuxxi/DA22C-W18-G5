#ifndef OLYMPIAN_H
#define OLYMPIAN_H

#include <iostream>
#include <string>

class Olympian
{
public:
    Olympian() = default;
    Olympian(std::string n, std::string sp, std::string st, char g, int a, int h, int ng, int ns, int nb)
    {
        name = n;
        sport = sp;
        state = st;
        gender = g;
        age = a;
        height = h;
        nGold = ng;
        nSilver = ns;
        nBronze = nb;
    }

    void setName(std::string n) { name = n; }
    void setSport(std::string s) { sport = s; }
    void setState(std::string s) { state = s; }
    void setGender(char g) { gender = g; }
    void setHeight(int h) { height = h; }
    void setAge(int a) { age = a; }
    void setnGold(int g) { nGold = g; }
    void setnSilver(int s) { nSilver = s; }
    void setnBronze(int b) { nBronze = b; }

    std::string getName() const { return name; }
    std::string getSport() const { return sport; }
    std::string getState() const { return state; }
    char getGender() const { return gender; }
    int getAge() const { return age; }
    int getHeight() const { return height; }
    int getnGold() const { return nGold; }
    int getnSilver() const { return nSilver; }
    int getnBronze() const { return nBronze; }

    friend std::ostream &operator<<(std::ostream&, const Olympian&);
    friend std::ostream &operator<<(std::ostream&, Olympian *const &);

private:
    std::string name;
    std::string state;
    std::string sport;
    char gender;
    int height;
    int age;
    int nGold;
    int nSilver;
    int nBronze;
};

inline std::ostream &operator<<(std::ostream &os, const Olympian &olympian)
{
    os << "Name: " << olympian.name << std::endl;
    os << "Age: " << olympian.age << std::endl;
    os << "Gender: " << olympian.gender << std::endl;
    os << "Height: " << olympian.height << std::endl;
    os << "State: " << olympian.state << std::endl;
    os << "Sport: " << olympian.sport << std::endl;
    os << "Number Gold Medals: " << olympian.nGold << std::endl;
    os << "Number Silver Medals: " << olympian.nSilver << std::endl;
    os << "Number Bronze Medals: " << olympian.nBronze << std::endl;

    return os;
}

inline std::ostream &operator<<(std::ostream &os, Olympian *const &olympian)
{
    os << "Name: " << olympian->name << std::endl;
    os << "Age: " << olympian->age << std::endl;
    os << "Gender: " << olympian->gender << std::endl;
    os << "Height: " << olympian->height << std::endl;
    os << "State: " << olympian->state << std::endl;
    os << "Sport: " << olympian->sport << std::endl;
    os << "Number Gold Medals: " << olympian->nGold << std::endl;
    os << "Number Silver Medals: " << olympian->nSilver << std::endl;
    os << "Number Bronze Medals: " << olympian->nBronze << std::endl;

    return os;
}

#endif