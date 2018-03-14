#ifdef _VECTOR_TEST
#pragma once

#include "../Vector.h"
#include "iostream"

void printVector(Vector<int> &v)
{
    for(int i = 0; i < v.count(); ++i)
    {
        std::cout << "idx[" << i << "]: " << v[i] << std::endl;
    }
}
void doVectorTests()
{
    Vector<int> v;
    size_t arraySize = 7;
    //fill up vector
    for (int i = 0; i < arraySize; ++i)
    {
        int random = std::rand() % arraySize - 1;
        v.add(random);
        std::cout << "\nadding new element at idx " << i << ": " << v[i];
        std::cout << ". count is:" << v.count() << std::endl;
        printVector(v);
    }

    //test adding past the limit
    std::cout << "\n\nsize is now at the limit of array size. vector should resize automatically.\n";
    int newSize = 2 * arraySize;
    //fill up vector
    for (int i = 7; i < newSize; ++i)
    {
        int random = std::rand() % newSize - 1;
        v.add(random);
        std::cout << "\nadding new element at idx " << i << ": " << v[i];
        std::cout << ". count is:" << v.count() << std::endl;
        printVector(v);
    }


    //remove elements
    while(!v.empty())
    {
        int random = std::rand() % arraySize - 1;

        if (random < v.count())
        {
            std::cout << "removing idx " << random;
            v.remove(random);
            std::cout << ". count is:" << v.count() << std::endl;
            printVector(v);
        }
    }
}

#endif