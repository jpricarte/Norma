//
// Created by jordi on 16/02/2021.
//

#include <tuple>
#include <algorithm>
#include "Intepreter.h"

using namespace std;

// Main Class and methods
Intepreter::Intepreter() {
    machine = new Machine();
    actualSubrotine = vector<Subrotine*>();
}

Intepreter::Intepreter(const vector<ParsedOperation *> &mainOperationList, const vector<Subrotine *> &subrotines)
        : mainOperationList(mainOperationList), subrotines(subrotines) {
    machine = new Machine();
    actualSubrotine = vector<Subrotine*>();
}


// Public Methods
uint64_t Intepreter::execute() {

    ParsedOperation *actualOperation = mainOperationList[0];
    do {
        actualOperation = executeOperation(actualOperation);
    } while (actualOperation != nullptr);

    return machine->finalResult();
}

void Intepreter::setInput(uint64_t input) {
    machine->input(input);
}

bool Intepreter::findSubRotine(string params) {
    auto paramsList = vector<string>();
    string reg_name;
    for (char c : params) {
        if(c == ','){
            paramsList.push_back(reg_name);
            reg_name="";
        }
        else {
            reg_name.push_back(c);
        }
    }
    paramsList.push_back(reg_name);

    //Find subrotine and associate regs to params
    int i=0;
    for (i=0; i<subrotines.size();i++) {
        if (subrotines[i]->name==paramsList[0]) {
            for (int j=1;j<paramsList.size();j++) {
                get<1>(subrotines[i]->registers[j-1])=paramsList[j];
            }
            break;
        }
    }

    actualSubrotine.push_back(subrotines[i]);
    bool subRotineReturn = executeSubrotine();
    actualSubrotine.pop_back();
    return subRotineReturn;
}

bool Intepreter::executeSubrotine() {
    auto subrotine = actualSubrotine.back();
    ParsedOperation *actualOperation = subrotine->operations[0];
    do {
        actualOperation = executeOperation(actualOperation, subrotine);
        if (actualOperation == nullptr)
            return true;
        if(actualOperation->nextLine=="TRUE")
            return true;
        else if(actualOperation->nextLine=="FALSE")
            return false;
    } while (actualOperation != nullptr);

}

string Intepreter::findRealRegister(string paramName, Subrotine *subrotine) {
    for(auto p : subrotine->registers) {
        if(paramName==get<0>(p))
            return get<1>(p);
    }
}

ParsedOperation *Intepreter::executeOperation(ParsedOperation *op) {
    Operations operation = op->operation;
    string nextOperation;
    bool resultIf;
    switch (operation) {
        case INC:
            machine->inc(op->regName);
            nextOperation = op->nextLine;
            break;
        case DEC:
            machine->dec(op->regName);
            nextOperation = op->nextLine;
            break;
        case ZERO:
            resultIf = machine->isZero(op->regName);
            nextOperation = getPath(resultIf, op->nextLine);
        case FUNC:
            resultIf = findSubRotine(op->regName);
            nextOperation = (op->lineType==TEST) ? getPath(resultIf, op->nextLine) : op->nextLine;
    }

    return findNextOperation(nextOperation);
}

ParsedOperation *Intepreter::executeOperation(ParsedOperation *op, Subrotine *subrotine) {
    Operations operation = op->operation;
    string realRegister = findRealRegister(op->regName, subrotine);
    string nextOperation;
    bool resultIf;
    switch (operation) {
        case INC:
            machine->inc(realRegister);
            nextOperation = op->nextLine;
            break;
        case DEC:
            machine->dec(realRegister);
            nextOperation = op->nextLine;
            break;
        case ZERO:
            resultIf = machine->isZero(realRegister);
            nextOperation = getPath(resultIf, op->nextLine);
            break;
        case FUNC:
            resultIf = findSubRotine(op->regName);
            nextOperation = (op->lineType==TEST) ? getPath(resultIf, op->nextLine) : op->nextLine;
    }

    return findNextOperation(nextOperation, subrotine);
}

ParsedOperation *Intepreter::findNextOperation(string nextOperation) {
    for(int i=0; i<mainOperationList.size(); i++) {
        if(mainOperationList[i]->lineId == nextOperation) {
            return mainOperationList[i];
        }
    }
    return nullptr;
}

ParsedOperation *Intepreter::findNextOperation(string nextOperation, Subrotine *subrotine) {
    for(int i=0; i<subrotine->operations.size(); i++) {
        if(subrotine->operations[i]->lineId == nextOperation) {
            return subrotine->operations[i];
        }
    }
    return nullptr;
}

string Intepreter::getPath(bool resultIf, string paths) {
    string path1,path2;
    int i=0;
    for(;i!=paths.size();i++){
        char c = paths[i];
        if(c==','){
            i++;
            break;
        }
        path1.push_back(c);
    }
    for(;i!=paths.size();i++){
        char c = paths[i];
        path2.push_back(c);
    }

    return (resultIf) ? path1 : path2;
}