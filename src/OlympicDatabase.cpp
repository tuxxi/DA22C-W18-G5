#include "OlympicDatabase.h"

#include <fstream>
#include <string>
#include <iomanip>
#include "Util.h"

using namespace std;

static const int BUCKET_SIZE = 3;
static const int SIZE = 512;


OlympicDatabase::OlympicDatabase(ifstream &infile)
{
    // create hash table based on the prime nearest to double the number of entries
    int size = Util::Primes.getNearestPrime(SIZE);

    hashTable = new HashTable<Olympian *>(size, BUCKET_SIZE, _hashFunc, _cmpName);
    ageBst = new BinarySearchTree<Olympian *>(_cmpAge);
    heightBst = new BinarySearchTree<Olympian *>(_cmpHeight);
    allRecords = new Vector<Olympian*>;
    deletionStack = new Stack<Olympian*>;
    alphabeticalOrderList = new LinkedList<Olympian *>(_cmpName);

    _buildDatabase(infile);
}

OlympicDatabase::~OlympicDatabase()
{
    delete hashTable;
    delete ageBst;
    delete heightBst;
    delete alphabeticalOrderList;
    delete deletionStack;

    for (auto entry : *allRecords)
        delete entry;

    delete allRecords;
}

bool OlympicDatabase::undoDelete()
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
Olympian *OlympicDatabase::searchByName(string name)
{
    Olympian o;
    auto foundRecord = &o;
    foundRecord->setName(name);

    if (hashTable->search(foundRecord))
        return foundRecord;

    return nullptr;
}

bool OlympicDatabase::searchByAge(int age, Vector<Olympian*>& vecResults)
{
    Olympian oly;
    auto searchObj = &oly;
    searchObj->setAge(age);

    ageBst->findAllEntries(searchObj, vecResults);
    return !vecResults.empty();
}

bool OlympicDatabase::searchByHeight(int height, Vector<Olympian*>& vecResults)
{
    Olympian oly;
    auto searchObj = &oly;
    searchObj->setHeight(height);

    heightBst->findAllEntries(searchObj, vecResults);
    return !vecResults.empty();
}

void OlympicDatabase::displayAgeInOrder()
{
    Vector<Olympian*> printVec;
    ageBst->insertInorder(printVec);
    cout << printVec;
}

void OlympicDatabase::displayHeightInOrder()
{
    Vector<Olympian*> printVec;
    heightBst->insertInorder(printVec);
    cout << printVec;
}

void OlympicDatabase::displayAgeTree()
{
    ageBst->printIndented(_printAgeIndented);
}

void OlympicDatabase::displayHeightTree()
{
    heightBst->printIndented(_printHeightIndented);
}

void OlympicDatabase::displayHashStats()
{
    double loadFactor = hashTable->getLoadFactor();

    loadFactor *= 100;

    cout << "Current Table Size: " << hashTable->getTableSize() << endl;
    cout << "Number of Buckets Used: " << hashTable->getnFilled() << endl;
    cout << "Current Load Factor: " << setprecision(2) << fixed << loadFactor << "%\n";
    cout << "# of Collisions: " << hashTable->getnCollisions() << endl;
}
void OlympicDatabase::displayAll()
{
    for (auto oly : *allRecords)
    {
        cout << *oly;
    }
}
void OlympicDatabase::displayAlphabeticalOrder()
{
    alphabeticalOrderList->displayList();
}

bool OlympicDatabase::saveDatabase(std::string outfileName)
{
    ofstream outfile(outfileName);

    if (!outfile)
    {
        cerr << "\nError saving database. The file " << outfileName << " could not be opened\n";
        return false;
    }

    for (auto item : *allRecords)
        _printRecord(item, outfile);

    outfile.close();
    return true;
}

bool OlympicDatabase::_buildDatabase(ifstream &infile)
{
    while (auto newRecord = _readRecord(infile))
        if (insert(newRecord))
            allRecords->add(newRecord);

    return true;
}

