//
// Created by jordi on 16/02/2021.
//

#include <fstream>
#include <tuple>
#include <algorithm>
#include "Intepreter.h"

using namespace std;

Intepreter::Intepreter() {
    machine = new Machine();
}

int Intepreter::ReadFile(string fileName) {
    //Open file
    ifstream file(fileName);
    //Read lines
    string line;
    while(!file.eof()){
        getline(file, line);
        if(line == "")
            continue;

        for (auto & c: line) c = toupper(c);
        auto parsedLine = parseLine(line);
        if (parsedLine == parsedOperation()) {
            file.close();
            return -2; ///Bad line sintax
        }
        operationList.push_back(parsedLine);
    }
    file.close();
    return 0;
}

uint64_t Intepreter::execute() {

    parsedOperation actualOperation = operationList[0];
    do {
        actualOperation = executeOperation(actualOperation);
    } while (actualOperation != parsedOperation());

    return machine->finalResult();
}

void Intepreter::setInput(uint64_t input) {
    machine->input(input);
}


parsedOperation Intepreter::parseLine(string line) {

    int i=0;

    string lineId = "";
    for(;i!=line.size();i++) {
        char c = line[i];
        if(c==':') break;
        // ignore spaces
        if(c!=' ')
            lineId.push_back(c);
    }
    if (lineId == line || lineId == "")
        return parsedOperation();

    i++;
    string operationString  = "";
    Operation operation;
    // ignore spaces
    for(;i!=line.size();i++) {
        char c = line[i];
        if(c!=' ') break;
    }
    // read operation
    for(;i!=line.size();i++) {
        char c = line[i];
        if(c==' ') break;
        operationString.push_back(c);
    }
    // convert to enum
    if (operationString == "INC")
        operation = INC;
    else if (operationString == "DEC")
        operation = DEC;
    else if (operationString == "IF")
        operation = IF;
    else
        return parsedOperation();

    i++;
    string param = "";
    // ignore spaces
    for(;i!=line.size();i++) {
        char c = line[i];
        if(c!=' ') break;
    }
    // read param
    for(;i!=line.size();i++) {
        char c = line[i];
        if(c==' ') break;
        param.push_back(c);
    }
    if (param == "" || i==line.size())
        return parsedOperation();


    string nextOperation = "";
    // ignore spaces
    for(;i!=line.size();i++) {
        char c = line[i];
        if(c!=' ') break;
    }
    for(;i!=line.size();i++) {
        char c = line[i];
        if(c==' ') break;
        nextOperation.push_back(c);
    }

    if (param == ""){
        return parsedOperation();
    }

    return parsedOperation(lineId, operation, param, nextOperation);
}

parsedOperation Intepreter::executeOperation(parsedOperation op) {
    Operation operation = get<1>(op);
    string nextOperation;
    bool resultIf;
    switch (operation) {
        case INC:
            machine->inc(get<2>(op));
            nextOperation = get<3>(op);
            break;
        case DEC:
            machine->dec(get<2>(op));
            nextOperation = get<3>(op);
            break;
        case IF:
        default:
            resultIf = machine->isZero(get<2>(op));
            nextOperation = getPath(resultIf, get<3>(op));
    }

    return findNextOperation(nextOperation);

}

string Intepreter::getPath(bool resultIf, string paths) {
    string path1,path2;
    int i=0;
    for(;i!=paths.size();i++){
        char c = paths[i];
        if(c==';'){
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

parsedOperation Intepreter::findNextOperation(string nextOperation) {
    for(int i=0; i<operationList.size(); i++) {
        if(get<0>(operationList[i]) == nextOperation) {
            return operationList[i];
        }
    }

    return parsedOperation();
}

