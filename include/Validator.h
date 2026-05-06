#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "Query.h"
#include <iostream>

using namespace std;

class Validator {
public:
    static bool validateAndSetQuery(Query& query);

private:
    static bool validateInsert(Query& query);
    static bool validateSelect(Query& query);
    static bool validateCreateTable(Query& query);
    static bool validateUpdate(Query& query);
    static bool validateDelete(Query& query);
};

#endif