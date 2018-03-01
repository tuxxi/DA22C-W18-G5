//
//  HashTable.h
//  Hasing_table
//
//  Created by YF on 22/02/2018.
//  Copyright © 2018 YF. All rights reserved.
//

#ifndef HASHTABLE_H
#define HASHTABLE_H

static const int g_kSIZE = 1000; //todo: remove this?

template<class T>
class HashTable
{
public:
    HashTable();
    HashTable(int size); //array size?

    ~HashTable();

    bool insert(int key, const T& data);
    int getAddress(int key) const;
    T* findEntry(const int key);
private:
    bool _insertion(const int, const int, const T&);
    int _find(const int address, const int key);

    struct Bucket
    {
        T data;
        int key = -1;
    };

    int (*hash_function)(int);

    Bucket** bucket; //allocate dynamically so we can set size at runtime
    int size;
    const int bucket_size = 3;
};

template<class T>
HashTable<T>::HashTable()
	: HashTable(g_kSIZE) //call HashTable(int)
{}

template<class T>
HashTable<T>::HashTable(int size)
	: size(size)
{
	bucket = new Bucket[size][bucket_size];
}

template<class T>
HashTable<T>::~HashTable()
{
	delete[] bucket;
}
template<class T>
bool HashTable<T>::insert(int key, const T&data)
{
    const int address = getAddress(key);
    return _insertion(address, key, data);
   // cout<<bucket[300][-1].key<<endl;
}

template<class T>
int HashTable<T>::getAddress(int key) const
{
    return hash_function(key);
}

template<class T>
bool HashTable<T>::_insertion(const int address, const int key, const T& data)
{
    for(int i=0; i < bucket_size; ++i)
    {
        if(this->bucket[address][i].key == -1)
        {
            this->bucket[address][i].key = key;
            this->bucket[address][i].data = data;
            return true;
        }
            
    }
    return false;
}

template<class T>
T* HashTable<T>::findEntry(const int key)
{
    const int address = getAddress(key);
    const int index = _find(address, key);
    
    if (index >= 0)
    {
        return bucket[address][index].data;
    }
    return nullptr;
}

template<class T>
int HashTable<T>::_find(const int address, const int key)
{
    for(int i=0; i < bucket_size; ++i)
    {
        if (key == bucket[address][i].key)
        {
            return i;
        }
    }
    return -1;
}

#endif /* HashTable.h */
