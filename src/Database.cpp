#include "../include/Database.h"

#include "../include/Parser.h"
#include "../include/Query.h"
#include "../include/Validator.h"

Database::Database(const string& filePath) : filePath(filePath) {
    Database::initialize();
}

Database::~Database() {
    for (auto const& [tableName, tablePtr] : tables) {
        delete tablePtr;
    }
    delete pager;
}

#include <filesystem>
#include <stdexcept>

void Database::initialize() const {
    if (filesystem::exists(this->filePath) && !filesystem::is_directory(this->filePath)) {
        throw runtime_error("FATAL ERROR: '" + this->filePath + "' adinda bir DOSYA zaten mevcut. Klasor olusturulamiyor. Lutfen dosyayi silin.");
    }

    if (!filesystem::exists(this->filePath)) {
        filesystem::create_directory(this->filePath);
        cout << "Database directory created: " << this->filePath << endl;
    }

    string headerPath = this->getHeadersDirPath();
    if (filesystem::exists(headerPath) && !filesystem::is_directory(headerPath)) {
        throw runtime_error("FATAL ERROR: '" + headerPath + "' adinda bir DOSYA zaten mevcut. Klasor olusturulamiyor. Lutfen dosyayi silin.");
    }

    if (!filesystem::exists(headerPath)) {
        filesystem::create_directory(headerPath);
        cout << "Headers directory created." << endl;
    }

    string tablesPath = this->getTablesDirPath();
    if (filesystem::exists(tablesPath) && !filesystem::is_directory(tablesPath)) {
        throw runtime_error("FATAL ERROR: '" + tablesPath + "' adinda bir DOSYA zaten mevcut. Klasor olusturulamiyor. Lutfen dosyayi silin.");
    }

    if (!filesystem::exists(tablesPath)) {
        filesystem::create_directory(tablesPath);
        cout << "Tables directory created." << endl;
    }
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

bool Database::exec(const string &input) const {
    try {
        Query query;
        if (!Parser::parseAndSetQuery(input, query)) return false;
        if (!Validator::validateAndSetQuery(query)) return false;

        switch (query.type) {
            case QueryType::CREATE_TABLE:
                return this->execCreateTable(query);
            default:
                throw runtime_error("Unknown query type for query execution");
        }
    }catch (exception& e) {
        cout << e.what() << endl;
        return false;
    }
}

bool Database::execCreateTable(const Query &query) const {
    string tableName = query.tablesStr;
    string headerFilePath = getHeadersDirPath() + "/" + tableName + ".dat";
    string tableFilePath  = getTablesDirPath() + "/" + tableName + ".tbl";

    if (filesystem::exists(headerFilePath) || filesystem::exists(tableFilePath)) {
        throw runtime_error(tableName + " table already exists");
    }

    ofstream tableFile(tableFilePath);
    tableFile.close();

    ofstream file(headerFilePath, ios::binary);
    if (!file.is_open()) throw runtime_error("Header file has not opened in writing mode");

    uint32_t columnCount = query.parsedColumns.size();
    file.write(reinterpret_cast<const char *>(&columnCount), sizeof(columnCount));

    uint32_t rowSize = 0;
    for (const auto& column : query.parsedColumns) {
        rowSize += column.size;
    }
    file.write(reinterpret_cast<const char *>(&rowSize), sizeof(rowSize));

    uint64_t rowCount = 0;
    file.write(reinterpret_cast<const char *>(&rowCount), sizeof(rowCount));

    uint8_t tableNameLength = tableName.size();
    file.write(reinterpret_cast<const char *>(&tableNameLength), sizeof(tableNameLength));

    char nameBuffer[255] = {0};
    strncpy(nameBuffer, tableName.c_str(), 255);
    file.write(nameBuffer, 255);

    for (const auto& column : query.parsedColumns) {
        uint8_t colNameLength = column.name.size();
        file.write(reinterpret_cast<const char *>(&colNameLength), sizeof(colNameLength));

        char colNameBuffer[255] = {0};
        strncpy(colNameBuffer, column.name.c_str(), 255);
        file.write(colNameBuffer, 255);

        uint32_t typeInt = static_cast<uint32_t>(column.type);
        file.write(reinterpret_cast<const char *>(&typeInt), sizeof(typeInt));

        file.write(reinterpret_cast<const char *>(&column.size), sizeof(uint32_t));
        file.write(reinterpret_cast<const char *>(&column.offset), sizeof(uint32_t));

        uint8_t constraintCount = column.constraints.size();
        file.write(reinterpret_cast<const char *>(&constraintCount), sizeof(uint8_t));

        for (const auto& constraint : column.constraints) {
            uint8_t constraintVal = static_cast<uint8_t>(Column::SUPPORTED_CONSTRAINTS.find(constraint)->second);
            file.write(reinterpret_cast<const char *>(&constraintVal), sizeof(uint8_t));
        }
    }

    file.close();
    return true;
}
string Database::getHeadersDirPath() const {
    return this->filePath + "/headers";
}

string Database::getTablesDirPath() const {
    return this->filePath + "/tables";
}

