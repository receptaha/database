#ifndef PARSER_H
#define PARSER_H

#include <string>

#include "Query.h"


class Parser {
private:
    static QueryType identifyQueryType(const std::string &input);
    static bool parseInsert(const std::string &input, Query &query);
    static bool parseSelect(const std::string &input, Query &query);
    static bool parseCreateTable(const string &input, Query &query);
    static bool parseUpdate(const string &input, Query &query);

public:

    static bool parseAndSetQuery(const std::string &input, Query& query);
};



#endif
