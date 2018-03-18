//
//  main.cpp
//  22C_Team_Project_Interface
//
//  Created by Ashley Cline on 3/14/18.

#include <fstream>
#include <iostream>
#include <string>
#include "OlympianDatabase.h"

ifstream getCmndLineInfile(int, const char *[]);
int getChoice();
void displayMenu();
void implementDecision(int choice, OlympianDatabase &olympianDatabase);
void searchSubMenu();
void listSubMenu();
void printVector(Vector<Olympian*> &vector);
ofstream getOutfile(string infileName);

using namespace std;

int main(int argc, const char * argv[])
{
    int choice;
    ifstream infile = getCmndLineInfile(argc, argv);
    ofstream outfile;

    if (!infile) return -1;

    OlympianDatabase olympianDatabase(infile);

    while ((choice = getChoice()) != 8)
    {
        implementDecision(choice, olympianDatabase);
        displayMenu();
    }

    olympianDatabase.saveDatabase(outfile);

    return 0;
}

ifstream getCmndLineInfile(int argc, const char *argv[])
{
    string infileName;

    if (!argc)
    {
        cerr << "Cannot build database. An input file was not specified. Exiting." << endl;
        exit(1);
    }

    infileName = argv[1];

    ifstream infile(infileName);

    if (!infile)
    {
        cerr << "The file " << infileName << " could not be opened. Exiting" << endl;
        exit(1);
    }

    return infile;
}

int getChoice()
{
    int choice = 0;
    char input;

    displayMenu();

    input = cin.get();

    while ((choice = atoi(&input)) < 1 || choice > 8)
    {
        cout << "You did not enter a valid option. Please enter a number between 1 and 8.\n";

        input = cin.get();
    }

    return choice;
}



void displayMenu()
{
    std::cout << "Choose an operation:\n";
    std::cout << "1. Add New Data\n";
    std::cout << "2. Delete Data\n";
    std::cout << "3. Undo Delete\n";
    std::cout << "4. Search\n";
    std::cout << "5. List\n";
    std::cout << "6. Write Data to File\n";
    std::cout << "7. Statistics\n";
    std::cout << "8. QUIT\n";
}

void implementDecision(int choice, OlympianDatabase &olympianDatabase)
{
    int searchOp, listOp, height, age;
    string key;
    Olympian *searchObj;
    Vector<Olympian *> searchResults;

    switch (choice)
    {
        case 1:
            Olympian *temp;
            temp = new Olympian;
            olympianDatabase.insert(temp);

            break;

        case 2:
            Olympian dataOut;
            cout << "Enter name of Olympian for deletion: ";
            cin >> key;
            olympianDatabase.remove(key);

            break;

        case 3:
            olympianDatabase.undoDelete();

            break;

        case 4:
            searchSubMenu();

            cin >> searchOp;

            switch (searchOp)
            {
                case 1:
                    cout << "Enter name of Olympian: ";
                    cin >> key;

                    searchObj = olympianDatabase.searchByName(key);

                    cout << *searchObj;

                    break;

                case 2:
                    cout << "Enter the age: ";
                    cin >> age;

                    searchResults = olympianDatabase.searchByAge(age);

                    //print out results

                    break;

                case 3:
                    cout << "Enter the height: ";
                    cin >> height;

                    searchResults = olympianDatabase.searchByHeight(height);
                    //print out results

                    break;

                default:
                    std::cout << "\nYou did not enter a valid option. Please enter a number between 1 and 3.\n\n";
            }

            break;

        case 5:
            listSubMenu();
            cin >> listOp;

            switch (listOp)
            {
                case 1:
                    // print all data from stack

                    break;

                case 2:
                    // print all names

                    break;

                case 3:
                    olympianDatabase.displayAgeInOrder();

                    break;

                case 4:
                    olympianDatabase.displayHeightInOrder();

                    break;

                case 5:
                    cout << "Indented Age Tree:\n";
                    olympianDatabase.displayAgeTree();
                    cout << "Indented Height Tree:\n";
                    olympianDatabase.displayHeightTree();

                    break;

                case 6: // hidden print option
                    // Put Aidan's last name here
                    cout << "\nDeveloped by Aidan, Ashley Cline, Alexander Langley, and Jeff Yang\n\n";

                    break;

                default:
                    cout << "\nYou did not enter a valid option. please enter an option between 1 and 5.\n\n";
            }

            break;

        case 6:
            //olympic.saveDatabase();

            break;

        case 7:
            olympianDatabase.displayHashStats();

            break;

        default:
            std::cout << "\nYou did not enter a valid option. Please enter a number between 1 and 8.\n\n";
    }
}

void searchSubMenu()
{
    std::cout << "Choose a search operation:\n";
    std::cout << "1 - Find and Display an Olympian by Name\n";
    std::cout << "2 - Find and Display Olympians by Age\n";
    std::cout << "3 - Find and Display Olympians by Height\n";
}

void listSubMenu()
{
    std::cout << "Choose a list operation:\n";
    std::cout << "1 - List Unsorted Olympians\n";
    std::cout << "2 - List Olympians Sorted by Name\n";
    std::cout << "3 - List Olympians Sorted by Age\n";
    std::cout << "4 - List Olympians Sorted by Height\n";
    std::cout << "5 - Print Data as Indented List\n";
}
