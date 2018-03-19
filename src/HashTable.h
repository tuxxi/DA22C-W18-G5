//
//  HashTable.h
//
//  Created by YF on 07/03/2018.
//  Copyright © 2018 YF. All rights reserved.
//

#ifndef Hash_h
#define Hash_h

#include <iostream>
#include <string>
#include <iomanip>
#include "LinkedList.h"
#include "Stack.h"
#include "CompareFunction.h"

template<class T>
class HashTable
{
private:
    struct Bucket
    {
        T const **data;

        bool *empty;
        int count;
    };

    struct Stats
    {
        int curr_size,
            stats_collision;
        double LoadFactor;
        //  friend ostream& operator << (ostream& os, const Stats& rhs);
    };

    // these are two function pointers
    COMPARE_FN(*cmp)(const T&, const T&);
    long(*hash)(const T, const int);

    //a linklist pointer
    LinkedList<T> *list;
    Stack<T*> *stack;
    Stack<Stats> *s_stats;

    //dymically allocate memory for bucket
    Bucket *bucket;
    int total_object;
    int resize;
    int num_rehash;
    int SIZE;
    int size_bucket;
    double load_factor;
    int bucket_used;
    int collision;

    /*
    the functions below are called by the public functions
    cannot be accessed by other classes's member functions
    */
    bool _insertion(T&, const int address);
    bool _find(const long address, T &dataOut);
    bool _remove(const long address, T &dataOut);
public:
    /*
    public functions
    */
    HashTable(COMPARE_FN(*cmp)(const T&, const T&), long(*hash)(const T, const int), int SIZE, int size_bucket);

    ~HashTable();
    bool insert(T data);
    bool findEntry(T &dataOut);
    bool remove(T &dataOut);
    bool rehash();
    void printStats();

};


