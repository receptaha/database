#ifndef TABLE_H
#define TABLE_H
#include <string>
#include "Column.h"
#include "Pager.h"
using namespace std;

class Table {
private:
    unordered_map<string, Column*> columns;
    Pager* pager;
    bool isColumnExists(const string& columnName) const;

public:
    string name;
    uint32_t rowSize;
    uint64_t numRows;
    Table(const string& name, Pager* p);
    void addColumn(Column* column);
    void printSchema() const;
};


#endif //TABLE_H
