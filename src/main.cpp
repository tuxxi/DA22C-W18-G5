//
//  main.cpp
//  22C_Team_Project_Interface
//
//  Created by Ashley Cline on 3/14/18.

#include <fstream>
#include <iostream>
#include <string>
#include "OlympianDatabase.h"

#define DEFAULT_INFILE "winter-olympics.csv"
#define DEFAULT_OUTFILE "winter-olympics-out.csv"

using namespace std;

ifstream getCmndLineInfile(int, const char *[]);
int getChoice();
void displayMenu();
void doMainMenu(int choice, OlympianDatabase &olympianDatabase);

void searchSubMenu();
void doSearchSubmenu(int choice, OlympianDatabase &database);

void listSubMenu();
void doListSubmenu(int choice, OlympianDatabase &database);

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
        infile = ifstream(DEFAULT_INFILE);
    }

    if (!infile) return -1;

    OlympianDatabase olympianDatabase(infile);

    displayMenu();
    while ((choice = getChoice()) != 8)
    {
        doMainMenu(choice, olympianDatabase);
        displayMenu();
    }

    olympianDatabase.saveDatabase(DEFAULT_OUTFILE);

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
    while (true)
    {
        string buffer;
        getline(cin, buffer);
        try
        {
            //return a number IFF the user entered something that stoi can parse.
            return stoi(buffer);
        }
        catch(...)
        {
            //else, we keep looping until they do
            cout << "You did not enter a valid option. Please enter a number between 1 and 8.\n";
        }
    }
}


void doMainMenu(int choice, OlympianDatabase &olympianDatabase)
{
    switch (choice)
    {
        case 1:
        {
            Olympian *temp = new Olympian;

            //TODO: prompt for data
            olympianDatabase.insert(temp);
            break;
        }

        case 2:
        {
            cout << "Enter name of Olympian for deletion: ";

            string key;
            getline(cin, key);
            if (olympianDatabase.remove(key))
                cout << "Deleted " << key << " successfully!\n";
            else
                cout << "ERROR! Could not delete " << key << "!\n";
            break;
        }

        case 3:
        {
            if (olympianDatabase.undoDelete())
                cout << "Undo delete successful!\n";
            else
                cout << "ERROR! Undo delete not successful!\n";
            break;
        }

        case 4:
        {
            searchSubMenu();
            int searchOp = getChoice();
            doSearchSubmenu(searchOp, olympianDatabase);
            break;

        }
        case 5:
        {
            listSubMenu();
            int listOp = getChoice();
            doListSubmenu(listOp, olympianDatabase);
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
void printVector(Vector<Olympian *> &vector)
{
    for (auto oly : vector)
    {
        if (oly) cout << *oly << endl;
    }
}
void doSearchSubmenu(int choice, OlympianDatabase &database)
{
    string key;
    switch (choice)
    {
    case 1:
    {
        cout << "Enter name of Olympian: ";
        getline(cin, key);

        Olympian *searchObj = database.searchByName(key);
        if (searchObj)
            cout << *searchObj;
        else
            cout << "ERROR! Could not find " << key << endl;
        break;
    }
    case 2:
    {
        cout << "Enter the age: ";
        int age = getChoice();

        Vector<Olympian*> searchResults;
        if (database.searchByAge(age, searchResults))
        {
            printVector(searchResults);
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
        int height = getChoice();

        Vector<Olympian*> searchResults;

        if (database.searchByHeight(height, searchResults))
        {
            printVector(searchResults);
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
}
void doListSubmenu(int choice, OlympianDatabase &database)
{
    switch (choice)
    {
    case 1:
        database.displayAll();
        break;

    case 2:
        database.displayAlphabeticalOrder();
        break;

    case 3:
        database.displayAgeInOrder();
        break;

    case 4:
        database.displayHeightInOrder();
        break;

    case 5:
        cout << "Indented Age Tree:\n";
        database.displayAgeTree();
        cout << "Indented Height Tree:\n";
        database.displayHeightTree();
        break;

    case 6: // hidden print option
        cout << "\nDeveloped by Aidan Sojourner, Ashley Cline, Alexander Langley, and Jeff Yang\n\n";
        break;

    default:
        cout << "\nYou did not enter a valid option. please enter an option between 1 and 5.\n\n";
    }

}
