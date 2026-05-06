#ifndef QUERY_H
#define QUERY_H
#include <vector>
#include <string>

#include "Column.h"
#include "QueryType.h"
using namespace std;

class Query {
public:
    QueryType type;
    string tablesStr;
    string columnsStr;
    string valuesStr;
    string conditionsStr;

    vector<Column> parsedColumns;
    vector<string> targetColumns;
    void printDetails() const;
};

#endif
