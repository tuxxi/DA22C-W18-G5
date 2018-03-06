#ifndef TEAM_PROJECT_OLYMPIANDATABASE_H
#define TEAM_PROJECT_OLYMPIANDATABASE_H

#include "Olympian.h"
#include "HashTable.h"
#include "BinarySearchTree.h"

class OlympianDatabase {
private:
    long (*hash)(const Olympian*, long tableSize);
    int (*cmpName)(const Olympian*, const Olympian*);
    int (*cmpAge)(const Olympian*, const Olympian*);
    int (*cmpHeight)(const Olympian*, const Olympian*);

    int nRecords;
    HashTable<Olympian> *hashTable;
    BinarySearchTree<Olympian> *bstAge;
    BinarySearchTree<Olympian> *bstHeight;

    void buildFromFile(std::string);
    const Olympian *readRecord(FILE*);
    bool _insert(const Olympian*);
public:
    OlympianDatabase(int, int, std::string);

    ~OlympianDatabase();

    bool remove(std::string);
    bool insert(const Olympian*);
    const Olympian* searchByName(std::string);
    const Olympian* searchByAge(int);
    const Olympian* searchByHeight(int);
};

#endif //TEAM_PROJECT_OLYMPIANDATABASE_H
