#ifndef DATATYPE_H
#define DATATYPE_H
#include <iostream>


enum class DataType {
    INT,
    BOOLEAN,
    VARCHAR_32,
    VARCHAR_255
};

uint32_t getDataTypeSize(DataType type);

#endif
