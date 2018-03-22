// Implementation file for the OlympicDatabase class

#include "OlympicDatabase.h"

#include <fstream>
#include <string>
#include <iomanip>
#include "Util.h"

using namespace std;

static const int BUCKET_SIZE = 3;
static const int SIZE = 100;

/******************************************************
   Accepts an already open input file stream and builds
   the database from the file
 */
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

/******************************************************
   Destroy all the data in the database.
 */
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

/******************************************************
   Undo previous deletions by popping from the stack of
   deleted items and inserting them into the database.
 */
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

/******************************************************
   Search the hash table to find an olympian by name.
 */
Olympian *OlympicDatabase::searchByName(string name)
{
    Olympian o;
    auto foundRecord = &o;
    foundRecord->setName(name);

    if (hashTable->search(foundRecord))
        return foundRecord;

    return nullptr;
}

/******************************************************
   Search the BST that is sorted by age to return all
   results that match a particular age.
 */
bool OlympicDatabase::searchByAge(int age, Vector<Olympian*>& vecResults)
{
    Olympian oly;
    auto searchObj = &oly;
    searchObj->setAge(age);

    ageBst->findAllEntries(searchObj, vecResults);
    return !vecResults.empty();
}

/******************************************************
   Search the BST that is sorted by age to return all
   results that match a particular height.
 */
bool OlympicDatabase::searchByHeight(int height, Vector<Olympian*>& vecResults)
{
    Olympian oly;
    auto searchObj = &oly;
    searchObj->setHeight(height);

    heightBst->findAllEntries(searchObj, vecResults);
    return !vecResults.empty();
}

/******************************************************
   Display the BST that is sorted by age inorder.
 */
void OlympicDatabase::displayAgeInOrder()
{
    Vector<Olympian*> printVec;
    ageBst->insertInorder(printVec);
    cout << printVec;
}

/******************************************************
   Display the BST that is sorted by height inorder.
 */
void OlympicDatabase::displayHeightInOrder()
{
    Vector<Olympian*> printVec;
    heightBst->insertInorder(printVec);
    cout << printVec;
}

/******************************************************
   Display the BST that is sorted by age as an indented
   tree.
 */
void OlympicDatabase::displayAgeTree()
{
    ageBst->printIndented(_printAgeIndented);
}

/******************************************************
   Display the BST that is sorted by height as an indented
   tree.
 */
void OlympicDatabase::displayHeightTree()
{
    heightBst->printIndented(_printHeightIndented);
}

/******************************************************
   Display the current size, number of buckets used,
   load factor, and number of collisions for the hash
   table.
 */
void OlympicDatabase::displayHashStats()
{
    double loadFactor = hashTable->getLoadFactor();

    loadFactor *= 100;

    cout << "Current Table Size: " << hashTable->getTableSize() << endl;
    cout << "Number of Buckets Used: " << hashTable->getnFilled() << endl;
    cout << "Current Load Factor: " << setprecision(2) << fixed << loadFactor << "%\n";
    cout << "Number of Collisions: " << hashTable->getnCollisions() << endl;
    cout << "Total Number of Resizes: " << hashTable->getnResizes() << endl;
}

/******************************************************
   Display the unsorted data.
 */
void OlympicDatabase::displayAll()
{
    for (auto oly : *allRecords)
    {
        cout << *oly;
    }
}

/******************************************************
   Display all records in alphabetical order by traversing
   the linked list that is sorted by name.
 */
void OlympicDatabase::displayAlphabeticalOrder()
{
    alphabeticalOrderList->displayList();
}

/******************************************************
   Save all records to an output file.
 */
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

/******************************************************
   Build the database from file. Keep inserting records
   as long as there are more records in the file.
 */
bool OlympicDatabase::_buildDatabase(ifstream &infile)
{
    while (auto newRecord = _readRecord(infile))
        if (insert(newRecord))
            allRecords->add(newRecord);

    return true;
}

/******************************************************
   Read one record from the input file and create a new
   Olympian object to be inserted into the database.
 */
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

/******************************************************
   Insert a record into every data structure in the
   database. If insertion into one structure is not
   successful, remove from the ones into which it has
   already been inserted.
 */
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
/******************************************************
   Remove a record from the entire database. If deletion
   from any data structure is not successful, re-inserts
   the record into the structures for which deletion was
   already done.
 */
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

/******************************************************
   Hash function to be passed into the hash table constructor.
 */
long int OlympicDatabase::_hashFunc(Olympian *const &olympian, long int size)
{
    string name = olympian->getName();

    const size_t arraysize = name.length() * sizeof(char);
    const unsigned CRC32 = Util::CRC32.CalcCRC32(name.data(), arraysize);

    return CRC32 % size;
}
/******************************************************
   Comparison function that takes two pointers to
   Olympian objects and compares their names.
 */
COMPARE_FN OlympicDatabase::_cmpName(Olympian *const &a, Olympian *const &b)
{
    if (a->getName() < b->getName()) return COMPARE_FN::LESS_THAN;
    else if (a->getName() > b->getName()) return COMPARE_FN::GREATER_THAN;
    else return COMPARE_FN::EQUAL_TO;
}
/******************************************************
   Comparison function that takes two pointers to
   Olympian objects and compares their ages.
 */
COMPARE_FN OlympicDatabase::_cmpAge(Olympian *const &a, Olympian *const &b)
{
    if (a->getName() == b->getName()) return COMPARE_FN::EQUAL_OBJECT;

    else if (a->getAge() < b->getAge()) return COMPARE_FN::LESS_THAN;
    else if (a->getAge() > b->getAge()) return COMPARE_FN::GREATER_THAN;
    else return COMPARE_FN::EQUAL_TO;
}

/******************************************************
   Comparison function that takes two pointers to
   Olympian objects and compares their heights.
 */
COMPARE_FN OlympicDatabase::_cmpHeight(Olympian *const &a, Olympian *const &b)
{
    if (a->getName() == b->getName()) return COMPARE_FN::EQUAL_OBJECT;

    else if (a->getHeight() < b->getHeight()) return COMPARE_FN::LESS_THAN;
    else if (a->getHeight() > b->getHeight()) return COMPARE_FN::GREATER_THAN;
    else return COMPARE_FN::EQUAL_TO;
}

/******************************************************
   Print a record in a format appropriate to a csv file.
 */
void OlympicDatabase::_printRecord(Olympian *const &olympian, std::ostream &os)
{
    os << olympian->getName() << "," << olympian->getSport() << "," << olympian->getState()
       << "," << olympian->getAge() << "," << olympian->getHeight() << "," << olympian->getGender()
       << "," << olympian->getnGold() << "," << olympian->getnSilver() << "," << olympian->getnBronze() << endl;
}

/******************************************************
   Function to passed to the print indented list function
   of the BST that is sorted by age. Prints the name and
   age of the input object.
 */
void OlympicDatabase::_printAgeIndented(Olympian *const &object, const int level)
{
    for(int i = 0; i < (level + 1) * 5 + 1; i++)
        cout << " ";

    cout << object->getName() << " -- " <<object->getAge() << " years old" << endl;
}

/******************************************************
   Function to passed to the print indented list function
   of the BST that is sorted by height. Prints the name and
   height of the input object.
 */
void OlympicDatabase::_printHeightIndented(Olympian *const &object, const int level)
{
    for(int i = 0; i < (level + 1) * 5 + 1; i++)
        cout << " ";

    cout << object->getName() << " -- " << object->getHeight() << " inches tall" << endl;
}