//
// Created by pujol on 20/02/2021.
//

#include "Subrotine.h"

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