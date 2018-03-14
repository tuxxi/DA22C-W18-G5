#ifdef _TESTING
#include <iostream>
#include "BSTTest.h"
#include "HashTableTest.h"
#include "VectorTest.h"

int main()
{
#ifdef _BST_TEST
    doBSTTests();
#elif _HASHTABLE_TEST
    doHashTest();
#elif _VECTOR_TEST
    doVectorTests();
#endif
    return 0;

}
#endif