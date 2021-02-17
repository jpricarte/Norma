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

class Subrotine {
public:
    string name;
    vector<tuple<string, string>> registers;
    vector<ParsedOperation*> operations;

    Subrotine(const string &name, const string &registers);
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
    vector<Subrotine*> subrotines;
    vector<Subrotine*> actualSubrotine;

    int readMain(ifstream* file);
    int readSubrotine(ifstream* file,Subrotine* subrotine);

    bool findSubRotine(string params);
    bool executeSubrotine(Subrotine* subrotine);
    static string findRealRegister(string paramName, Subrotine *subrotine);

    ParsedOperation *executeOperation(ParsedOperation *op);
    ParsedOperation *executeOperation(ParsedOperation *op, Subrotine *subrotine);

    ParsedOperation *findNextOperation(string nextOperation);
    ParsedOperation *findNextOperation(string nextOperation, Subrotine *subrotine);

    static ParsedOperation *parseLine(string line);
    static vector<string> lineDivide(const string& line);
    static string formatLineId(const string& idToken);
    static LineType formatlineType(const string& lineTypeToken);
    static Operation formatOperation(const string& operationToken);
    static string formatParams(const string& functionToken);
    static string formatNextLine(const string& firstLineId,const string& secondLineId);

    static string getPath(bool resultIf, string paths);
    static string formatLine(string line);
};


#endif //NORMA_INTEPRETER_H
