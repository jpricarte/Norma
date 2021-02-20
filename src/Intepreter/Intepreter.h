//
// Created by jordi on 16/02/2021.
//

#ifndef NORMA_INTEPRETER_H
#define NORMA_INTEPRETER_H

#include "../Machine/Machine.h"
#include "../Enums/Operations.h"
#include "../Enums/OperationTypes.h"
#include "../AuxiliarClasses/ParsedOperation.h"
#include "../AuxiliarClasses/Subrotine.h"
#include <iostream>

using namespace std;

class Intepreter {
public:
    Intepreter();

    Intepreter(const vector<ParsedOperation *> &mainOperationList, const vector<Subrotine *> &subrotines);


    uint64_t execute();

    void setInput(uint64_t input);


private:
    Machine *machine;
    vector<ParsedOperation*> mainOperationList;
    vector<Subrotine*> subrotines;
    vector<Subrotine*> actualSubrotine;

    bool findSubRotine(string params);
    bool executeSubrotine();

    static string findRealRegister(string paramName, Subrotine *subrotine);

    ParsedOperation *executeOperation(ParsedOperation *op);
    ParsedOperation *executeOperation(ParsedOperation *op, Subrotine *subrotine);

    ParsedOperation *findNextOperation(string nextOperation);
    ParsedOperation *findNextOperation(string nextOperation, Subrotine *subrotine);

    static string getPath(bool resultIf, string paths);
};


#endif //NORMA_INTEPRETER_H
