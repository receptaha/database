#include "../include/Database.h"

Database::Database(const string& filePath) : filePath(filePath) {
    this->pager = new Pager(filePath);
}

Database::~Database() {
    for (auto const& [tableName, tablePtr] : tables) {
        delete tablePtr;
    }
    delete pager;
}

void Database::addTable(const string& tableName) {
    if (tables.contains(tableName)) {
        cerr << "ADD TABLE ERROR: '" << tableName << "' has already been created!" << endl;
        return;
    }

    Table* newTable = new Table(tableName, this->pager);
    tables[tableName] = newTable;
    cout << "Table has been created: " << tableName << endl;
}

Table* Database::getTable(const string& tableName) {
    if (!tables.contains(tableName)) {
        cerr << "GET TABLE ERROR: '" << tableName << "' has not found!" << endl;
        return nullptr;
    }
    return tables[tableName];
}

string Database::getDbName() {
    return this->filePath;
}
