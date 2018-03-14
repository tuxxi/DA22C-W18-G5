//
//  Hash.h
//  two templates
//
//  Created by YF on 07/03/2018.
//  Copyright © 2018 YF. All rights reserved.
//

#ifndef Hash_h
#define Hash_h
#include <iostream>
#include <string>
#include "LinkList.h"
#include "Stack.h"
const int SIZE = 10;
const int size_bucket = 3;
template<class T, class K>
class Hash
{
private:
    struct Bucket
    {
        T const *data[size_bucket];
        bool empty[size_bucket];
        int count;
    };
   
    // these are two function pointers
    int (*cmp)(const T*, const K&);
    long (*hash)(const K&, const int);
    
    //a linklist pointer
    LinkedList<T, K> *list;
    
    //dymically allocate memory for bucket
    Bucket *bucket;
    int resize = SIZE;
    double load_factor = 0;
    int bucket_used = 0;
    
    /*
     the functions below are called by the public functions 
     cannot be accessed by other classes's member functions
    */
    bool _insertion(const T&, const int address);
    bool _find(const long address, T &dataOut, const K key);
    bool _remove(const long address, T &dataOut, const K key);
public:
    /*
     public functions
     */
    Hash( int (*cmp)(const T*, const K&), long (*hash)(const K&, const int));
    ~Hash();
    void insert(const K&key, const T&data);
    bool findEntry(const K& key, T &dataOut);
    bool remove(const K&, T &dataOut);
    
};

/*
 dynamical allocation for each pointer in the buckets.
 set empty to true, which indicates that this spot has not been taken
 it takes two function pointers, the first one is comparing function (compare the key and the object)
 the second funciton is the hasing function
 */
template<class T,class K>
Hash<T, K>::Hash(int (*cmp)(const T*, const K&), long (*hash)(const K&, const int))
{
     this->cmp = cmp;
     this->hash = hash;
     list = new LinkedList<T, K>(this->cmp);
    
    bucket = new Bucket[SIZE];
      for(int i=0; i<SIZE; i++)
    {
        bucket[i].count = 0;
     
        for(int j=0; j<size_bucket; j++)
        {
              bucket[i].data[j] = new T;
              bucket[i].data[j] = nullptr;
              bucket[i].empty[j] = true;
        }
    }
}

/*
 the first parameter the the key which can get the index of the bucket
 */

template<class T,class K>
void Hash<T, K>::insert(const K& key, const T& data)
{
    long address = hash(key, SIZE);
    
    bool success;
    success = _insertion(data, (int)address);
    
   if(!success)
       list->insert_overflow(&data);
  
}

/*takes the key and find the object
 return true if the item is found
 */
template<class T,class K>
bool Hash<T, K>::findEntry(const K& key, T &dataOut)
{
    long address = hash(key, SIZE);
    bool success = _find(address, dataOut, key);
    
    
    if(!success)
        success = list->search_overflow(key, dataOut);
    
    return success;
    
}

/*
 try to remove the object from the hashtable
 return true if the deletion is successful
 */
template<class T,class K>
bool Hash<T, K>::remove(const K& key, T &dataOut)
{
    long address = hash(key, SIZE);
    
    bool success;
    success = _remove(address, dataOut, key);
    
    if(!success)
        success = list->remove_overflow(key, dataOut);

    return success;
}


/*
 this is a private function, which is accessed by insert function 
 add a new item in the hashtable if that bucket still has vacant place
 */
template<class T,class K>
bool Hash<T, K>::_insertion(const T& data, const int address)
{

    for(int i=0; i<size_bucket; i++)
    {
        if(bucket[address].empty[i] == true)
        {
            bucket[address].empty[i] = false;
            bucket[address].data[i] = &data;
            bucket[address].count ++;
            return true;
        }
    }
    
    return false;
}

/*
 this is a private function, which is accessed by delete function 
 remove the item if the item is found in the hashtable
 */
template<class T,class K>
bool Hash<T, K>::_remove(const long address, T &dataOut, const K key)
{
    for(int i=0; i<size_bucket; i++)
    {
        if(bucket[address].data[i] != nullptr)
        {
            if(cmp(bucket[address].data[i], key) == 1)
            {
                dataOut = *bucket[address].data[i];
                bucket[address].data[i] = nullptr;
                bucket[address].count --;
                return true;
            }
        }
        
    }
    
    return false;
}

/*
 this is a private function, which is accessed by _find function 
 return true if the item is found in the hashtable
 */

template<class T,class K>
bool Hash<T, K>::_find(const long address, T &dataOut, const K key)
{

    for(int i=0; i<size_bucket; i++)
    {
       if(bucket[address].data[i] != nullptr)
       {
        if(cmp(bucket[address].data[i], key) == 1)
        {
            dataOut = *bucket[address].data[i];
            return true;
        }
        }
    }
    
    return false;
}

/*
 release all the memory has been dymically allocated
 */
template<class T,class K>
Hash<T, K>::~Hash()
{
    for(int i=0; i<SIZE; i++)
    {
        for(int j=0; j<size_bucket; j++)
            delete bucket[i].data[j];
    }
    delete bucket;
    
}

#endif /* Hash_h */
