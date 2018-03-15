cmake_minimum_required(VERSION 3.9)
project(DA22C_W18_G5)

set(CMAKE_CXX_STANDARD 11)

#these definitions can be use to enable or disable testing various data structures
#add_definitions(-D_TESTING)    #testing everything, enable this for testing in general to be compiled

#individual data structures tests
#add_definitions(-D_VECTOR_TEST)
#add_definitions(-D_HASHTABLE_TEST)
#add_definitions(-D_BST_TEST)

add_executable(DA22C_W18_G5
        src/HashTable.h
        src/LinkedList.h
        src/BinarySearchTree.h
        src/Vector.h

        src/Olympian.h
        src/OlympianDatabase.h src/OlympianDatabase.cpp
        src/tests/BSTTest.h src/tests/VectorTest.h src/tests/HashTableTest.h
        src/tests/Tests.cpp)
