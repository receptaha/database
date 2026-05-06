#ifndef COLUMN_H
#define COLUMN_H
#include <string>
#include "DataType.h"
#include <unordered_set>
using namespace std;

struct Column {
    string name;
    DataType type;
    uint32_t size;
    uint32_t offset;
    vector<string> constraints;

    inline static const unordered_map<string, DataType> SUPPORTED_TYPES = {
        {"INT", DataType::INT},
        {"VARCHAR", DataType::VARCHAR},
        {"BOOL", DataType::BOOLEAN},
    };

    inline static const unordered_set<string> SUPPORTED_CONSTRAINTS = {
        "PRIMARY KEY", "NOT NULL", "AUTO INCREMENT", "UNIQUE"
    };
};

#endif
