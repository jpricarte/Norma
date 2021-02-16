//
// Created by jordi on 16/02/2021.
//

#ifndef NORMA_INTEPRETER_H
#define NORMA_INTEPRETER_H

#include "Machine.h"
#include <iostream>

using namespace std;

enum Operation {
    INC,
    DEC,
    IF
};

typedef tuple<string,Operation,string,string> parsedOperation;

class Intepreter {
public:
    Intepreter();

    int ReadFile(string fileName);

    uint64_t execute();

    void setInput(uint64_t input);

private:
    Machine *machine;
    vector<parsedOperation> operationList;

    parsedOperation parseLine(string line);
    parsedOperation executeOperation(parsedOperation op);
    string getPath(bool resultIf, string paths);
    parsedOperation findNextOperation(string nextOperation);
};


#endif //NORMA_INTEPRETER_H
