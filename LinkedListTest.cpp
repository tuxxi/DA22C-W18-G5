#include <iostream>
#include <fstream>
#include "Olympian.h"
#include "LinkedList.h"

using namespace std;

char gender;
int nGold, nSilver, nBronze, age, height;
string input, firstName, lastName, sport, state;
Olympian *newRecord = nullptr, *olympian;

int cmp(Olympian *const &o1, Olympian *const &o2)
{
    if (o1->getName() < o2->getName()) return 1;
    else if (o1->getName() > o2->getName()) return -1;
    return 0;
}

int main ()
{
    char gender;
    int nGold, nSilver, nBronze, age, height;
    string input, firstName, lastName, sport, state;
    Olympian *newRecord = nullptr, key;
    ifstream infile("winter-olympians.csv");
    LinkedList<Olympian*> linkedList(cmp);

    while(getline(infile, firstName, ',')) {
        if (firstName.length()) {
            getline(infile, lastName, ',');
            getline(infile, input, ',');
            gender = input[0];
            getline(infile, sport, ',');
            getline(infile, input, ',');

            if (input.find('"') != string::npos) {
                getline(infile, input, '"');
                getline(infile, input, ',');
                getline(infile, input, ',');
            } else
                (getline(infile, input, ','));

            if (input.find('"') != string::npos) {
                getline(infile, input, '"');
                getline(infile, input, ',');
            }

            getline(infile, input, ',');
            if (isdigit(input[0]))
                nGold = stoi(input);
            else
                nGold = 0;
            getline(infile, input, ',');
            if (isdigit(input[0]))
                nSilver = stoi(input);
            else
                nSilver = 0;
            getline(infile, input, ',');
            if (isdigit(input[0]))
                nBronze = stoi(input);
            else
                nBronze = 0;

            getline(infile, input, '"');
            getline(infile, input, '\'');
            height = 12 * stoi(input);
            getline(infile, input, '"');
            height += stoi(input);

            for (int i = 0; i < 5; i++)
                getline(infile, input, ',');
            age = stoi(input);

            for (int i = 0; i < 2; i++)
                getline(infile, input, ',');
            state = input;

            getline(infile, input);

            newRecord = new Olympian(firstName + " " + lastName, sport, state, gender,
                                     age, height, nGold, nSilver, nBronze);

            linkedList.insert(newRecord);
        }
    }

    olympian = new Olympian;
    olympian->setName("AJ Muss");

    linkedList.search(olympian);

    cout << olympian << endl;

    return 0;
}