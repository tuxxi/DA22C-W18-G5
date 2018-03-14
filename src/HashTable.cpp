#include <iostream>
#include <string>
#include "HashTable.h"

using namespace std;

struct testStruct {
    string name;
    int num;
};

int cmp(const testStruct *t1, const string &s)
{
    if (t1->name == s)
        return 0;
    else if (t1->name < s)
        return -1;

    return 1;
}

long hashFunc(const testStruct *t1, int size)
{
    long sum = 0;

    for (int i = 0; i < t1->name.length(); i++)
        sum += t1->name[i] * t1->name[i] * t1->name[i];

    return sum % size;
}

int main ()
{
    HashTable <testStruct, string>hashTable(100, 3, hashFunc, cmp);

    testStruct t1 = {"name", 1};

    hashTable.insert(&t1);

    return 0;
}
