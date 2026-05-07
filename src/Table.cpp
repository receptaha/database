//
// Created by Recep on 30.04.2026.
//

#include "../include/Table.h"

#include "../include/Pager.h"

Table::Table(const string& name, Pager* p) : name(name), rowSize(0), numRows(0), pager(p) {
}

void Table::addColumn(Column* column) {
    if (this->isColumnExists(column->name)) {
        throw runtime_error(column->name + " column is already exists in " + this->name + " table");
    }

    this->columns[column->name] = column;
    cout << column->name << " column is added to " << this->name << " table" << endl;

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


