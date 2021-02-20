//
// Created by pujol on 20/02/2021.
//

#include <sstream>
#include <fstream>
#include "Compiler.h"

Compiler::Compiler() {}

Intepreter *Compiler::ReadFile(string fileName) {
    auto subrotines = vector<Subrotine*>();
    auto mainOperationList = vector<ParsedOperation *>();

    //Open file
    ifstream file(fileName);
    //Read lines
    string line;
    while(!file.eof()){
        getline(file, line);

        line = formatLine(line);
        if (line.empty())
            continue;

        if ((line.find("OPERATION") != string::npos || line.find("TEST") != string::npos ) &&
            line.find_first_of('{') != string::npos)
        {
            auto tokens = splitLine(line);
            auto subrotine = new Subrotine(tokens[1], tokens[2]);
            subrotines.push_back(subrotine);
            auto status = readSubrotine(&file, subrotine);
            if (status == -1) {
                file.close();
                return nullptr;
            }
        }

        if (line.find("MAIN") != string::npos && line.find_first_of('{') != string::npos) {
            mainOperationList = readMain(&file);
            if (mainOperationList.empty()) {
                file.close();
                return nullptr;
            }
        }


    }
    file.close();
    return new Intepreter(mainOperationList, subrotines);
}

vector<ParsedOperation*> Compiler::readMain(ifstream *file) {
    auto mainOperationList =  vector<ParsedOperation*>();
    while(!file->eof()) {
        string line;
        getline(*file, line);

        line = formatLine(line);

        if (line == "}") {
            return mainOperationList;
        }

        auto parsedLine = parseLine(line);

        if (parsedLine== nullptr) {
            return vector<ParsedOperation*>();
        }

        mainOperationList.push_back(parsedLine);
    }
}

int Compiler::readSubrotine(ifstream *file, Subrotine* subrotine) {
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


ParsedOperation *Compiler::parseLine(string line) {

    auto tokens = splitLine(line);

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

vector<string> Compiler::splitLine(const string& line) {
    vector<string> tokens;
    istringstream tokenizer { line };
    string token;

    // separa as string por espaço e coloca no vetor destino
    while (tokenizer >> token)
        tokens.push_back(token);

    return tokens;
}

string Compiler::formatLineId(const string &idToken) {
    if(int end = idToken.find_last_of(':')!=string::npos)
        return idToken.substr(0,idToken.size()-1);
    else return idToken;
}

OperationTypes Compiler::formatlineType(const string &lineTypeToken) {
    if (lineTypeToken=="DO")
        return OPERATOR;
    else if (lineTypeToken=="IF")
        return TEST;
    else return ERROR;
}

Operations Compiler::formatOperation(const string &operationToken) {
    if(operationToken=="INC")
        return INC;

    else if(operationToken=="DEC")
        return DEC;

    else if(operationToken=="ZERO")
        return ZERO;

    else return FUNC;

}

string Compiler::formatNextLine(const string &firstLineId, const string &secondLineId) {
    return firstLineId+","+secondLineId;
}


string Compiler::formatLine(string line) {
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

string& Compiler::ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

string& Compiler::rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

string& Compiler::trim(std::string& str)
{
    const std::string& chars = "\t\n\v\f\r ";
    return ltrim(rtrim(str, chars), chars);
}