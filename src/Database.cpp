#include "../include/Database.h"

#include "../include/Parser.h"
#include "../include/Query.h"
#include "../include/Validator.h"

Database::Database(const string& filePath) : filePath(filePath) {
    this->initialize();
    this->loadTablesFromHeaders();
}

Database::~Database() {
    for (auto &[tableName, tablePtr] : tables) {
        delete tablePtr;
    }
}

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
    string headerFilePath = this->getHeaderFilePath(tableName);
    string tableFilePath  = this->getTableFilePath(tableName);

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
            uint8_t constraintVal = static_cast<uint8_t>(constraint);
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

string Database::getHeaderFilePath(const string &tableName) const {
    return getHeadersDirPath() + "/" + tableName + ".dat";
}

string Database::getTableFilePath(const string &tableName) const {
    return getTablesDirPath() + "/" + tableName + ".tbl";
}

void Database::loadTablesFromHeaders() {
    string headerDirPath = getHeadersDirPath();
    if (!filesystem::exists(headerDirPath)) {
        throw runtime_error("Header directory has not founded for loading, path: " + headerDirPath);
    }

    for (const auto& file: filesystem::directory_iterator(headerDirPath)) {
        if (file.path().extension() != ".dat") {
            cerr << file.path().c_str() << " is not a header file";
            continue;
        }

        if (!file.is_regular_file()) {
            cerr << file.path().c_str() << " is not a regular file";
            continue;
        }

        ifstream fstream(file.path().c_str(), ios::binary);
        if (!fstream.is_open()) {
            cerr << file.path().c_str() << " has not opened for reading mode";
            continue;
        }

        uint32_t columnCount;
        fstream.read(reinterpret_cast<char*>(&columnCount), sizeof(columnCount));

        uint32_t rowSize;
        fstream.read(reinterpret_cast<char*>(&rowSize), sizeof(rowSize));

        uint64_t rowCount;
        fstream.read(reinterpret_cast<char*>(&rowCount), sizeof(rowCount));

        uint8_t tableNameLength;
        fstream.read(reinterpret_cast<char*>(&tableNameLength), sizeof(tableNameLength));

        char nameBuffer[255] = {0};
        fstream.read(nameBuffer, 255);
        string tableName(nameBuffer);

        Pager* tablePager = new Pager(this->getTableFilePath(tableName));
        Table* loadedTable = new Table(tableName, tablePager);

        loadedTable->rowSize = rowSize;
        loadedTable->numRows = rowCount;

        for (uint32_t i = 0; i < columnCount; i++) {
            Column* column = new Column;

            uint8_t colNameLength;
            fstream.read(reinterpret_cast<char*>(&colNameLength), sizeof(colNameLength));

            char colNameBuffer[255] = {0};
            fstream.read(colNameBuffer, 255);
            string colName(colNameBuffer);
            column->name = colName;

            uint32_t typeInt;
            fstream.read(reinterpret_cast<char*>(&typeInt), sizeof(typeInt));
            auto colType = static_cast<DataType>(typeInt);
            column->type = colType;

            uint32_t columnSize, columnOffset;
            fstream.read(reinterpret_cast<char*>(&columnSize), sizeof(columnSize));
            column->size = columnSize;

            fstream.read(reinterpret_cast<char*>(&columnOffset), sizeof(columnOffset));
            column->offset = columnOffset;

            uint8_t constraintCount;
            fstream.read(reinterpret_cast<char *>(&constraintCount), sizeof(constraintCount));

            for (uint8_t j = 0; j < constraintCount; j++) {
                uint8_t constraintVal;
                fstream.read(reinterpret_cast<char *>(&constraintVal), sizeof(constraintVal));
                auto constraint = static_cast<Constraint>(constraintVal);
                column->constraints.push_back(constraint);
            }

            loadedTable->addColumn(column);
        }

        this->addTable(loadedTable);
    }
}

void Database::addTable(Table* table) {
    if (this->tables.contains(table->name)) {
        throw runtime_error(table->name + " table is already exists!");
    }

    this->tables[table->name] = table;
    cout << table->name << " table is added to database" << endl;
}



