#include "../include/Database.h"

Database::Database() {
}

Database::~Database() {
    disconnect();
}

void Database::create(const string& path) {
    fstream file;

    file.open(path, ios::in | ios::binary);
    if (file.is_open()) {
        cerr << "ERROR: Database '" << path << "' already exists!" << endl;
        file.close();
        return;
    }

    file.clear();

    file.open(path, ios::out | ios::binary);
    if (file.is_open()) {
        cout << "SUCCESS: '" << path << "' database has been created." << endl;
        file.close();
    } else {
        cerr << "CREATE ERROR: Could not create database file at " << path << endl;
    }
}

void Database::connect(const string& path) {
    if (this->connection.is_open()) {
        this->connection.close();
    }

    this->connection.open(path, ios::in | ios::out | ios::binary);

    if (!this->connection.is_open()) {
        this->connection.clear();
        cerr << "CONNECTION ERROR: Database '" << path << "' not found or could not be opened." << endl;
    } else {
        this->filePath = path;
        cout << "SUCCESS: Connected to " << path << " (Ready for I/O)" << endl;
    }
}

void Database::disconnect() {
    if (this->connection.is_open()) {
        this->connection.close();
        cout << "INFO: Disconnected from " << this->filePath << endl;
        this->filePath = "";
    }
}

bool Database::isOpen() const {
    return this->connection.is_open();
}