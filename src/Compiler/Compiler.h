//
// Created by pujol on 20/02/2021.
//

#ifndef NORMA_COMPILER_H
#define NORMA_COMPILER_H

#include "../Machine/Machine.h"
#include "../Enums/Operations.h"
#include "../Enums/OperationTypes.h"
#include "../AuxiliarClasses/ParsedOperation.h"
#include "../AuxiliarClasses/Subrotine.h"
#include "../Intepreter/Intepreter.h"
#include <iostream>

using namespace std;

class Compiler {
public:
    Compiler();

    Intepreter* ReadFile(string fileName);


private:
    vector<ParsedOperation*> readMain(ifstream* file);
    int readSubrotine(ifstream* file, Subrotine* subrotine);

    static ParsedOperation *parseLine(string line);
    static vector<string> splitLine(const string& line);
    static string formatLineId(const string& idToken);
    static OperationTypes formatlineType(const string& lineTypeToken);
    static Operations formatOperation(const string& operationToken);
    static string formatNextLine(const string& firstLineId,const string& secondLineId);
    static string formatLine(string line);

    // Trim algorithm from http://www.martinbroadhurst.com/how-to-trim-a-stdstring.html
    static string& ltrim(std::string& str, const std::string& chars);
    static string& rtrim(std::string& str, const std::string& chars);
    static string& trim(std::string& str);
};


#endif //NORMA_COMPILER_H
