//
//  main.cpp
//  22C_Team_Project_Interface
//
//  Created by Ashley Cline on 3/14/18.
//  Copyright © 2018 Ashley Cline. All rights reserved.
//

#include <fstream>
#include <iostream>
#include <string>
#include "OlympianDatabase.h"

void saveOutFile();
int getChoice();
void displayMenu();
void implementDecision(int choice, OlympianDatabase &olympic);
void searchSubMenu();
void listSubMenu();


int main(int argc, const char * argv[])
{
    std::ifstream file("data/winter-olympians.csv");
    if (!file) return -1;

    OlympianDatabase olympic(file);

    displayMenu();
    int choice = getChoice();

    while (choice != 8)
    {
        implementDecision(choice, olympic);
        displayMenu();
    }

    // write output to file

    return 0;
}

int getChoice()
{
    int choice = 0;
    std::cin >> choice;
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

void implementDecision(int choice, OlympianDatabase &olympic)
{
    if (choice == 1)
    {
        Olympian * temp;
        temp = new Olympian;
        olympic.insert(temp);
    }

    else if (choice == 2)
    {
        Olympian dataOut;
        std::string key;
        std::cout << "Enter name of Olympian for deletion: ";
        std::cin >> key;
        olympic.remove(key);
    }

    else if (choice == 3)
    {
        //olympic.undo();
    }

    else if (choice == 4)
    {
        searchSubMenu();
        int searchOp;
        std::cin >> searchOp;
        if (searchOp == 9)
        {
            std::cout << "Enter name of Olympian: ";
            std::string searchKey;
            std::cin >> searchKey;
            Olympian *searchObj = olympic.searchByName(searchKey);

            //print out results
        }
        else if (searchOp == 10)
        {
            std::cout << "Enter the age: ";
            int age;
            std::cin >> age;
            Vector<Olympian*> v = olympic.searchByAge(age);

            //print out results
        }
        else if (searchOp == 11)
        {
            std::cout << "Enter the height: ";
            int height;
            std::cin >> height;
            Vector<Olympian*> v = olympic.searchByHeight(height);
            //print out results

        }
        else
            std::cout << "Invalid entry!\n";
    }

    else if (choice == 5)
    {
        listSubMenu();
        int listOp;
        std::cin >> listOp;
        if (listOp == 12)
        {
            // print all data from stack
        }
        else if (listOp == 13)
        {
            // print all names
        }
        else if (listOp == 14)
        {
            olympic.displayAgeInOrder();
        }
        else if (listOp == 15)
        {
            olympic.displayHeightInOrder();
        }
        else if (listOp == 16)
        {
            std::cout << "Indented Age Tree:\n";
            olympic.displayAgeTree();
            std::cout << "Indented Height Tree:\n";
            olympic.displayHeightTree();
        }
        else if (listOp == 17) // hidden print option
        {
            std::cout << "Aidan, Alexander, Ashley, and Jeff\n";
        }
        else
            std::cout << "Invalid entry!\n";
    }

    else if (choice == 6)
    {
        // save data to output file
    }

    else if (choice == 7)
    {
        // get load factor, number of collisions, etc from hash
    }

    else if (choice == 8)
        exit(0);

    else
        std::cout << "Invalid entry!\n";
}

void searchSubMenu()
{
    std::cout << "Choose a search operation:\n";
    std::cout << "9. Find and Display an Olympian by Name\n";
    std::cout << "10. Find and Display Olympians by Age\n";
    std::cout << "11. Find and Display Olympians by Height\n";
}

void listSubMenu()
{
    std::cout << "Choose a list operation:\n";
    std::cout << "12. List Unsorted Olympians\n";
    std::cout << "13. List Olympians Sorted by Name\n";
    std::cout << "14. List Olympians Sorted by Age\n";
    std::cout << "15. List Olympians Sorted by Height\n";
    std::cout << "16. Print Data as Indented List\n";
}
