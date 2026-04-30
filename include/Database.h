#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <fstream>
#include <string>

#include "Table.h"

using namespace std;

class Database {
private:
    string filePath;
    Pager* pager;
    unordered_map<string, Table*> tables;

public:
    explicit Database(const string& filePath);
    ~Database();

    void addTable(const string &tableName);

    Table *getTable(const string &tableName);
};

#endif