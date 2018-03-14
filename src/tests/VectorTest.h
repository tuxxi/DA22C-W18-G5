#ifdef _VECTOR_TEST
#pragma once

#include "../Vector.h"
#include <iostream>

void printVector(Vector<int> &v)
{
    for(int i = 0; i < v.size(); ++i)
    {
        std::cout << "idx[" << i << "]: " << v[i] << std::endl;
    }
}
void doVectorTests()
{
    std::srand(unsigned(time(nullptr))); //seed rand(), this is unnecessary but it makes the test look cooler

    Vector<int> v;
    size_t arraySize = 8;
    //fill up vector
    for (int i = 0; i < arraySize; ++i)
    {
        int random = std::rand() % 100;
        v.add(random);
        std::cout << "\nadding new element at idx " << i << ": " << v[i];
        std::cout << ". count is:" << v.size() << std::endl;
        printVector(v);
    }

    //test adding past the limit
    std::cout << "\n\nsize is now at the limit of array size. vector should resize automatically.\n";
    size_t newSize = 2 * arraySize;
    //fill up vector
    for (int i = arraySize; i < newSize; ++i)
    {
        int random = std::rand() % 100;
        v.add(random);
        std::cout << "\nadding new element at idx " << i << ": " << v[i];
        std::cout << ". count is:" << v.size() << std::endl;
        printVector(v);
    }

    //remove elements
    while(!v.empty())
    {
        int random = std::rand() % int(newSize - 1);

        if (random < v.size())
        {
            v.remove(random);
            std::cout << "removing idx " << random;
            std::cout << ". count is:" << v.size() << std::endl;
            printVector(v);
        }
    }
}

#endif