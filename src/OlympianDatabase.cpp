#include "OlympianDatabase.h"

using namespace std;

OlympianDatabase::OlympianDatabase(ifstream &infile)
{
    // hashTable = new HashTable<Olympian*, string>;
    ageBst = new BinarySearchTree<Olympian*>(cmpAge);
    heightBst = new BinarySearchTree<Olympian*>(cmpHeight);
}

bool OlympianDatabase::undoDelete()
{
    Olympian *lastDeleted;

    if (deletionStack->pop(lastDeleted))
    {
        if (insert(lastDeleted))
            return true;

        deletionStack->push(lastDeleted);

        cerr << "Attempted to undo previous deletion. But the deleted item could not be"
                "\ninserted back into the database\n";

        return false;
    }

    cout << "\nThere are no previous deletions to undo.\n" << endl;

    return false;

}

bool OlympianDatabase::insert(Olympian *newRecord)
{
    return _insert(newRecord);
}

Olympian *OlympianDatabase::searchByName(string name)
{
    Olympian *foundRecord;

    //if (hashTable->findEntry(name, foundRecord))
        return foundRecord;

    return nullptr;
}

Vector<Olympian*> OlympianDatabase::searchByAge(int age)
{
    Vector<Olympian*> results;
    Olympian *searchObj = new Olympian;

    searchObj->setAge(age);

    ageBst->findAllEntries(searchObj, results);

    delete searchObj;

    return results;
}

Vector<Olympian*> OlympianDatabase::searchByHeight(int height)
{
    Vector<Olympian*> results;
    Olympian *searchObj = new Olympian;

    searchObj->setHeight(height);

    heightBst->findAllEntries(searchObj, results);

    delete searchObj;

    return results;
}

Vector<Olympian*> OlympianDatabase::getYoungest()
{
    Olympian *youngest = ageBst->findSmallest();
    Vector<Olympian*> results;

    ageBst->findAllEntries(youngest, results);
}

Vector<Olympian*> OlympianDatabase::getOldest()
{
    Olympian *oldest = ageBst->findLargest();
    Vector<Olympian*> results;

    ageBst->findAllEntries(oldest, results);

    return results;
}

Vector<Olympian*> OlympianDatabase::getShortest()
{
    Olympian *shortest = heightBst->findSmallest();
    Vector<Olympian*> results;

    heightBst->findAllEntries(shortest, results);
}

Vector<Olympian*> OlympianDatabase::getTallest()
{
    Olympian *tallest = heightBst->findLargest();
    Vector<Olympian*> results;

    heightBst->findAllEntries(tallest, results);

    return results;
}

void OlympianDatabase::displayAgeInOrder()
{
    ageBst->inOrder(printOlympian);
}

void OlympianDatabase::displayHeightInOrder()
{
    heightBst->inOrder(printOlympian);
}

void OlympianDatabase::displayAgeTree()
{
    ageBst->inOrder(printOlympian);
}

void OlympianDatabase::displayHeightTree()
{
    heightBst->inOrder(printOlympian);
}

bool OlympianDatabase::_buildDatabase(ifstream &infile)
{
    Olympian *newRecord;

    while ((newRecord = _readRecord(infile)))
        _insert(newRecord);
}

Olympian *OlympianDatabase::_readRecord(std::ifstream &infile)
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

bool OlympianDatabase::_insert(Olympian *newRecord)
{
    //if (hashTable->insert(newRecord->getName(), newRecord))
        if (ageBst->insert(newRecord))
            if (heightBst->insert(newRecord))
            {
                allRecords->add(newRecord);

                return true;
            }
            else
                ageBst->remove(newRecord);
        //else
          //  hashTable->remove(newRecord->getName(), newRecord);

    return false;
}

bool OlympianDatabase::_remove(string)
{
    Olympian *foundRecord;


    //if (hashTable->remove(foundRecord->getName(), foundRecord))
        if (ageBst->remove(foundRecord))
            if (heightBst->remove(foundRecord))
            {
                deletionStack->push(foundRecord);

                return true;
            }
            else
                ageBst->insert(foundRecord);
        //else
          //  hashTable->insert(foundRecord, foundRecord->getName());

    return false;
}

COMPARE_FN OlympianDatabase::cmpName(Olympian *const &a, Olympian *const &b)
{
    if (a->getName() < b->getName()) return COMPARE_FN::LESS_THAN;
    else if (a->getName() > b->getName()) return COMPARE_FN::GREATER_THAN;
    return COMPARE_FN::EQUAL_TO;
}
COMPARE_FN OlympianDatabase::cmpAge(Olympian *const &a, Olympian *const &b)
{
    if (a->getName() == b->getName()) return COMPARE_FN::EQUAL_TO;
    else if (a->getAge() < b->getAge()) return COMPARE_FN::LESS_THAN;
    else if (a->getAge() > b->getAge()) return COMPARE_FN::GREATER_THAN;
    return COMPARE_FN::EQUAL_TO;
}
COMPARE_FN OlympianDatabase::cmpHeight(Olympian *const &a, Olympian *const &b)
{
    if (a->getName() == b->getName()) return COMPARE_FN::EQUAL_TO;
    else if (a->getHeight() < b->getHeight()) return COMPARE_FN::LESS_THAN;
    else if (a->getHeight() > b->getHeight()) return COMPARE_FN::GREATER_THAN;
    return COMPARE_FN::EQUAL_TO;
}

void OlympianDatabase::printOlympian(Olympian *&olympian)
{
    cout << *olympian << endl;
}

OlympianDatabase::~OlympianDatabase()
{
    //delete hashTable;
    delete ageBst;
    delete heightBst;
    delete deletionStack;
    delete allRecords;
}