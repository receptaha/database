#include "../include/DataType.h"

uint32_t getDataTypeSize(DataType type) {
    switch (type) {
        case DataType::BOOLEAN:
            return 1;
        case DataType::INT:
            return 4;
        case DataType::VARCHAR_32:
            return 32;
        case DataType::VARCHAR_255:
            return 255;
        default:
            return 0;
    }
}
