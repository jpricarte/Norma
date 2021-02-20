//
// Created by pujol on 20/02/2021.
//

#ifndef NORMA_SUBROTINE_H
#define NORMA_SUBROTINE_H

#include "ParsedOperation.h"
#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

class Subrotine {
public:
    string name;
    vector<tuple<string, string>> registers;
    vector<ParsedOperation*> operations;

    Subrotine(const string &name, const string &registers);
};


#endif //NORMA_SUBROTINE_H
