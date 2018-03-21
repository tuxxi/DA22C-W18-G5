#include "OlympianDatabase.h"

#include <fstream>
#include <string>
#include "Util.h"

using namespace std;

static const int BUCKET_SIZE = 3;
static const int SIZE = 512;


OlympianDatabase::OlympianDatabase(ifstream &infile)
    : nRecords(SIZE)
{
    // create hash table based on the prime nearest to double the number of entries
    int size = Util::Primes.getNearestPrime(2 * nRecords);

    hashTable = new HashTable<Olympian*>(size, BUCKET_SIZE, hashFunc, cmpName);
    ageBst = new BinarySearchTree<Olympian*>(cmpAge);
    heightBst = new BinarySearchTree<Olympian*>(cmpHeight);
    allRecords = new Vector<Olympian*>;
    deletionStack = new Stack<Olympian*>;
    alphabeticalOrderList = new LinkedList<Olympian*>(cmpName);

    _buildDatabase(infile);
}

OlympianDatabase::~OlympianDatabase()
{
    delete hashTable;
    delete ageBst;
    delete heightBst;
    delete alphabeticalOrderList;

    delete deletionStack;
    for (auto entry : *allRecords)
    {
        delete entry;
    }
    delete allRecords;
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
Olympian *OlympianDatabase::searchByName(string name)
{
    Olympian o;
    auto foundRecord = &o;
    foundRecord->setName(name);

    if (hashTable->search(foundRecord))
        return foundRecord;

    return nullptr;
}

bool OlympianDatabase::searchByAge(int age, Vector<Olympian*>& vecResults)
{
    Olympian oly;
    auto searchObj = &oly;
    searchObj->setAge(age);

    ageBst->findAllEntries(searchObj, vecResults);
    return !vecResults.empty();
}

bool OlympianDatabase::searchByHeight(int height, Vector<Olympian*>& vecResults)
{
    Olympian oly;
    auto searchObj = &oly;
    searchObj->setHeight(height);

    heightBst->findAllEntries(searchObj, vecResults);
    return !vecResults.empty();
}

Vector<Olympian*> OlympianDatabase::getYoungest()
{
    Olympian *youngest = ageBst->findSmallest();
    Vector<Olympian*> results;

    ageBst->findAllEntries(youngest, results);
    return results;
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
    return results;
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
    std::cout << *ageBst << std::endl;
}

void OlympianDatabase::displayHeightTree()
{
    std::cout << *heightBst << std::endl;
}

void OlympianDatabase::displayHashStats()
{
    cout << "Load Factor: " << hashTable->getLoadFactor() << endl;
    cout << "# of Collisions: " << hashTable->getnCollisions() << endl;
    cout << "Total size: " << hashTable->getnFilled() << endl;
}
void OlympianDatabase::displayAll()
{
    for (auto oly : *allRecords)
    {
        printOlympian(oly);
    }
}
void OlympianDatabase::displayAlphabeticalOrder()
{
    alphabeticalOrderList->displayList();
}

bool OlympianDatabase::saveDatabase(std::string outfileName)
{
    ofstream outfile(outfileName);

    if (!outfile)
        cerr << "\nError saving database. The file " << outfileName << " could not be opened\n";

    for (auto item : *allRecords)
        outfile << item->getName() << "," << item->getSport() << "," << item->getState() << ","
                << item->getAge() << "," << "," << item->getHeight() << "," << item->getGender()
                << item->getnGold() << "," << "," << item->getnSilver() << "," << item->getnBronze() << endl;

    outfile.close();
}

bool OlympianDatabase::_buildDatabase(ifstream &infile)
{
    /*size_t lineNum = 1; //skip first line
    //read all entries into vector*/
    Olympian *newRecord;

    while ((newRecord = _readRecord(infile)))
        insert(newRecord);


#ifdef _DEBUG
    cout << "Hash table size was << " << nRecords << endl;
#endif

    //insert the records into the data structures
    //this is only O(2 * n) so this is fine, and it allows us to control
    //the initial hash table size without rehashing a lot
    /*for (auto oly : *allRecords)
    {
        insert(oly);
    }*/
    return true;
}

Olympian *OlympianDatabase::_readRecord(std::ifstream &infile)
{
    /*//seek to the given line num
    infile.seekg(std::ios::beg);
    for (int i = 0; i < lineNum; ++i) {
        infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }*/
    string input, name, sport, state, gender, goldStr;
    int age, height, nGold, nSilver, nBronze;

    getline(infile, name, ',');

    if (name.length())
    {
        getline(infile, sport, ',');
        getline(infile, state, ',');
        getline(infile, input, ',');
        age = stoi(input);
        getline(infile, input, ',');
        height = stoi(input);
        getline(infile, gender, ',');
        getline(infile, input, ',');
        nGold = stoi(input);
        getline(infile, input, ',');
        nSilver = stoi(input);
        getline(infile, input);
        nBronze = stoi(input);



#ifdef _DEBUG
        cout << lastName + ", " << firstName << ": " << gender << "; " << sport << "; "<< height << ", " << age << "; " << state << endl;
#endif

        auto newEntry = new Olympian(name, sport, state, gender[0], age, height, nGold, nSilver, nBronze);
        //allRecords->add(newEntry);
        //nRecords++;

        return newEntry;
    }

    //EOF or malformed line
    return nullptr;
}

bool OlympianDatabase::insert(Olympian *newEntry)
{
    //insert into hash table first.
    if (!hashTable->insert(newEntry)) return false;

    bool success = true;
    if (!ageBst->insert(newEntry)) success = false;
    if (!heightBst->insert(newEntry)) success = false;
    if (!alphabeticalOrderList->insert(newEntry)) success = false;

    if (!success)
    {
        ageBst->remove(newEntry);
        heightBst->remove(newEntry);
        hashTable->remove(newEntry);
        alphabeticalOrderList->remove(newEntry);
        return false;
    }

    allRecords->add(newEntry);
    nRecords++;

    return true;
}

bool OlympianDatabase::remove(string name)
{
    Olympian o;
    auto foundRecord = &o;
    foundRecord->setName(std::move(name));
    //try to find the record first, and stop if we can't find it
    if (!hashTable->search(foundRecord)) return false;
    bool success = true;
    if (!hashTable->remove(foundRecord)) success = false;
    if (!ageBst->remove(foundRecord)) success = false;
    if (!heightBst->remove(foundRecord)) success = false;
    if (!alphabeticalOrderList->remove(foundRecord)) success = false;

    if (!success)
    {
        //re insert
        insert(foundRecord);
        return false;
    }
    deletionStack->push(foundRecord);
    return true;
}
long int OlympianDatabase::hashFunc(Olympian *const &olympian, long int size)
{
    long int sum = 0;
    string name = olympian->getName();

    for (int i = 0; i < name.length(); i++)
        sum += name[i] * name[i] * name[i];

    return sum % size;
}
COMPARE_FN OlympianDatabase::cmpName(Olympian *const &a, Olympian *const &b)
{
    if (a->getName() == b->getName()) return COMPARE_FN::EQUAL_TO;
    if (a->getName() < b->getName()) return COMPARE_FN::LESS_THAN;
    if (a->getName() > b->getName()) return COMPARE_FN::GREATER_THAN;
    return COMPARE_FN::EQUAL_TO;
}
COMPARE_FN OlympianDatabase::cmpAge(Olympian *const &a, Olympian *const &b)
{
    if (a->getName() == b->getName()) return COMPARE_FN::EQUAL_TO;
    if (a->getAge() < b->getAge()) return COMPARE_FN::LESS_THAN;
    if (a->getAge() > b->getAge()) return COMPARE_FN::GREATER_THAN;
    return COMPARE_FN::EQUAL_TO;
}
COMPARE_FN OlympianDatabase::cmpHeight(Olympian *const &a, Olympian *const &b)
{
    if (a->getName() == b->getName()) return COMPARE_FN::EQUAL_TO;
    if (a->getHeight() < b->getHeight()) return COMPARE_FN::LESS_THAN;
    if (a->getHeight() > b->getHeight()) return COMPARE_FN::GREATER_THAN;
    return COMPARE_FN::EQUAL_TO;
}
int OlympianDatabase::int_cmpName(Olympian *const &a, Olympian *const &b)
{
    return int(cmpName(a, b));
}

void OlympianDatabase::printOlympian(Olympian *&olympian)
{
    if (olympian)
        cout << *olympian << endl;
    else
        cout << "ERROR! null pointer in database. \n";
}
