#ifdef _TESTING
#include "BSTTest.h"
#include "HashTableTest.h"
int main()
{
#ifdef _BST_TEST
    doBSTTests();
#elif _HASHTABLE_TEST
    doHashTest();
#endif
    return 0;
}
#endif