//
//  main.cpp
//  22C_Team_Project_Interface
//
//  Created by Ashley Cline on 3/14/18.

#include <fstream>
#include <iostream>
#include <string>
#include "OlympicDatabase.h"

#define DEFAULT_INFILE "winter-olympics.csv"
#define DEFAULT_OUTFILE "winter-olympics-out.csv"

using namespace std;

ifstream getCmndLineInfile(int, const char *[]);
int getChoice();
void displayMenu();
void doMainMenu(int choice, OlympicDatabase &olympianDatabase);

void searchSubMenu();
void doSearchSubmenu(int choice, OlympicDatabase &database);

void listSubMenu();
void doListSubmenu(int choice, OlympicDatabase &database);

void printVector(Vector<Olympian*> &vector);

Olympian *getRecordFromKeyboard();
bool validateName(string);
string formatName(string);
bool validateState(string);
string formatState(string);
bool validateAge(string);
bool validateHeight(string);
bool validatenMedals(string);

// ofstream getOutfile(string infileName);


int main(int argc, const char * argv[])
{
    int choice;
    ifstream infile;

    if (argc > 1)
        infile = getCmndLineInfile(argc, argv);
    else //default file jic
        infile = ifstream(DEFAULT_INFILE);

    if (!infile) return -1;

    OlympicDatabase olympianDatabase(infile);

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


void doMainMenu(int choice, OlympicDatabase &olympianDatabase)
{
    switch (choice)
    {
        case 1:
        {
            Olympian *newRecord;

            if ((newRecord = getRecordFromKeyboard()))
                olympianDatabase.insert(newRecord);

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
void doSearchSubmenu(int choice, OlympicDatabase &database)
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
void doListSubmenu(int choice, OlympicDatabase &database)
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

Olympian *getRecordFromKeyboard()
{
    string name, firstName, lastName, sport, state, ageStr, heightStr, gender, goldStr, silverStr, bronzeStr;
    int age, height;
    int nGold, nSilver, nBronze;
    Olympian *newRecord;

    cout << "Enter the olympian's first name or enter QUIT to go back to the main menu: ";
    cin >> firstName;

    if (firstName == "QUIT")
        return nullptr;

    while (!validateName(firstName))
    {
        cout << "\nYou did not enter a valid name. Names can only contain letters.\n";
        cout << "Please enter a name, or press QUIT to return to the main menu: ";
        cin >> firstName;

        if (firstName == "QUIT")
            return nullptr;
    }

    cout << "Enter the olympian's last name or enter QUIT to go back to the main menu: ";
    cin >> lastName;

    if (lastName == "QUIT")
        return nullptr;

    firstName = formatName(firstName);

    while (!validateName(lastName))
    {
        cout << "\nYou did not enter a valid name. Names can only contain letters.\n";
        cout << "Please enter a name, or press QUIT to return to the main menu: ";
        cin >> lastName;

        if (lastName == "QUIT")
            return nullptr;
    }

    lastName = formatName(lastName);
    name = firstName + " " + lastName;

    cout << "Enter the olympian's home state or enter QUIT to go back to the main menu: ";
    cin >> state;

    if (state == "QUIT")
        return nullptr;

    while (!validateState(state))
    {
        cout << "\nYou did not enter a valid name. Names can only contain letters.\n";
        cout << "Please enter a name, or press QUIT to return to the main menu: ";
        cin >> state;

        if (state == "QUIT")
            return nullptr;
    }

    state = formatState(state);

    cout << "Enter the olympian's age or enter QUIT to return to the main menu: ";
    cin >> ageStr;

    if (ageStr == "QUIT")
        return nullptr;

    while (!validateAge(ageStr))
    {
        cout << "\nYou did not enter a valid age. Ages can only contain integers between\n";
        cout << "14 and 50. Please enter a name, or press QUIT to return to the main menu: ";
        cin >> ageStr;

        if (ageStr == "QUIT")
            return nullptr;
    }

    age = stoi(ageStr);

    cout << "Enter the olympian's height in inches or enter QUIT to return to the main menu: ";
    cin >> heightStr;

    if (heightStr == "QUIT")
        return nullptr;

    while (!validateHeight(heightStr))
    {
        cout << "\nYou did not enter a valid height. Heights only contain integers between\n";
        cout << "36 and 96. Please enter a height, or enter QUIT to return to the main menu: ";
        cin >> heightStr;

        if (heightStr == "QUIT")
            return nullptr;
    }

    height = stoi(heightStr);

    cout << "Enter the olympian's gender or enter QUIT to return to the main menu: ";
    cin >> gender;

    if (gender == "QUIT")
        return nullptr;

    while (!(gender == "M" || gender == "F" || gender == "m" || gender == "f"))
    {
        cout << "\nYou did not enter a valid gender. Please enter a gender M/F: ";
        cin >> gender;

        if (gender == "QUIT")
            return nullptr;
    }

    cout << "Enter the number of gold medals the olympian received, or enter QUIT to return to the"
         << "main menu: ";
    cin >> goldStr;

    if (goldStr == "QUIT")
        return nullptr;

    while (!validatenMedals(goldStr))
    {
        cout << "\nYou did not enter a valid number of medals. Valid numbers are integers between\n";
        cout << "0 and 20. Enter the number of gold medals, or enter QUIT to return to the main menu: ";
        cin >> goldStr;

        if (goldStr == "QUIT")
            return nullptr;
    }

    nGold = stoi(goldStr);

    cout << "Enter the number of silver medals the olympian received, or enter QUIT to return to the"
         << "main menu: ";
    cin >> silverStr;

    if (goldStr == "QUIT")
        return nullptr;

    while (!validatenMedals(silverStr))
    {
        cout << "\nYou did not enter a valid number of medals. Valid numbers are integers between\n";
        cout << "0 and 20. Enter the number of gold medals, or enter QUIT to return to the main menu: ";
        cin >> silverStr;

        if (silverStr == "QUIT")
            return nullptr;
    }

    nSilver = stoi(silverStr);

    cout << "Enter the number of bronze medals the olympian received, or enter QUIT to return to the"
         << "main menu: ";
    cin >> bronzeStr;

    if (bronzeStr == "QUIT")
        return nullptr;

    while (!validatenMedals(bronzeStr))
    {
        cout << "\nYou did not enter a valid number of medals. Valid numbers are integers between\n";
        cout << "0 and 20. Enter the number of gold medals, or enter QUIT to return to the main menu: ";
        cin >> bronzeStr;

        if (bronzeStr == "QUIT")
            return nullptr;
    }

    nBronze = stoi(bronzeStr);

    newRecord = new Olympian(name, sport, state, toupper(gender[0]), age, height, nGold, nSilver, nBronze);

    return newRecord;
}

bool validateName(string name)
{
    int n = name.length();

    for (int i = 0; i < n; i++)
        if (!isalpha(name[i]))
            return false;

    return true;
}

string formatName(string name)
{
    int n = name.length();

    name[0] = toupper(name[0]);

    for (int i = 1; i < n; i++)
        name[i] = tolower(name[i]);

    return name;
}

bool validateState(string state)
{
    return (state.length() == 2 && isalpha(state[0]) && isalpha(state[1]));
}

string formatState(string state)
{
    state[0] = toupper(state[0]);
    state[1] = toupper(state[1]);

    return state;
}

bool validateAge(string ageStr)
{
    int age, n = ageStr.length();

    for (int i = 0; i < n; i++)
        if (!isdigit(ageStr[i]))
            return false;

    age = stoi(ageStr);

    if (age < 15 || age > 50)
        return false;

    return true;
}

bool validateHeight(string heightStr)
{
    int height, n = heightStr.length();

    for (int i = 0; i < n; i++)
        if (!isdigit(heightStr[i]))
            return false;

    height = stoi(heightStr);

    if (height < 36 || height > 96)
        return false;

    return true;
}

bool validatenMedals(string medalStr)
{
    int nMedals, n = medalStr.length();

    for (int i = 0; i < n; i++)
        if (!isdigit(medalStr[i]))
            return false;

    nMedals = stoi(medalStr);

    if (nMedals > 20 || nMedals < 0)
        return false;

    return true;


}