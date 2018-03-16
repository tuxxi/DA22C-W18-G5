#pragma once
#ifdef _VECTOR_TEST


#include "../Vector.h"
#include <iostream>
#include <ctime>
#include <string>

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

    //test a vector of pointers
    Vector<int*> vecPointers;
    int a = 5, b = 7, c = 10;
    vecPointers.add(&a);
    vecPointers.add(&b);
    vecPointers.add(&c);

    for (int i = 0; i < 3; ++i)
    {
        std::cout << "vecPointers contains " << vecPointers[i] << " , with a value of " << *vecPointers[i] << std::endl;
    }

    //change the values
    a = 3;
    b = 4421;
    c = -6;

    //they should now be updated
    for (int i = 0; i < 3; ++i)
    {
        std::cout << "vecPointers contains " << vecPointers[i] << " , with a value of " << *vecPointers[i] << std::endl;
    }
    c = 5;
}

#endif