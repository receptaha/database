//
// Created by Recep on 30.04.2026.
//

#include "../include/Table.h"

#include "../include/Pager.h"

Table::Table(const string& name, Pager* p) : name(name), rowSize(0), numRows(0), pager(p) {
}

void Table::addColumn(Column* column) {
    if (this->isColumnExists(column->name)) {
        cerr << "ADD COLUMN ERROR: '" << column->name << "' column is already exists in '" << this->name << "' table" <<  endl;
        return;
    }

    this->columns[column->name] = column;
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


