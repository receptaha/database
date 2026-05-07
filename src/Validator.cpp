#include "../include/Validator.h"

#include <sstream>
#include "../include/Column.h"
#include "../include/DataType.h"

bool Validator::validateAndSetQuery(Query &query) {
    switch (query.type) {
        case QueryType::CREATE_TABLE:
            return validateCreateTable(query);
        case QueryType::SELECT:
            return validateSelect(query);
        case QueryType::INSERT_INTO:
            return validateInsert(query);
        case QueryType::UPDATE:
            return validateUpdate(query);
        case QueryType::DELETE:
            return validateDelete(query);
        default:
            return false;
    }
}

bool Validator::validateCreateTable(Query &query) {
    // This method validates only data type and constraints!! Not table
    // This method sets query's parsedColumns

    // CREATE TABLE tableName (id int PRIMARY KEY AUTOINCREMENT, user_id int, username VARCHAR)
    if (query.tablesStr.empty()) {
        throw invalid_argument("Table name cannot be null for create table query type!");
    }
    if (query.columnsStr.empty()) {
        throw invalid_argument("Columns and constraints cannot be null for create table query type!");
    }

    uint32_t totalOffset = 0;
    vector<string> columnDefinitions;
    stringstream ss(query.columnsStr);
    string word;

    while (getline(ss, word, ',')) columnDefinitions.push_back(word);
    for (const string& definition: columnDefinitions) {
        string colName, colType;
        stringstream colDef(definition);
        colDef >> colName; colDef >> colType;
        // Column type validation
        transform(colType.begin(), colType.end(), colType.begin(), ::toupper);
        if (!Column::SUPPORTED_TYPES.contains(colType)) {
            throw invalid_argument("Unsupported data type for '" + colName + "' column: " + colType);
        }

        // Column constraints validation
        vector<string> constraints, finded_constraints;
        while (colDef >> word) {
            transform(word.begin(), word.end(), word.begin(), ::toupper);
            constraints.push_back(word);
        };

        if (!constraints.empty()) {
            string findedWord;
            string totalWord;
            for (size_t i = 0; i < constraints.size(); i++) {
                findedWord += (findedWord.empty() ? "" : " ") + constraints[i];
                if (Column::SUPPORTED_CONSTRAINTS.contains(findedWord)) {
                    totalWord += (totalWord.empty() ? "" : " ") + findedWord;
                    findedWord.clear();
                }
            }
            stringstream ss2(totalWord);
            string differents_str;

            while (getline(ss2, word, ' ')) finded_constraints.push_back(word);

            for (auto i = 0; i < constraints.size(); i++) {
                if ((i < finded_constraints.size() && finded_constraints[i] != constraints[i]) || (i >= finded_constraints.size())) {
                    differents_str += (differents_str.empty() ? "" : " ") + constraints[i];
                }
            }

            if (!differents_str.empty()) {
                throw invalid_argument("There is unmatched constraints for column '" + colName + "': " + differents_str);
            }

            transform(colType.begin(), colType.end(), colType.begin(), ::toupper);
            DataType validType = Column::SUPPORTED_TYPES.find(colType)->second;
            Column validColumn;
            validColumn.name = colName;
            validColumn.size = getDataTypeSize(validType);
            validColumn.type = validType;
            validColumn.offset = totalOffset;
            validColumn.constraints = finded_constraints;
            totalOffset += getDataTypeSize(validType);
            query.parsedColumns.push_back(validColumn);
        }
    }
    return true;
}

bool Validator::validateInsert(Query &query) {
    // INSERT INTO Users (name, surname, email, password) VALUES (recep taha, ayvaz, rtaha.ayvaz@gmail.com, 123123123)
}

bool Validator::validateSelect(Query &query) {
    // SELECT column1, column2, column3 FROM table1 WHERE condition
    if (query.tablesStr.empty()) {
        throw invalid_argument("Table name cannot be null for select query type");
    }
    if (query.columnsStr.empty()) {
        throw invalid_argument("Specified columns cannot be null for select query type");
    }

    stringstream ss(query.columnsStr);
    string word;

    while (getline(ss, word, ',')) {
        auto first = word.find_first_not_of(" ");
        auto last = word.find_last_not_of(" ");
        word = word.substr(first, last - first + 1);
        query.targetColumns.push_back(word);
    }
    return true;
}

bool Validator::validateUpdate(Query &query) {
    // UPDATE table_name SET column1 = value1, column2 = value2 WHERE condition;
}

bool Validator::validateDelete(Query &query) {
}
