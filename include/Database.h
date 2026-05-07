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
    unordered_map<string, Table*> tables;

    void initialize() const;
    bool execCreateTable(const Query& query) const;
    string getHeadersDirPath() const;
    string getTablesDirPath() const;
    string getHeaderFilePath(const string& tableName) const;
    string getTableFilePath(const string& tableName) const;
    void loadTablesFromHeaders();
    void addTable(Table *table);
public:
    explicit Database(const string& filePath);
    ~Database();
    Table* getTable(const string &tableName);
    string getDbName();
    bool exec(const string& input) const;
};

#endif