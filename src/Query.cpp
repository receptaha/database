//
// Created by Recep on 1.05.2026.
//

#include "../include/Query.h"
#include <iostream>

void Query::printDetails() const {
    cout << "Query details are printing..." << endl;
    cout << "Tables String: " << this->tablesStr << endl;
    cout << "Columns String: " << this->columnsStr << endl;
    cout << "Values String: " << this->valuesStr << endl;
    cout << "Conditions String: " << this->conditionsStr << endl;

    cout << "-----------------------------------" << endl;
}

using namespace std;