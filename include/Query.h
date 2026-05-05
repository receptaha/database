#ifndef QUERY_H
#define QUERY_H
#include <vector>
#include <string>
#include "QueryType.h"
using namespace std;

class Query {
public:
    QueryType type;
    string tablesStr;
    string columnsStr;
    string valuesStr;
    string conditionsStr;

    void printDetails() const;
};

#endif
