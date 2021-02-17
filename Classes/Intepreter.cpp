//
// Created by jordi on 16/02/2021.
//

#include <fstream>
#include <tuple>
#include <algorithm>
#include <utility>
#include <sstream>
#include "Intepreter.h"

using namespace std;

// Trim algorithm from http://www.martinbroadhurst.com/how-to-trim-a-stdstring.html
std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    return ltrim(rtrim(str, chars), chars);
}

// Auxiliaries classes
Subrotine::Subrotine(const string &name, const string &registers) {
    this->name = name;
    this->registers = vector<tuple<string,string>>();

    string reg_name="";
    for (char c : registers) {
        if(c == ','){
            this->registers.emplace_back(reg_name,"");
            reg_name="";
        }
        else {
            reg_name.push_back(c);
        }
    }
    this->registers.emplace_back(reg_name,"");

    this->operations = vector<ParsedOperation*>();

}

ParsedOperation::ParsedOperation(string lineId, LineType lineType, Operation operation, string regName,
                                 string nextLine) {
    this->lineId=std::move(lineId);
    this->operation=operation;
    this->nextLine=std::move(nextLine);
    this->regName=std::move(regName);
    this->lineType=lineType;
}

// Main Class and methods
Intepreter::Intepreter() {
    machine = new Machine();
    actualSubrotine = vector<Subrotine*>();
}

// Public Methods

int Intepreter::ReadFile(string fileName) {
    //Open file
    ifstream file(fileName);
    //Read lines
    string line;
    while(!file.eof()){
        getline(file, line);

        line = formatLine(line);
        if (line.empty())
            continue;

        if (line.find("OPERATION") != string::npos && line.find_first_of('{') != string::npos) {
            auto tokens = lineDivide(line);
            auto subrotine = new Subrotine(tokens[1], tokens[2]);
            subrotines.push_back(subrotine);
            int status = readSubrotine(&file, subrotine);
            if (status == -1) {
                file.close();
                return -2;
            }
        }

        if (line.find("MAIN") != string::npos && line.find_first_of('{') != string::npos) {
            int status = readMain(&file);
            if (status == -1) {
                file.close();
                return -2;
            }
        }


    }
    file.close();
    return 0;
}

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

//Private non static methods
int Intepreter::readMain(ifstream *file) {
    while(!file->eof()) {
        string line;
        getline(*file, line);

        line = formatLine(line);

        if (line == "}") {
            return 0;
        }

        auto parsedLine = parseLine(line);

        if (parsedLine== nullptr) {
            return -1;
        }

        mainOperationList.push_back(parsedLine);
    }
}

int Intepreter::readSubrotine(ifstream *file, Subrotine* subrotine) {
    while(!file->eof()) {
        string line;
        getline(*file, line);

        line = formatLine(line);

        if (line == "}") {
            return 0;
        }

        auto parsedLine = parseLine(line);

        if (parsedLine== nullptr) {
            return -1;
        }

        subrotine->operations.push_back(parsedLine);
    }
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
    Operation operation = op->operation;
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
    Operation operation = op->operation;
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

ParsedOperation *Intepreter::parseLine(string line) {

    auto tokens = lineDivide(line);

    if (tokens.size() != 6 && tokens.size() != 10)
        return nullptr;

    auto lineId = formatLineId(tokens[0]);
    auto lineType = formatlineType(tokens[1]);
    auto operation = formatOperation(tokens[2]);

    //If have some error, return nullptr
    if (lineType==ERROR || operation==NO_OP)
        return nullptr;

    string regName;
    //Get Reg or Params (now just Reg)
    if (operation==FUNC) {
        regName = tokens[2]+","+tokens[3];
    } else {
        regName = tokens[3];
    }


    //get nextLine (2 for tests)
    string nextLine;
    if (lineType==TEST)
        nextLine = formatNextLine(tokens[6], tokens[9]);
    else nextLine = tokens[5];

    return new ParsedOperation(lineId,lineType,operation,regName,nextLine);
}

vector<string> Intepreter::lineDivide(const string& line) {
    vector<string> tokens;
    istringstream tokenizer { line };
    string token;

    // separa as string por espaço e coloca no vetor destino
    while (tokenizer >> token)
        tokens.push_back(token);

    return tokens;
}

string Intepreter::formatLineId(const string &idToken) {
    if(int end = idToken.find_last_of(':')!=string::npos)
        return idToken.substr(0,idToken.size()-1);
    else return idToken;
}

LineType Intepreter::formatlineType(const string &lineTypeToken) {
    if (lineTypeToken=="DO")
        return OPERATOR;
    else if (lineTypeToken=="IF")
        return TEST;
    else return ERROR;
}

Operation Intepreter::formatOperation(const string &operationToken) {
    if(operationToken=="INC")
        return INC;

    else if(operationToken=="DEC")
        return DEC;

    else if(operationToken=="ZERO")
        return ZERO;

    else return FUNC;

}

string Intepreter::formatParams(const string &functionToken) {
    int begin = functionToken.find_first_of('(');
    int end   = functionToken.find_first_of(')');
    return functionToken.substr(begin, end-begin);
}

string Intepreter::formatNextLine(const string &firstLineId, const string &secondLineId) {
    return firstLineId+","+secondLineId;
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

string Intepreter::formatLine(string line) {
    //Ignore empty lines
    if(line == "")
        return "";

    //Remove spaces from beginning and ending
    line = trim(line);

    //Ignore comments
    if (line.find("//") == 0)
        return "";

    //Make uppercase
    for (auto & c: line) c = toupper(c);

    return line;
}
