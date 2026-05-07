#ifndef COLUMN_H
#define COLUMN_H
#include <string>
#include "DataType.h"
#include <unordered_set>

#include "Constraint.h"
enum class Constraint;
using namespace std;

struct Column {
    string name;
    DataType type;
    uint32_t size;
    uint32_t offset;
    vector<Constraint> constraints;

    inline static const unordered_map<string, DataType> SUPPORTED_TYPES = {
        {"INT", DataType::INT},
        {"VARCHAR", DataType::VARCHAR},
        {"BOOL", DataType::BOOLEAN},
    };

    inline static const unordered_map<string, Constraint> SUPPORTED_CONSTRAINTS = {
        {"PRIMARY KEY", Constraint::PRIMARY_KEY},
        {"NOT NULL", Constraint::NOT_NULL},
        {"UNIQUE", Constraint::UNIQUE},
        {"AUTOINCREMENT", Constraint::AUTOINCREMENT}
    };
};

#endif
