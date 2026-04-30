#ifndef COLUMN_H
#define COLUMN_H
#include <string>
#include "DataType.h"
using namespace std;

struct Column {
    string name;
    DataType type;
    uint32_t size;
    uint32_t offset;
};

#endif
