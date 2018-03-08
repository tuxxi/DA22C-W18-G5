#include <iostream>
#include <fstream>
#include <string>
#include "OlympianDatabase.h"
#include "Olympian.h"

#define FILE_READ_ERROR(s) cout << "Error reading the input file " << s << ". Exiting\n", exit(1);
#define LINE_SIZE 300

using namespace std;

OlympianDatabase::OlympianDatabase(int hashTableSize, int bucketSize, string infileName)
{
    nRecords = 0;
    hashTable = new HashTable<Olympian, key>(hashTableSize, bucketSize);
    bstAge = new BinarySearchTree<Olympian>;
    bstHeight = new BinarySearchTree<Olympian>;
}

void OlympianDatabase::buildFromFile(string infileName)
{
    Olympian *newRecord;
    ifstream infile;

    if (!infile.open(infileName))
        FILE_READ_ERROR(infileName)

    fgets(input, LINE_SIZE, infile);

    while ((newRecord = readRecord()))
        _insert(newRecord);
}

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
        getline(infile, input, '\n');
    }

}

/*const Olympian *OlympianDatabase::searchByName(std::string key)
{
    Olympian *foundRecord;

    if ((foundRecord = hashTable->findEntry(key)))
        cout << *foundRecord;
    else
        cout << key << " was not found in the database." << endl;
}

const Olympian *OlympianDatabase::searchByAge(int age)
{
    Olympian foundRecord;

    foundRecord.setAge(age);

    //if ((foundRecord = bstAge->getEntry()))
}

const Olympian *OlympianDatabase::searchByHeight(int height)
{

}

bool OlympianDatabase::_insert(const Olympian *newRecord)
{
    return (hashTable->insert(newRecord->getName(), newRecord) && bstHeight->insert(newRecord)
            && bstAge->insert(newRecord));
}*/

int cmpName(const Olympian *olympian1, const Olympian *olympian2)
{
    if (olympian1->getName() < olympian2->getName())
        return 1;
    else if (olympian1->getName() > olympian2->getName())
        return -1;

    return 0;
}

int cmpAge(const Olympian *olympian1, const Olympian *olympian2)
{
    if (olympian1->getAge() < olympian2->getAge())
        return 1;
    else if (olympian2->getAge() > olympian2->getAge())
        return -1;

    return 0;
}

int cmpHeight(const Olympian *olympian1, const Olympian *olympian2)
{
    if (olympian->getHeight() < olympian2->getHeight())
        return 1;
    else if (olympian1->getHeight() > olympian2->getHeight())
        return -1;

    return 0;
}

OlympianDatabase::~OlympianDatabase()
{
    delete hashTable;
    delete bstAge;
    delete bstHeight;
}
