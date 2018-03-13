#ifndef TEAM_PROJECT_OLYMPIANDATABASE_H
#define TEAM_PROJECT_OLYMPIANDATABASE_H

#include "Olympian.h"
#include "HashTable.h"
#include "BinarySearchTree.h"

class OlympianDatabase {
private:
    void buildFromFile(std::string);
    const Olympian *readRecord(std::ifstream&);
    bool _insert(const Olympian *newRecord);
    bool _remove(std::string);
    static COMPARE_FN cmpName(const Olympian&, const Olympian&);
    static COMPARE_FN cmpAge(const Olympian&, const Olympian&);
    static COMPARE_FN cmpHeight(const Olympian&, const Olympian&);

    int nRecords;
    HashTable<Olympian /*, std::string*/> *hashTable;
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
