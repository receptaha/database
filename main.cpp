#include <iostream>
#include <string>
#include <cstdlib>
#include "include/Database.h"
#include "include/Parser.h"
#include "include/Query.h"

using namespace std;

int main(int argc, char* argv[]) {
    // if (argc < 2) {
    //     cerr << "ERROR: Database file not specified!" << endl;
    //     cerr << "Usage: ./mydb <database_name.db>" << endl;
    //     return EXIT_FAILURE;
    // }

    string filename = "my_db";

    Database currentDb(filename);

    cout << "Connected Database: " << filename << endl;

    string input;
    while (true) {
        cout << currentDb.getDbName() << ">";
        getline(cin, input);

        if (input == "exit" || input == "quit") {
            cout << "Bye!";
            break;
        }

        if (input.empty()) continue;

        currentDb.exec(input);
    }

    return EXIT_SUCCESS;
}