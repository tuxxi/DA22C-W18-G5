//
//  main.cpp
//  two templates
//
//  Created by YF on 06/03/2018.
//  Copyright © 2018 YF. All rights reserved.
//
#ifdef _HASHTABLE_TEST
#pragma once

#include <iostream>
#include <string>
#include <cmath>

#include "../HashTable.h"

using namespace std;
class StuInfo
{
private:
    int id;
    string name;
public:
    StuInfo(int id = 0, string name = ""){this->id = id; this->name = name;}
    string get_name() const {return this->name;}
    int get_id() const {return this->id;}
    string get_key() const {return this->name;}
    friend ostream& operator<<(ostream& os, const StuInfo& stu);

};

ostream& operator<<(ostream& os, const StuInfo& stu)
{
    os<<stu.name<<" "<<stu.id<<endl;
    return os;
}

long hash_string(const string &key, const int size)
{
    int sum = 0;
    for(int i=0; i<key.size(); i++)
        sum += key[i]*key[i]*key[i];

    return sum % size;

}



int compareName(const StuInfo* student, const string &key)
{
    if(key == student->get_key())
        return 1;

    return 0;
}


void doHashTest()
{
    string name[7] = {"Jeff", "eJff", "ffJe", "Jffe", "Elias", "Aidan", "AJ"};
    int id[7] = {20278379, 2341123, 9873423, 2342123, 3438923, 24333425, 29389833};
    HashTable<StuInfo, string> hash(compareName, hash_string);

    StuInfo *student[7];


    for(int i=0; i<7; i++)
        student[i] = new StuInfo(id[i], name[i]);

    for(int i=0; i<7; i++)
    {
        hash.insert(student[i]->get_key(), *student[i]);

    }

    //test
    cout<<"please enter the name you wanna search\n";
    string object;
    cin>>object;

    StuInfo outData;

    bool success = hash.findEntry(object, outData);

    if(success)
        cout<<outData;

    cout<<"please enter the name you wanna delete\n";
    cin>>object;
    success = hash.remove(object, outData);

    if(success)
    {
        cout<<outData;
        cout<<"the removal was successful\n";
    }

    cout<<"please enter the name you wanna search\n";
    cin>>object;


    success = hash.findEntry(object, outData);

    if(success)
        cout<<outData;


}
#endif