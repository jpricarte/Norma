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
    ZERO,
    FUNC,
    NO_OP
};

enum LineType {
    OPERATOR,
    TEST,
    ERROR
};

class ParsedOperation {
public:
    string lineId;
    LineType lineType;
    Operation operation;
    string regName;
    string nextLine;

    ParsedOperation(string lineId,LineType lineType,Operation operation,string regName,string nextLine);
};

class Intepreter {
public:
    Intepreter();

    int ReadFile(string fileName);

    uint64_t execute();

    void setInput(uint64_t input);


private:
    Machine *machine;
    vector<ParsedOperation*> mainOperationList;

    ParsedOperation *parseLine(string line);

    static LineType findTypeLine(const string& line);
    static Operation findOperation(const string& line);
    static string findRegName(const string& line, LineType lineType, Operation operation);
    static string findNextLine(string line, LineType lineType);

    ParsedOperation *executeOperation(ParsedOperation *op);
    static string getPath(bool resultIf, string paths);
    ParsedOperation *findNextOperation(string nextOperation);
};


#endif //NORMA_INTEPRETER_H