/*
dynamical allocation for each pointer in the buckets.
set empty to true, which indicates that this spot has not been taken
it takes two function pointers, the first one is comparing function (compare the key and the object)
the second funciton is the hasing function
*/
template<class T>
HashTable<T>::HashTable(COMPARE_FN(*cmp)(const T&, const T&), long(*hash)(const T, const int), int SIZE, int size_bucket)
{
    this->SIZE = SIZE;        //initial size
    this->size_bucket = size_bucket;
    this->resize = SIZE;
    this->total_object = 0;
    this->bucket_used = 0;
    this->load_factor = 0;
    this->num_rehash = 0;
    this->collision = 0;

    this->cmp = cmp;
    this->hash = hash;
    list = new LinkedList<T>(this->cmp);
    stack = new Stack<T*>;
    s_stats = new Stack<Stats>;

    bucket = new Bucket[resize];

    for (int i = 0; i<resize; i++)
    {
        bucket[i].count = 0;
        bucket[i].data = new const T*[size_bucket];
        bucket[i].empty = new bool[size_bucket];

        for (int j = 0; j<size_bucket; j++)
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

template<class T>
bool HashTable<T>::insert(T data)
{
    T *hold = nullptr;
    hold = &data;
    this->total_object++;

    long address = hash(data, this->resize);

    // cout<<address<<endl;

    stack->push(hold);

    bool success;
    success = _insertion(data, (int)address);

    if (!success)
        list->insert_overflow(data);

    load_factor = double(bucket_used) / resize;

    //   std::cout<<load_factor<<" "<<bucket_used<<" "<<resize<<" "<<std::endl;

    if (load_factor >= 0.6)
    {
        this->collision = this->total_object - this->bucket_used;
        this->num_rehash++;

        Stats stats;
        stats.stats_collision = this->collision;
        stats.curr_size = this->resize;
        stats.LoadFactor = this->load_factor;

        s_stats->push(stats);

        rehash();


    }


    return success;
}

/*takes the key and find the object
return true if the item is found
*/
template<class T>
bool HashTable<T>::findEntry(T &dataOut)
{
    long address = hash(dataOut, resize);
    bool success = _find(address, dataOut);

    if (!success)
        success = list->search_overflow(dataOut);

    return success;
}

/*
try to remove the object from the hashtable
return true if the deletion is successful
*/
template<class T>
bool HashTable<T>::remove(T &dataOut)
{
    long address = hash(dataOut, resize);

    bool success;
    success = _remove(address, dataOut);

    if (!success)
        success = list->remove_overflow(dataOut);

    return success;
}


/*
this is a private function, which is accessed by insert function
add a new item in the hashtable if that bucket still has vacant place
*/
template<class T>
bool HashTable<T>::_insertion(T &data, const int address)
{
    for (int i = 0; i<size_bucket; i++)
    {
        if (bucket[address].empty[i] == true)
        {
            //  std::cout<< i <<"    ";       //debug
            bucket[address].empty[i] = false;
            bucket[address].data[i] = &data;

            if (bucket[address].data[i] != nullptr)      // debug
                                                         // std::cout<<"good!\n";

                if (i == 0)
                    bucket_used++;

            bucket[address].count++;
            //   cout<<*bucket[address].data[i]<<endl;
            return true;
        }
    }
    return false;
}

/*
this is a private function, which is accessed by delete function
remove the item if the item is found in the hashtable
*/
template<class T>
bool HashTable<T>::_remove(const long address, T &dataOut)
{

    for (int i = 0; i<size_bucket; i++)
    {
        if (bucket[address].empty[i] == false)
        {
            // cout<<"_remove"<<**bucket[address].data[0]<<endl;
            //   std::cout<<dataOut<<std::endl;       //debug

            if (cmp(*bucket[address].data[i], dataOut) == COMPARE_FN::EQUAL_TO)
            {
                //  cout<<key<<endl;       //debug
                dataOut = *bucket[address].data[i];
                bucket[address].empty[i] = true;
                bucket[address].data[i] = nullptr;
                bucket[address].count--;
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

template<class T>
bool HashTable<T>::_find(const long address, T &dataOut)
{
    // cout<<"_find"<<address<<endl;
    for (int i = 0; i<size_bucket; i++)
    {
        if (bucket[address].empty[i] == false)
        {
            if (cmp(*bucket[address].data[i], dataOut) == COMPARE_FN::EQUAL_TO)
            {
                // std::cout<<"found!\n";
                dataOut = *bucket[address].data[i];
                return true;
            }
        }
    }

    return false;
}


template<class T>
bool HashTable<T>::rehash()
{
    /*
    int index = 0;
    T* In = new T[stack->getCount()];

    while (!stack->isEmpty())
    {
        //In[index] = nullptr;
        stack->pop(In[index]);
        index++;
    }

    for (int i = 0; i<resize; i++)
    {
        delete[] bucket[i].data;
    }

    delete[] bucket;

    resize *= 3;

    bucket = new Bucket[resize];
    this->bucket_used = 0;
    this->load_factor = 0;

    for (int i = 0; i<resize; i++)
    {
        bucket[i].count = 0;
        bucket[i].data = new const T*[size_bucket];
        bucket[i].empty = new bool[size_bucket];

        for (int j = 0; j<size_bucket; j++)
        {
            bucket[i].data[j] = new T;
            bucket[i].data[j] = nullptr;
            bucket[i].empty[j] = true;
        }
    }

    for (int i = 0; i<index; i++)
        insert(**In[i]);

    delete[] In;
    return true;
    */
    return false;
}


template<class T>
void HashTable<T>::printStats()
{
    Stats dataOut;

    cout << left << setw(15) << "Current_SIZE" << left << setw(15) << "Collisions" << left << setw(15)
        << "Load_factor" << endl;
    while (!s_stats->isEmpty())
    {
        s_stats->pop(dataOut);
        cout << left << setw(15) << dataOut.curr_size << left << setw(15) << dataOut.stats_collision << left << setw(15)
            << dataOut.LoadFactor << endl;
    }
}

/*
release all the memory has been dymically allocated
*/

template<class T>
HashTable<T>::~HashTable()
{
    for (int i = 0; i<SIZE; i++)
    {
        delete[] bucket[i].data;
    }
    delete[] bucket;

}


#endif /* Hash_h */
