//
//  Olympian.cpp
//  group
//
//  Created by YF on 19/03/2018.
//  Copyright © 2018 YF. All rights reserved.
//

#include "Olympian.h"

using namespace std;

Olympian::Olympian()
{
    name = "";
    state = "";
    sport = "";
    gender = '\0';
    height = -1;
    age = -1;
    nGold = -1;
    nSilver = -1;
    nBronze = -1;
}

Olympian::Olympian(string n, string sp, string st, char g, int a, int h, int ng, int ns, int nb)
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

ostream &operator<<(std::ostream &os, const Olympian olympian)
{
    os << "Name: " << olympian.name << endl;
    os << "Age: " << olympian.age << endl;
    os << "Height: " << olympian.height << endl;
    os << "Gender: " << olympian.gender << endl;
    os << "State: " << olympian.state << endl;
    os << "Sport: " << olympian.sport << endl;
    os << "Number Gold Medals: " << olympian.nGold << endl;
    os << "Number Silver Medals: " << olympian.nSilver << endl;
    os << "Number Bronze Medals: " << olympian.nBronze << endl;
    
    return os;
}

ostream &operator<<(std::ostream &os, Olympian *const &olympian)
{
    os << "Name: " << olympian->name << endl;
    os << "Age: " << olympian->age << endl;
    os << "Height: " << olympian->height << endl;
    os << "Gender: " << olympian->gender << endl;
    os << "State: " << olympian->state << endl;
    os << "Sport: " << olympian->sport << endl;
    os << "Number Gold Medals: " << olympian->nGold << std::endl;
    os << "Number Silver Medals: " << olympian->nSilver << endl;
    os << "Number Bronze Medals: " << olympian->nBronze << endl;

    return os;
}

