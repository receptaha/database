#ifndef TABLE_H
#define TABLE_H
#include <string>
#include "Column.h"
using namespace std;

class Table {
private:
    string name;
    vector<Column> columns;
    uint32_t rowSize;
    bool isColumnExists(const string& columnName) const;
public:
    Table(const string& name);
    void addColumn(const string& columnName, DataType columnType);

    void printSchema() const;
};


#endif //TABLE_H