Olympian *OlympicDatabase::_readRecord(std::ifstream &infile)
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
        cout << name << ": " << gender << "; " << sport << "; "<< height << ", " << age << "; " << state << endl;
#endif

        auto newEntry = new Olympian(name, sport, state, gender[0], age, height, nGold, nSilver, nBronze);
        return newEntry;
    }

    //EOF or malformed line
    return nullptr;
}

bool OlympicDatabase::insert(Olympian *newEntry)
{
    //insert into hash table first.
    if (hashTable->insert(newEntry))
        if (ageBst->insert(newEntry))
            if (heightBst->insert(newEntry))
                if (alphabeticalOrderList->insert(newEntry))
                {
                    nRecords++;

                    return true;
                }
                else
                    heightBst->remove(newEntry);
            else
                ageBst->remove(newEntry);
        else
            hashTable->remove(newEntry);

    return false;
}

bool OlympicDatabase::remove(string name)
{
    Olympian o;
    auto foundRecord = &o;
    foundRecord->setName(std::move(name));
    //try to find the record first, and stop if we can't find it
    if (!hashTable->search(foundRecord)) return false;

    if (hashTable->remove(foundRecord))
        if (ageBst->remove(foundRecord))
            if (heightBst->remove(foundRecord))
                if (alphabeticalOrderList->remove(foundRecord))
                {
                    nRecords--;
                    deletionStack->push(foundRecord);

                    return true;
                }
                else
                    heightBst->insert(foundRecord);
            else
                ageBst->insert(foundRecord);
        else
            hashTable->insert(foundRecord);

    return false;
}
long int OlympicDatabase::_hashFunc(Olympian *const &olympian, long int size)
{
    long int sum = 0;
    string name = olympian->getName();

    for (int i = 0; i < name.length(); i++)
        sum += name[i] * name[i] * name[i];

    return sum % size;
}
COMPARE_FN OlympicDatabase::_cmpName(Olympian *const &a, Olympian *const &b)
{
    if (a->getName() < b->getName()) return COMPARE_FN::LESS_THAN;
    else if (a->getName() > b->getName()) return COMPARE_FN::GREATER_THAN;
    else return COMPARE_FN::EQUAL_TO;
}
COMPARE_FN OlympicDatabase::_cmpAge(Olympian *const &a, Olympian *const &b)
{
    if (a->getName() == b->getName()) return COMPARE_FN::EQUAL_OBJECT;

    else if (a->getAge() < b->getAge()) return COMPARE_FN::LESS_THAN;
    else if (a->getAge() > b->getAge()) return COMPARE_FN::GREATER_THAN;
    else return COMPARE_FN::EQUAL_TO;
}
COMPARE_FN OlympicDatabase::_cmpHeight(Olympian *const &a, Olympian *const &b)
{
    if (a->getName() == b->getName()) return COMPARE_FN::EQUAL_OBJECT;

    else if (a->getHeight() < b->getHeight()) return COMPARE_FN::LESS_THAN;
    else if (a->getHeight() > b->getHeight()) return COMPARE_FN::GREATER_THAN;
    else return COMPARE_FN::EQUAL_TO;
}

void OlympicDatabase::_printRecord(Olympian *const &olympian, std::ostream &os)
{
    os << olympian->getName() << "," << olympian->getSport() << "," << olympian->getState()
       << "," << olympian->getAge() << "," << olympian->getHeight() << "," << olympian->getGender()
       << "," << olympian->getnGold() << "," << olympian->getnSilver() << "," << olympian->getnBronze() << endl;
}

void OlympicDatabase::_printAgeIndented(Olympian *const &object, const int level)
{
    for(int i=0; i<(level + 1) * 5 + 1; i++)
        std::cout<<" ";

    std::cout<<object->getName()<<" "<<object->getAge()<<std::endl;
}

void OlympicDatabase::_printHeightIndented(Olympian *const &object, const int level)
{
    for(int i=0; i<(level + 1) * 5 + 1; i++)
        std::cout<<" ";

    std::cout<<object->getName()<<" "<<object->getHeight()<<std::endl;
}