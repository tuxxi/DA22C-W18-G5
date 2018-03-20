//
//  Olympian.h
//  group
//
//  Created by YF on 19/03/2018.
//  Copyright © 2018 YF. All rights reserved.
//

#ifndef Olympian_h
#define Olympian_h

#include <iostream>
#include <string>

class Olympian
{
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
    
public:
    // Constructors
    Olympian();
    Olympian(std::string n, std::string sp, std::string st, char g, int a, int h, int ng, int ns, int nb);
    
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
    char setGender() const { return gender; }
    int getAge() const { return age; }
    int getHeight() const { return height; }
    int getnGold() const { return nGold; }
    int getnSilver() const { return nSilver; }
    int getnBronze() const { return nBronze; }
    
    friend std::ostream &operator<<(std::ostream&, const Olympian);
    friend std::ostream &operator<<(std::ostream&, Olympian *const &);
};


#endif

