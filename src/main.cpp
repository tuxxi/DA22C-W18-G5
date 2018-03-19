//
//  main.cpp
//  22C_Team_Project_Interface
//
//  Created by Ashley Cline on 3/14/18.

#include <fstream>
#include <iostream>
#include <string>
#include "OlympianDatabase.h"

using namespace std;

ifstream getCmndLineInfile(int, const char *[]);
int getChoice();
void displayMenu();
void implementDecision(int choice, OlympianDatabase &olympianDatabase);
void searchSubMenu();
void listSubMenu();
void printVector(Vector<Olympian*> &vector);
ofstream getOutfile(string infileName);


int main(int argc, const char * argv[])
{
    int choice;
    ifstream infile;
    if (argc > 1)
    {
        infile = getCmndLineInfile(argc, argv);
    }
    else //default file jic
    {
        infile = ifstream("winter-olympians.csv");
    }

    if (!infile) return -1;

    OlympianDatabase olympianDatabase(infile);

    while ((choice = getChoice()) != 8)
    {
        implementDecision(choice, olympianDatabase);
        displayMenu();
    }

    ofstream outfile;
    olympianDatabase.saveDatabase(outfile);

    return 0;
}

ifstream getCmndLineInfile(int argc, const char *argv[])
{

    if (argc <= 1) //argc includes the name of the program in the count, similar to how argv[0] is the name of the program
    {
        cerr << "Cannot build database. An input file was not specified. Exiting." << endl;
        exit(-1);
    }

    const string infileName = argv[1];

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

    displayMenu();

    char input = cin.get();

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

    switch (choice)
    {
        case 1:
        {
        Olympian *temp = new Olympian;
        olympianDatabase.insert(temp);

        break;

        }

        case 2:
        {
            //Olympian dataOut;
            cout << "Enter name of Olympian for deletion: ";
            cin >> key;
            olympianDatabase.remove(key);

            break;
        }

        case 3:
        {   
            olympianDatabase.undoDelete();
            break;
        }

        case 4:
        {
            searchSubMenu();

            cin >> searchOp;

            switch (searchOp)
            {
            case 1:
            {
                cout << "Enter name of Olympian: ";
                cin >> key;

                Olympian *searchObj = olympianDatabase.searchByName(key);

                cout << *searchObj;

                break;
            }
                

            case 2:
            {
                cout << "Enter the age: ";
                cin >> age;

                Vector<Olympian*> searchResults;
                if (olympianDatabase.searchByAge(age, searchResults))
                {
                    for (auto oly : searchResults)
                    {
                        cout << *oly << endl;
                    }
                }
                else
                {
                    cout << "No entries found!\n";
                }
                break;

            }
                
            case 3:
            {
                cout << "Enter the height: ";
                cin >> height;

                Vector<Olympian*> searchResults;

                if (olympianDatabase.searchByHeight(height, searchResults))
                {
                    for (auto oly : searchResults)
                    {
                        cout << *oly << endl;
                    }
                }
                else
                {
                    cout << "No entries found!\n";
                }
                break;
            }
                
            default:
                std::cout << "\nYou did not enter a valid option. Please enter a number between 1 and 3.\n\n";
            }

            break;

        } 
        case 5:
        {
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
                cout << "\nDeveloped by Aidan Sojourner, Ashley Cline, Alexander Langley, and Jeff Yang\n\n";
                break;

            default:
                cout << "\nYou did not enter a valid option. please enter an option between 1 and 5.\n\n";
            }

            break;
        }
  
        case 6:
        {
            //olympianDatabase.saveDatabase();
            break;
        }


        case 7:
        {
            olympianDatabase.displayHashStats();
            break;
        }

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
