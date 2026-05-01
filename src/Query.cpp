//
// Created by Recep on 1.05.2026.
//

#include "../include/Query.h"
#include <iostream>
using namespace std;

void Query::printDetails() const {
    cout << "--- HEDEF TABLO: " << this->targetName << " ---" << endl;

    cout << "Kolonlar:\t";
    for (const auto& column : this->columns) cout << column << "\t";
    cout << endl;

    cout << "Degerler:\t";
    for (const auto& value : this->values) cout << value << "\t";
    cout << endl;
    cout << "-----------------------------------" << endl;
}
