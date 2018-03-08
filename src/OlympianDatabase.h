#ifndef TEAM_PROJECT_OLYMPIANDATABASE_H
#define TEAM_PROJECT_OLYMPIANDATABASE_H

#include "Olympian.h"
#include "HashTable.h"
#include "BinarySearchTree.h"

class OlympianDatabase {
private:
    void buildFromFile(std::string);
    const Olympian *readRecord(ifstream&);
    bool _insert(Olympian *newRecord);
    int cmpName(const Olympian*, const Olympian*);
    int cmpAge(const Olympian*, const Olympian*);
    int cmpHeight(const Olympian*, const Olympian*);

    int nRecords;
    HashTable<Olympian, string> *hashTable;
    BinarySearchTree<Olympian> *bstAge;
    BinarySearchTree<Olympian> *bstHeight;

public:
    OlympianDatabase(int, int, std::string);

    ~OlympianDatabase();

    bool remove(std::string);
    const Olympian *searchByName(std::string);
    const Olympian *searchByAge(int);
    const Olympian *searchByHeight(int);
};

#endif //TEAM_PROJECT_OLYMPIANDATABASE_H
