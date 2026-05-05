//
// Created by Recep on 30.04.2026.
//

#include "../include/Table.h"

#include "../include/Pager.h"

Table::Table(const string& name, Pager* p) : name(name), rowSize(0), numRows(0), pager(p) {
}

void Table::addColumn(const string& columnName, DataType columnType) {
    if (this->isColumnExists(columnName)) {
        cerr << "ADD COLUMN ERROR: '" << columnName << "' column is already exists in '" << this->name << "' table" <<  endl;
        return;
    }

    Column* newColumn = new Column();
    uint32_t sizeOfColumn = getDataTypeSize(columnType);
    newColumn->name = columnName;
    newColumn->size = sizeOfColumn;
    newColumn->type = columnType;
    newColumn->offset = this->rowSize;

    this->rowSize += sizeOfColumn;
    this->columns[columnName] = newColumn;

    cout << "Column added: (" << columnName << ")"
        << " Size: " << sizeOfColumn
        << " Column Offset: " << newColumn->offset << endl;
}

void Table::printSchema() const {
    cout << "--- Tablo Semasi: " << name << " ---" << endl;
    cout << "Toplam Satir Boyutu: " << rowSize << " byte" << endl;
    for (const auto& [columnName, column] : columns) {
        cout << "- " << column->name << " (Offset: " << column->offset << ")" << endl;
    }
}

bool Table::isColumnExists(const string &columnName) const {
    return this->columns.contains(columnName);
}


