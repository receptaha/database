#ifndef TABLE_H
#define TABLE_H
#include <string>
#include "Column.h"
#include "Pager.h"
using namespace std;

class Table {
private:
    string name;
    unordered_map<string, Column*> columns;
    uint32_t rowSize;
    uint32_t numRows;
    Pager* pager;
    bool isColumnExists(const string& columnName) const;

public:
    Table(const string& name, Pager* p);
    void addColumn(const string& columnName, DataType columnType);

    void printSchema() const;
};


#endif //TABLE_H
