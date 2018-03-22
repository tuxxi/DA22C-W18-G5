#ifndef OLYMPIAN_DATABASE_H
#define OLYMPIAN_DATABASE_H

#include <iostream>
#include <string>

#include "Olympian.h"
#include "BinarySearchTree.h"
#include "Stack.h"
#include "Vector.h"
#include "LinkedList.h"
#include "HashTable.h"
#include "Util.h"

class OlympianDatabase {

protected:
    int nRecords;

    HashTable<Olympian*> *hashTable;
    BinarySearchTree<Olympian*> *ageBst;
    BinarySearchTree<Olympian*> *heightBst;
    LinkedList<Olympian*> *alphabeticalOrderList;

    Stack<Olympian*> *deletionStack;
    Vector<Olympian*> *allRecords;

    bool _buildDatabase(std::ifstream&);
    Olympian *_readRecord(std::ifstream&);

    static long _hashFunc(Olympian *const &olympian, long int size);

    static COMPARE_FN _cmpName(Olympian *const &, Olympian *const &);
    static COMPARE_FN _cmpAge(Olympian *const &, Olympian *const &);
    static COMPARE_FN _cmpHeight(Olympian *const &, Olympian *const &);

    static void _printRecord(Olympian *const &, std::ostream &);

    static void _printAgeIndented(Olympian *const &, const int);

    static void _printHeightIndented(Olympian *const &, const int);

public:
    explicit OlympianDatabase(std::ifstream&);
    ~OlympianDatabase();

    int getnRecords() const { return nRecords; }

    bool insert(Olympian*);
    bool remove(std::string);
    bool undoDelete();
    Olympian *searchByName(std::string);
    bool searchByAge(int, Vector<Olympian*>&);
    bool searchByHeight(int, Vector<Olympian*>&);

    void displayAgeInOrder();
    void displayHeightInOrder();
    void displayAll();
    void displayAlphabeticalOrder();
    void displayAgeTree();
    void displayHeightTree();
    void displayHashStats();
    bool saveDatabase(std::string);
};


#endif // OLYMPIAN_DATABASE_H