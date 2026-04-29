#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Database {
private:
    fstream connection;
    string filePath;

public:
    Database();
    ~Database();

    void create(const string& path);

    void connect(const string& path);

    void disconnect();

    bool isOpen() const;
};

#endif