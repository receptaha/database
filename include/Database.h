#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <fstream>
#include <string>

#include "Query.h"
#include "Table.h"

using namespace std;

class Database {
private:
    string filePath;
    Pager* pager;
    unordered_map<string, Table*> tables;

    void initialize() const;
    bool execCreateTable(const Query& query) const;
    string getHeadersDirPath() const;
    string getTablesDirPath() const;
public:
    explicit Database(const string& filePath);
    ~Database();
    void addTable(const string &tableName);
    Table* getTable(const string &tableName);
    string getDbName();
    bool exec(const string& input) const;
};

#endif