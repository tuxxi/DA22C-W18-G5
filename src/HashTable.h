#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "Vector.h"
#include "Stack.h"
#include "Util.h"

#define MAX_LOAD_FACTOR 0.6

template <class T>
class HashTable
{
private:
    long int tableSize;
    int bucketSize;
    long int tableSpacesFilled;
    int nCollisions;
    int nResizes;
    T **table;
    bool **isFilled;
    int *nFilled;
    Vector<T> *overflowArea;

    typedef long int (*hashFn)(const T&, long int);
    hashFn _hash;

    typedef COMPARE_FN (*cmpFn)(const T&, const T&);
    cmpFn cmp;

    bool _insert(const T&);
    bool _insertOverflow(const T&);
    bool _removeOverflow(const T&);
    bool _searchOverflow(T&);
    bool _checkDuplicate(const T&, long int);
    bool _remove(const T&);
    bool _search(T&);
    bool _resize(long int);
    void _deleteTable(T**, bool**, int*, Vector<T>*, long int);

public:
    HashTable(long int, int, hashFn, cmpFn, int nResizes = 0);

    ~HashTable();

    long int getTableSize() { return tableSize; }
    long int getnFilled() { return tableSpacesFilled; }
    int getnCollisions() { return nCollisions; }
    double getLoadFactor() { return static_cast<double>(tableSpacesFilled) / tableSize; }
    int getnResizes() { return nResizes; }

    bool insert(const T&);
    bool remove(const T&);
    bool search(T&);
};

/******************************************************
   Constructor for the hash table. Takes the table size,
   bucket size, hash function and comparison function
   that will be used to compare records in the table.
 */
template <class T>
HashTable<T>::HashTable(long int tableSize, int bucketSize, hashFn hash, cmpFn cmp, int nResizes)
{
    this->tableSize = tableSize;
    this->bucketSize = bucketSize;
    this->tableSpacesFilled = 0;
    this->nCollisions = 0;
    this->nResizes = nResizes;
    this->table = new T*[tableSize] {nullptr};
    this->isFilled = new bool*[tableSize] {nullptr};
    this->nFilled = new int[tableSize] {0};
    this->overflowArea = new Vector<T>;
    this->_hash = hash;
    this->cmp = cmp;
}

template <class T>
bool HashTable<T>::insert(const T &newData)
{
    if (!_insert(newData))
        return false;

    if (this->getLoadFactor() >= MAX_LOAD_FACTOR)
    {
        int newSize = tableSize * 2;

        while (!_resize(Util::Primes.getNearestPrime(newSize)))
            newSize *= 2;
    }

    return true;
}

template <class T>
bool HashTable<T>::remove(const T &removalKey)
{
    return (_remove(removalKey) || _removeOverflow(removalKey));
}

template <class T>
bool HashTable<T>::search(T &searchObj)
{
    return (_search(searchObj) || _searchOverflow(searchObj));
}

template <class T>
bool HashTable<T>::_insert(const T &newData)
{
    long int hashVal = _hash(newData, tableSize);
    T searchObj;

    if (table[hashVal])
    {
        if (!_checkDuplicate(newData, hashVal) && !_searchOverflow((searchObj = newData)))
        {
            if (nFilled[hashVal] < bucketSize)
            {
                for (int i = 0; i < bucketSize; i++)
                    if (!isFilled[hashVal][i])
                    {
                        table[hashVal][i] = newData;
                        isFilled[hashVal][i] = true;
                        nFilled[hashVal]++;
                        nCollisions++;

                        return true;
                    }
            }
            else
                return (_insertOverflow(newData));
        }
    }
    else
    {
        table[hashVal] = new T[bucketSize]{ T() };
        isFilled[hashVal] = new bool[bucketSize] {false};
        table[hashVal][0] = newData;
        isFilled[hashVal][0] = true;
        nFilled[hashVal]++;
        tableSpacesFilled++;

        return true;
    }

    return false;
}

/******************************************************
   Insert a record into the overflow area.
 */
template <class T>
bool HashTable<T>::_insertOverflow(const T &newData)
{
    if (overflowArea->add(newData))
    {
        nCollisions++;

        return true;
    }

    return false;
}

