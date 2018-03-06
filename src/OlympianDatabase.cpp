#include <iostream>
#include <fstream>
#include <cstring>
#include "OlympianDatabase.h"
#include "Olympian.h"

#define FILE_READ_ERROR(s) cout << "Error reading the input file " << s << ". Exiting\n", exit(1);
#define LINE_SIZE 300

using namespace std;

OlympianDatabase::OlympianDatabase(int hashTableSize, int bucketSize, string infileName)
{
    ;
}

void OlympianDatabase::buildFromFile(string infileName)
{
    /* I commented this out bc it doesn't compile on my machine
     * pls fix
    Olympian *newRecord;
    FILE *infile;

    if (!(infile = fopen(infileName)))
        FILE_READ_ERROR(infileName)

    fgets(input, LINE_SIZE, infile);

    while ((newRecord = readRecord()))
        insert(newRecord);
        */
}

const Olympian * OlympianDatabase::readRecord(FILE *infile)
{
    //this function gives me lots of warnings with -Wall -Wextra -Wpedantic
    //in addition to clang-tidy problems
    //perhaps we should rewrite?

    char input[LINE_SIZE], *firstName, *lastName, *fullName, *state, *sport, *medals,
            *bronze, *silver, *gold, *strPtr, *endPtr, gender;
    int age, height, nGold, nSilver, nBronze;
    Olympian *newRecord = nullptr;

    fgets(input, LINE_SIZE, infile);

    firstName = strtok(input, ",");

    if (firstName) {
        lastName = strtok(NULL, ",");
        gender = *strtok(NULL, ",");
        sport = strPtr = strtok(NULL, ",");
        strPtr += strlen(sport) + 1;


        if (*strPtr == '\"')
            strPtr = strchr(strPtr + 1, '\"') + 2;
        else
            strPtr = strchr(strPtr, ',') + 1;
        if (*strPtr == '\"')
            strPtr = strchr(strPtr + 1, '\"') + 2;
        else if (isdigit(*strPtr))
            strPtr = strchr(strPtr, ',') + 1;
        else
            strPtr = strchr(strPtr + 1, ',');

        nGold = strtol(strPtr, &endPtr, 10);
        nSilver = strtol(endPtr + 1, &endPtr, 10);
        nBronze = strtol(endPtr + 1, &endPtr, 10);
        strPtr = endPtr + 2;

        height = 12 * strtol(strPtr, &endPtr, 10) + strtol(endPtr + 1, &endPtr, 10);
        strPtr = endPtr + 4;

        for (int i = 0; i < 3; i++)
            strPtr = strchr(strPtr, ',') + 1;
        age = strtol(strPtr, &endPtr, 10);

        strPtr = strchr(endPtr + 1, ',') + 1;
        state = strtok(strPtr, ",");

        newRecord = new Olympian(strcat(lastName, strcat(", ", firstName)), sport, state, gender,
                                 age, height, nGold, nSilver, nBronze);
    }

    return newRecord;
}

bool OlympianDatabase::insert(const Olympian *)
{
    ;
}

OlympianDatabase::~OlympianDatabase()
{
    delete hashTable;
    delete bstAge;
    delete bstHeight;
}
