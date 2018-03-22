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
    int tableSpacesFilled;
    int nCollisions;
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
    HashTable(long int, int, hashFn, cmpFn);

    ~HashTable();

    long int getTableSize() { return tableSize; }
    int getnFilled() { return tableSpacesFilled; }
    int getnCollisions() { return nCollisions; }
    double getLoadFactor() { return static_cast<double>(tableSpacesFilled) / tableSize; }

    bool insert(const T&);
    bool remove(const T&);
    bool search(T&);
};

template <class T>
HashTable<T>::HashTable(long int tableSize, int bucketSize, hashFn hash, cmpFn cmp)
{
    this->tableSize = tableSize;
    this->bucketSize = bucketSize;
    this->tableSpacesFilled = 0;
    this->nCollisions = 0;
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

template <class T>
bool HashTable<T>::_checkDuplicate(const T &newData, long int hashVal)
{
    for (int i = 0; i < bucketSize; i++)
        if (isFilled[hashVal][i] && cmp(newData, table[hashVal][i]) == COMPARE_FN::EQUAL_TO)
            return true;

    return false;
}

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
                tableSpacesFilled--;

                if (!nFilled[hashVal])
                {
                    delete table[hashVal];
                    table[hashVal] = nullptr;
                }

                return true;
            }

    return false;
}

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

template <class T>
bool HashTable<T>::_resize(long int newSize)
{
    HashTable<T> *newHashTable = new HashTable<T>(newSize, bucketSize, _hash, cmp);

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

template <class T>
void HashTable<T>::_deleteTable(T **table, bool **isFilled, int *nFilled, Vector<T> *overflowArea, long int tableSize)
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