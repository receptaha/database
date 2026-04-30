//
// Created by Recep on 30.04.2026.
//

#include "../include/Table.h"

Table::Table(const string& name) : name(name) ,rowSize(0) {}

void Table::addColumn(const string& columnName, DataType columnType) {
    if (this->isColumnExists(columnName)) {
        cerr << "ADD COLUMN ERROR: '" << columnName << "' column is already exists in '" << this->name << "' table" <<  endl;
        return;
    }

    Column newColumn;
    uint32_t sizeOfColumn = getDataTypeSize(columnType);
    newColumn.name = columnName;
    newColumn.size = sizeOfColumn;
    newColumn.type = columnType;
    newColumn.offset = this->rowSize;

    this->rowSize += sizeOfColumn;
    this->columns.push_back(newColumn);

    cout << "Sütun eklendi: (" << columnName << ")"
        << " Boyut: " << sizeOfColumn
        << " Column Offset: " << newColumn.offset << endl;


}

void Table::printSchema() const {
    cout << "--- Tablo Semasi: " << name << " ---" << endl;
    cout << "Toplam Satir Boyutu: " << rowSize << " byte" << endl;
    for (const auto& col : columns) {
        cout << "- " << col.name << " (Offset: " << col.offset << ")" << endl;
    }
}

bool Table::isColumnExists(const string &columnName) {
    for(const auto& existColumn: this->columns) {
        if (existColumn.name == columnName) {
            return true;
        }
    }
    return false;
}

