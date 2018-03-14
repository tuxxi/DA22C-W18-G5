#include <iostream>
#include <fstream>
#include "OlympianDatabase.h"

#define FILE_READ_ERROR(s) cout << "Error reading the input file " << s << ". Exiting\n", exit(1);

using namespace std;

OlympianDatabase::OlympianDatabase(int hashTableSize, int bucketSize, string infileName)
{
    nRecords = 0;
    //hashTable = new HashTable<Olympian, key>(hashTableSize, bucketSize);
    bstAge = new BinarySearchTree<Olympian>(cmpAge);
    bstHeight = new BinarySearchTree<Olympian>(cmpHeight);
}

/*void OlympianDatabase::buildFromFile(string infileName)
{
    ifstream infile;
    infile.open(infileName);

    if (!infile)
        FILE_READ_ERROR(infileName)

    while (const auto newRecord = readRecord(infile))
            _insert(newRecord);
}*/

const Olympian *OlympianDatabase::readRecord(ifstream &infile)
{
    string input, firstName, lastName, sport, gender, infileName = "winter-olympians.csv";
    int nGold, nSilver, nBronze, age, height;

    while(getline(infile, firstName, ','))
    {
        if (firstName.length())
        {
            getline(infile, lastName, ',');
            getline(infile, gender, ',');
            getline(infile, sport, ',');
            getline(infile, input, ',');

            if (input.find('"') != string::npos)
            {
                getline(infile, input, '"');
                getline(infile, input, ',');
                getline(infile, input, ',');
            }
            else
                (getline(infile, input, ','));

            if (input.find('"') != string::npos)
            {
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

            cout << lastName + ", " << firstName << "; " << sport << "; "  << nGold << ", " << nSilver << ", " << nBronze << endl;
        }
        getline(infile, input);
    }

}

/*bool OlympianDatabase::remove(std::string key)
{
    if (_remove(key))
        return true;

    return false;
}*/


/*const Olympian *OlympianDatabase::searchByName(std::string key)
{
    Olympian *foundRecord;

    if ((foundRecord = hashTable->findEntry(key)))
        return foundRecord;

    cout << key << " was not found in the database." << endl;

    return nullptr;
}

const Olympian *OlympianDatabase::searchByAge(int age)
{
    Olympian foundRecord;

    foundRecord.setAge(age);

    //if ((foundRecord = bstAge->findAllEntries()))
}

const Olympian *OlympianDatabase::searchByHeight(int height)
{

}
*/
/*bool OlympianDatabase::_insert(const Olympian *newRecord)
{
    return (hashTable->insert(newRecord->getName(), *newRecord) && bstHeight->insert(*newRecord)
            && bstAge->insert(*newRecord));
}

bool OlympianDatabase::_remove(std::string key)
{
    Olympian foundRecord;

    foundRecord.setName(key);

    return (hashTable->remove(key) && bstAge->remove(foundRecord) && bstHeight->remove(foundRecord));
}*/

OlympianDatabase::~OlympianDatabase()
{
    delete hashTable;
    delete bstAge;
    delete bstHeight;
}
COMPARE_FN OlympianDatabase::cmpName(const Olympian& a, const Olympian& b)
{
    if (a.getName() < b.getName()) return COMPARE_FN::LESS_THAN;
    else if (a.getName() > b.getName()) return COMPARE_FN::GREATER_THAN;
    return COMPARE_FN::EQUAL_TO;
}
COMPARE_FN OlympianDatabase::cmpAge(const Olympian &a, const Olympian &b)
{
    if (a.getName() == b.getName()) return COMPARE_FN::EQUAL_TO;
    else if (a.getAge() < b.getAge()) return COMPARE_FN::LESS_THAN;
    else if (a.getAge() > b.getAge()) return COMPARE_FN::GREATER_THAN;
    return COMPARE_FN::EQUAL_TO;
}
COMPARE_FN OlympianDatabase::cmpHeight(const Olympian &a, const Olympian &b)
{
    if (a.getName() == b.getName()) return COMPARE_FN::EQUAL_TO;
    else if (a.getHeight() < b.getHeight()) return COMPARE_FN::LESS_THAN;
    else if (a.getHeight() > b.getHeight()) return COMPARE_FN::GREATER_THAN;
    return COMPARE_FN::EQUAL_TO;
}
