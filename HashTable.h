#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "LinkedList.h"

#define MAX_COLLISIONS 15

template <class T>
class HashTable
{
private:
    int tableSize;
    int bucketSize;
    int nFilled;
    int nCollisions;
    T **table;
    bool **isFilled;
    LinkedList<T> *overflowArea;

    long int (*_hash)(const T&, long int);
    int (*cmp)(const T&, const T&);

    bool _insert(const T&);
    bool _insertOverflow(const T&);
    bool _checkDuplicate(const T&, long int);
    bool _remove(const T&);
    bool _search(T&);
    bool _resize();
    void _deleteTable(T**, bool**, long int);

public:
    HashTable(long int, int, long int (*)(const T&, long int), int (*)(const T&, const T&));

    ~HashTable();

    long int getTableSize() { return tableSize; }
    int getnFilled() { return nFilled; }
    int getnCollisions() { return nCollisions; }
    double getLoadFactor() { return nFilled / tableSize; }

    bool insert(const T&);
    bool remove(const T&);
    bool search(T&);
};

template <class T>
HashTable<T>::HashTable(long int tableSize, int bucketSize,
                        long int (*hash)(const T&, long int), int (*cmp)(const T&, const T&))
{
    this->tableSize = tableSize;
    this->bucketSize = bucketSize;
    this->nFilled = 0;
    this->nCollisions = 0;
    this->table = new T*[tableSize] {nullptr};
    this->isFilled = new bool*[tableSize] {nullptr};
    this->overflowArea = new LinkedList<T>(cmp);
    this->_hash = hash;
    this->cmp = cmp;
}

template <class T>
bool HashTable<T>::insert(const T &newData)
{
    return _insert(newData);
}

template <class T>
bool HashTable<T>::remove(const T &removalKey)
{
    return (_remove(removalKey) || overflowArea->insert(removalKey));
}

template <class T>
bool HashTable<T>::search(T &searchObj)
{
    return (_search(searchObj) || overflowArea->search(searchObj));
}

template <class T>
bool HashTable<T>::_insert(const T &newData)
{
    long int hashVal = _hash(newData, tableSize);

    if (table[hashVal])
    {
        if (!_checkDuplicate(newData, hashVal))
        {
            for (int i = 0; i < bucketSize; i++)
                if (!isFilled[hashVal][i])
                {
                    table[hashVal][i] = newData;
                    isFilled[hashVal][i] = true;
                    nCollisions++;

                    return true;
                }

            if (!_insertOverflow(newData))
                return false;
        }
    }
    else
    {
        table[hashVal] = new T[bucketSize] {T()};
        isFilled[hashVal] = new bool[bucketSize] {false};
        table[hashVal][0] = newData;
        isFilled[hashVal][0] = true;

        nFilled++;

        return true;
    }

    return false;
}

template <class T>
bool HashTable<T>::_insertOverflow(const T &newData)
{
    if (overflowArea->insert(newData))
    {
        nCollisions++;

        /*if (overflowArea->getCount() >= MAX_COLLISIONS)
            _resize();*/

        return true;
    }

    return false;
}

template <class T>
bool HashTable<T>::_checkDuplicate(const T &newData, long int hashVal)
{
    for (int i = 0; i < bucketSize; i++)
        if (isFilled[hashVal][i] && !cmp(newData, table[hashVal][i]))
            return true;

    return false;
}

template <class T>
bool HashTable<T>::_remove(const T &removalKey)
{
    long int hashVal = _hash(removalKey, tableSize);

    if (table[hashVal])
        for (int i = 0; i < bucketSize; i++)
            if (isFilled[hashVal][i] && !cmp(table[hashVal][i], removalKey))
            {
                isFilled[hashVal][i] = false;
                table[hashVal][i] = T();

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
            if (isFilled[hashVal][i] && !cmp(searchObj, table[hashVal][i]))
            {
                searchObj = table[hashVal][i];

                return true;
            }

    return false;
}

template <class T>
bool HashTable<T>::_resize()
{
    T **oldTable = table, **ptr1;
    bool **oldIsFilled = isFilled, **ptr2;

    table = new T*[tableSize * 2];
    isFilled = new bool*[tableSize * 2];

    for (int i = 0; i < tableSize; i++)
        if (oldTable[i])
            for (int j = 0; j < bucketSize; j++)
                if (oldIsFilled[i][j])
                    if (!_insert(oldTable[i][j]) && !_insertOverflow(oldTable[i][j]))
                    {
                        std::cerr << "\n[ERROR]: There was a problem resizing the hash table.\n\n";

                        _deleteTable(table, isFilled, tableSize * 2);

                        table = oldTable;
                        isFilled = oldIsFilled;

                        return false;
                    }

    _deleteTable(oldTable, oldIsFilled, tableSize);
    tableSize *= 2;

    return true;
}

template <class T>
void HashTable<T>::_deleteTable(T **table, bool **isFilled, long int tableSize)
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
}

template <class T>
HashTable<T>::~HashTable()
{
    std::cout << overflowArea->getCount() << std::endl;
    T **ptr1 = table;
    bool **ptr2 = isFilled;

    _deleteTable(table, isFilled, tableSize);
}

#endif // HASH_TABLE_H