#include <iostream>
#include <fstream>
#include "Olympian.h"
#include "Stack.h"

using namespace std;

char gender;
int nGold, nSilver, nBronze, age, height;
string input, firstName, lastName, sport, state;
Olympian *newRecord = nullptr, *olympian;

int main ()
{
    char gender;
    int nGold, nSilver, nBronze, age, height;
    string input, firstName, lastName, sport, state;
    Olympian *newRecord = nullptr, key;
    ifstream infile("winter-olympians.csv");
    Stack<Olympian*> stack;

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

            stack.push(newRecord);
        }
    }

    while (!stack.isEmpty())
    {
        stack.pop(olympian);
        cout << olympian << endl;
    }

    cout << stack.isEmpty() << endl;

    return 0;
}