#ifndef QUERYTYPE_H
#define QUERYTYPE_H

enum class QueryType {
    CREATE_TABLE,
    CREATE_DATABASE,
    CREATE,
    USE,
    SELECT,
    INSERT_INTO,
    UPDATE,
    DELETE,
    UNKNOWN
};

#endif
