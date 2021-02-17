//
// Created by jordi on 16/02/2021.
//

#include <fstream>
#include <tuple>
#include <algorithm>
#include <utility>
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

        //Ignore empty lines
        if(line == "")
            continue;

        //Remove spaces from beginning and ending
        line = trim(line);

        //Ignore comments
        if (line.find("//") == 0)
            continue;

        //Make uppercase
        for (auto & c: line) c = toupper(c);

        auto parsedLine = parseLine(line);

        if (parsedLine== nullptr) {
            file.close();
            return -1;
        }

        mainOperationList.push_back(parsedLine);

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

ParsedOperation *Intepreter::parseLine(string line) {
    int begin=0,end=0;

    //Get lineId
    end=line.find_first_of(':');
    string lineId = line.substr(begin,end);

    //Get lineType
    LineType lineType=findTypeLine(line);
    if (lineType==ERROR)
        return nullptr;

    Operation operation=findOperation(line);
    if (lineType==ERROR)
            return nullptr;

    //Get regName
    string regName = findRegName(line, lineType, operation);
    if (regName.empty())
        return nullptr;

    string nextLine = findNextLine(line, lineType);
    if (nextLine.empty() || nextLine==";")
        return nullptr;

    return new ParsedOperation(lineId,lineType,operation,regName,nextLine);
}

LineType Intepreter::findTypeLine(const string& line) {
    if(line.find("IF") != string::npos) {
        return TEST;
    } else if(line.find("DO") != string::npos) {
        return OPERATOR;
    } else return ERROR;
}

Operation Intepreter::findOperation(const string &line) {
    if(line.find("DO INC") != string::npos) {
        return INC;
    } else if(line.find("DO DEC") != string::npos) {
        return DEC;
    } else if(line.find("IF ZERO") != string::npos) {
        return ZERO;
    } else if(line.find('(') != string::npos) {
        return FUNC;
    }else return NO_OP;
}

string Intepreter::findRegName(const string &line, LineType lineType, Operation operation) {
    string wordBefore, wordAfter;
    int increment=0;
    switch (operation) {
        case INC:
            wordBefore="INC";
            increment=3; break;
        case DEC:
            wordBefore="DEC";
            increment=3; break;
        case ZERO:
            wordBefore="ZERO";
            increment=4; break;
        case FUNC:
        default:
            wordBefore="(";
            increment=1;
    }

    switch (lineType) {
        case OPERATOR:
            wordAfter="GOTO"; break;
        case TEST:
        default:
            wordAfter="THEN"; break;
    }

    int begin=line.find(wordBefore)+increment;
    int end=line.find(wordAfter);

    string reg = line.substr(begin,end-begin);

    return trim(reg);
}

string Intepreter::findNextLine(string line, LineType lineType) {
    if (lineType==OPERATOR) {
        int begin = line.find("GOTO")+4;
        string nextLine = line.substr(begin);
        return trim(nextLine);
    }
    else if (lineType==TEST) {
        int beginFirst = line.find("THEN GOTO")+9;
        int beginSecond = line.find("ELSE GOTO");
        string nextLineTrue = line.substr(beginFirst, beginSecond-beginFirst);
        nextLineTrue = trim(nextLineTrue);

        beginSecond+=9;
        string nextLineFalse = line.substr(beginSecond);
        nextLineFalse = trim(nextLineFalse);

        return nextLineTrue+";"+nextLineFalse;
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
        default:
            resultIf = machine->isZero(op->regName);
            nextOperation = getPath(resultIf, op->nextLine);
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

ParsedOperation *Intepreter::findNextOperation(string nextOperation) {
    for(int i=0; i<mainOperationList.size(); i++) {
        if(mainOperationList[i]->lineId == nextOperation) {
            return mainOperationList[i];
        }
    }
    return nullptr;
}

void Intepreter::setInput(uint64_t input) {
    machine->input(input);
}

ParsedOperation::ParsedOperation(string lineId, LineType lineType, Operation operation, string regName,
                                 string nextLine) {
    this->lineId=std::move(lineId);
    this->operation=operation;
    this->nextLine=std::move(nextLine);
    this->regName=std::move(regName);
    this->lineType=lineType;
}
