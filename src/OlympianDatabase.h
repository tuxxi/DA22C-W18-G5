#ifndef OLYMPIAN_DATABASE_H
#define OLYMPIAN_DATABASE_H

#include <string>

#include "Olympian.h"
#include "BinarySearchTree.h"
#include "Stack.h"
#include "Vector.h"
#include "CompareFunction.h"
#include "HashTable.h"

class OlympianDatabase {
private:
    int nRecords;

    HashTable<Olympian*> *hashTable;
    BinarySearchTree<Olympian*> *ageBst;
    BinarySearchTree<Olympian*> *heightBst;
    LinkedList<Olympian*> *alphabeticalOrderList;

    Stack<Olympian*> *deletionStack;
    Vector<Olympian*> *allRecords;

    bool _buildDatabase(std::ifstream&);
    bool _readRecord(std::ifstream &, size_t);
    static long hashFunc(Olympian *const &olympian, long int size);

    static COMPARE_FN cmpName(Olympian *const &, Olympian *const &);
    static int int_cmpName(Olympian *const &, Olympian *const &);
    static COMPARE_FN cmpAge(Olympian *const &, Olympian *const &);
    static COMPARE_FN cmpHeight(Olympian *const &, Olympian *const &);
    static void printOlympian(Olympian*&);

public:
    explicit OlympianDatabase(std::ifstream&);
    ~OlympianDatabase();

    int getnRecords() { return nRecords; }

    bool insert(Olympian*);
    bool remove(std::string);
    bool undoDelete();
    Olympian *searchByName(std::string);
    bool searchByAge(int, Vector<Olympian*>&);
    bool searchByHeight(int, Vector<Olympian*>&);

    Vector<Olympian*> getYoungest();
    Vector<Olympian*> getOldest();
    Vector<Olympian*> getShortest();
    Vector<Olympian*> getTallest();

    void displayAgeInOrder();
    void displayHeightInOrder();
    void displayAll();
    void displayAlphabeticalOrder();
    void displayAgeTree();
    void displayHeightTree();
    void displayHashStats();
    void saveDatabase(std::ofstream&);
};


#endif // OLYMPIAN_DATABASE_H