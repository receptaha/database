#include <iostream>
using namespace std;

int main() {
    cout << "Welcome to Database" << endl;
    cout << "db> ";
    string input;

    while (true) {
        getline(cin, input);

        if (input == "exit" || input == "quit") {
            cout << "Bye!" << endl;
            break;
        }
    }
}