/******************************************************
   Remove a record from the overflow area
 */
template <class T>
bool HashTable<T>::_removeOverflow(const T &removalKey)
{
    int n = overflowArea->size();

    for (int i = 0; i < n; i++)
        if (cmp((*overflowArea)[i], removalKey) == COMPARE_FN::EQUAL_TO)
        {
            overflowArea->remove(i);

            return true;
        }

    return false;
}

template <class T>
bool HashTable<T>::_searchOverflow(T &searchObj)
{
    int n = overflowArea->size();

    for (int i = 0; i < n; i++)
        if (cmp((*overflowArea)[i], searchObj) == COMPARE_FN::EQUAL_TO)
        {
            searchObj = (*overflowArea)[i];

            return true;
        }

    return false;
}

/******************************************************
   Checks a bucket and returns true if a record identical
   to newData is found in it. Return false otherwise.
 */
template <class T>
bool HashTable<T>::_checkDuplicate(const T &newData, long int hashVal)
{
    for (int i = 0; i < bucketSize; i++)
        if (isFilled[hashVal][i] && cmp(newData, table[hashVal][i]) == COMPARE_FN::EQUAL_TO)
            return true;

    return false;
}

/******************************************************
   Try to remove an item from the main table. Return false
   if the record is not found.
 */
template <class T>
bool HashTable<T>::_remove(const T &removalKey)
{
    long int hashVal = _hash(removalKey, tableSize);

    if (table[hashVal])
        for (int i = 0; i < bucketSize; i++)
            if (isFilled[hashVal][i] && cmp(table[hashVal][i], removalKey) == COMPARE_FN::EQUAL_TO)
            {
                table[hashVal][i] = T();
                isFilled[hashVal][i] = false;
                nFilled[hashVal]--;

                if (!nFilled[hashVal])
                {
                    delete table[hashVal];
                    table[hashVal] = nullptr;
                    tableSpacesFilled--;
                }

                return true;
            }

    return false;
}

/******************************************************
   Search for a record in the main table. Return false
   if the record is not found.
 */
template <class T>
bool HashTable<T>::_search(T &searchObj)
{
    long int hashVal = _hash(searchObj, tableSize);

    if (table[hashVal])
        for (int i = 0; i < bucketSize; i++)
            if (isFilled[hashVal][i] && cmp(searchObj, table[hashVal][i]) == COMPARE_FN::EQUAL_TO)
            {
                searchObj = table[hashVal][i];
                return true;
            }

    return false;
}

/******************************************************
   Resize the hash table. Creates a new HashTable object
   and rehashes all entries. Assigns the new HashTable
   object to *this and deletes the old one. Return false
   if the resize operation triggers another resize.
 */
template <class T>
bool HashTable<T>::_resize(long int newSize)
{
    HashTable<T> *newHashTable = new HashTable<T>(newSize, bucketSize, _hash, cmp, nResizes + 1);

    for (int i = 0; i < tableSize; i++)
        if (table[i])
            for (int j = 0; j < bucketSize; j++)
                if (isFilled[i][j])
                    if (!newHashTable->_insert(table[i][j]) && !newHashTable->_insertOverflow(table[i][j])
                        || newHashTable->getLoadFactor() >= MAX_LOAD_FACTOR)
                    {
                        delete newHashTable;

                        return false;
                    }

    for (auto item : *overflowArea)
        if (!newHashTable->_insert(item) && !newHashTable->_insertOverflow(item)
            || newHashTable->getLoadFactor() >= MAX_LOAD_FACTOR)
        {
            delete newHashTable;

            return false;
        }

    *this = *newHashTable;

    return true;
}

/******************************************************
   Free all allocated memory in the HashTable object
 */
template <class T>
HashTable<T>::~HashTable()
{
    T **ptr1 = table;
    bool **ptr2 = isFilled;

    while (ptr1 < table + tableSize)
    {
        if (*ptr1)
        {
            delete *ptr1;
            delete *ptr2;
        }

        ptr1++;
        ptr2++;
    }

    delete nFilled;
    delete overflowArea;
}

#endif // HASH_TABLE_H