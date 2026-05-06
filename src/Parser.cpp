#include "../include/Parser.h"

#include <sstream>
#include <regex>

#include "../include/Query.h"

QueryType Parser::identifyQueryType(const std::string &input) {
    if (input.starts_with("CREATE TABLE")) return QueryType::CREATE_TABLE;
    if (input.starts_with("SELECT")) return QueryType::SELECT;
    if (input.starts_with("INSERT INTO")) return QueryType::INSERT_INTO;
    if (input.starts_with("UPDATE")) return QueryType::UPDATE;
    if (input.starts_with("DELETE")) return QueryType::DELETE;

    return QueryType::UNKNOWN;
}
bool Parser::parseAndSetQuery(const std::string &input, Query &query) {
    query.type = identifyQueryType(input);

    switch (query.type) {
        case QueryType::CREATE_TABLE:
            return parseCreateTable(input, query);
        case QueryType::SELECT:
            return parseSelect(input, query);
        case QueryType::INSERT_INTO:
            return parseInsert(input, query);
        case QueryType::UPDATE:
            return parseUpdate(input, query);
        case QueryType::DELETE:
            return parseDelete(input, query);
        default:
            throw invalid_argument("Unknown query type!");
    }
}

bool Parser::parseInsert(const std::string &input, Query &query) {
    // INSERT INTO Users (name, surname, email, password) VALUES (recep taha, ayvaz, rtaha.ayvaz@gmail.com, 123123123)
    regex pattern("^\\s*INSERT\\s+INTO\\s+(\\w+)\\s*\\(([^)]+)\\)\\s*VALUES\\s*\\(([^)]+)\\)\\s*$");
    smatch matches;
    if (!regex_search(input, matches, pattern)) {
        throw invalid_argument("Insert query pattern doesnt match!");
    }

    query.tablesStr = matches[1].str();
    query.columnsStr = matches[2].str();
    query.valuesStr = matches[3].str();

    return true;
}

bool Parser::parseSelect(const std::string &input, Query& query) {
    // SELECT column1, column2, column3 FROM table1 WHERE condition
    regex pattern("^\\s*SELECT\\s+(.+?)\\s+FROM\\s+(.+?)(?:\\s+WHERE\\s+(.+))?$");
    smatch matches;

    if (!regex_search(input, matches, pattern)) {
        throw invalid_argument("Select query pattern doesnt match!");
    }

    query.tablesStr = matches[2].str();
    query.columnsStr = matches[1].str();
    query.conditionsStr = matches[3].matched ? matches[3].str() : "";

    return true;
}

bool Parser::parseCreateTable(const string &input, Query &query) {
    // CREATE TABLE tableName (id int PRİMARY KEY AUTOINCREMENT, user_id int, username VARCHAR(255) )
    regex pattern("^\\s*CREATE\\s+TABLE\\s*(\\w+?)\\s*\\(\\s*(.+)\\s*\\)");
    smatch matches;

    if (!regex_search(input, matches, pattern)) {
        throw invalid_argument("Create table query pattern doesnt match!");
    }

    query.tablesStr = matches[1].str();
    query.columnsStr = matches[2].str();

    return true;
}

bool Parser::parseUpdate(const string &input, Query &query) {
    // UPDATE table_name SET column1 = value1, column2 = value2 WHERE condition;
    regex pattern("^\\s*UPDATE\\s+(\\w+)\\s+SET\\s(.+?)\\s+(?:WHERE\\s+(.+?))?;?$");
    smatch matches;

    if (!regex_search(input, matches, pattern)) {
        throw invalid_argument("Update query pattern doesnt match!");
    }

    query.tablesStr = matches[1].str();
    query.columnsStr = matches[2].str();
    query.conditionsStr = matches[3].matched ? matches[3].str() : "";

    return true;
}

bool Parser::parseDelete(const string &input, Query &query) {
    // DELETE FROM table_name WHERE condition
    regex pattern("^\\s*DELETE\\s+FROM\\s+(\\w+)\\s*(?:\\s*WHERE\\s+(.+))?;?$");
    smatch matches;

    if (!regex_search(input, matches, pattern)) {
        throw invalid_argument("Delete query pattern doesnt match!");
    }

    query.tablesStr = matches[1].str();
    query.conditionsStr = matches[2].matched ? matches[2].str() : "";

    return true;
}
