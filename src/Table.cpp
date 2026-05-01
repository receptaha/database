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

void Table::insert(const vector<Column> &inputColumns, const vector<string> &values) {
    if (inputColumns.empty()) {
        cerr << "INSERT ERROR: columns cannot be empty" << endl;
        return;
    }
    if (values.empty()) {
        cerr << "INSERT ERROR: values cannot be empty" << endl;
        return;
    }
    if (inputColumns.size() != values.size()) {
        cerr << "INSERT ERROR: columns count and values count must match" << endl;
        return;
    }

    char* rowBuffer = new char[this->rowSize];

    for (int i = 0; i < inputColumns.size(); i++) {
        bool isMatching = false;
        const Column& i_column = inputColumns[i];
        for (int j = 0; j < this->columns.size(); j++) {
            const Column& existColumn = this->columns[j];
            if (i_column.name == existColumn.name) {
                isMatching = true;
                break;
            }
        }

        if (!isMatching) {
            cerr << "INSERT ERROR: Invalid column: '" << i_column.name << "'" << endl;
            delete[] rowBuffer;
            return;
        }

        bool success = serializeColumnData(i_column, values[i], rowBuffer);
        if (!success) {
            delete[] rowBuffer;
            return;
        }
    }

    uint32_t rowsPerPage = PAGE_SIZE / this->rowSize;
    uint32_t pageNumber = this->numRows / rowsPerPage;
    uint32_t offsetInPage = (this->numRows % rowsPerPage) * this->rowSize;
    void* targetPage = this->pager->getPage(pageNumber);
    memcpy((char*) targetPage + offsetInPage, rowBuffer, this->rowSize);

    this->numRows++;
    this->pager->flush(pageNumber);
    delete[] rowBuffer;

    cout << "SUCCESS: Records has been writen to disk! -> Page: " << pageNumber << ", Total offset: " << numRows * rowSize << endl;
}

bool Table::serializeColumnData(const Column& column, const string& value, char* rowBuffer) {

    if (column.type == DataType::INT) {
        try {
            int intValue = stoi(value);

            memcpy(rowBuffer + column.offset, &intValue, column.size);
            return true;
        } catch (...) {
            cerr << "INSERT ERROR: Type mismatch! '" << value << "' is not a valid integer for column '" << column.name << "'" << endl;
            return false;
        }
    }

    if (column.type == DataType::VARCHAR_32 || column.type == DataType::VARCHAR_255) {

        if (value.size() > column.size) {
            cerr << "INSERT ERROR: Size overflow: '" << value << "' is too long for column '" << column.name << "'" << endl;
            return false;
        }

        memset(rowBuffer + column.offset, 0, column.size);
        memcpy(rowBuffer + column.offset, value.c_str(), value.size());
        return true;
    }

    return false;
}



