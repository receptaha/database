#ifndef QUERY_H
#define QUERY_H
#include <vector>
#include <string>
#include "QueryType.h"
using namespace std;

class Query {
public:
    QueryType type;
    string targetName;
    vector<string> columns;
    vector<string> values;
    vector<string> conditions;

    void printDetails() const;
};

#endif
