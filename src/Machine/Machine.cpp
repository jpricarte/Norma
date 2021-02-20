//
// Created by jordi on 16/02/2021.
//

#include <tuple>
#include "Machine.h"

using namespace std;
Machine::Machine() {
    registers = vector<tuple<string, uint64_t>>();
    registers.emplace_back("X", 0);
    registers.emplace_back("Y", 0);
}

void Machine::inc(string regName) {
    auto *reg = findReg(regName);
    if (reg != nullptr) {
        get<1>(*reg)++;
    }
    else {
        registers.emplace_back(regName, 1);
    }
}

void Machine::dec(string regName) {
    auto *reg = findReg(regName);
    if (reg != nullptr) {
        auto actualValue = get<1>(*reg);
        get<1>(*reg) = (actualValue == 0) ? 0 : actualValue-1;
    }
}

bool Machine::isZero(string regName) {
    auto *reg = findReg(regName);
    if (reg != nullptr) {
        if (get<1>(*reg) != 0)
            return false;
    }
    return true;
}

uint64_t Machine::finalResult() {
    return get<1>(registers[1]);
}

tuple<string, uint64_t> *Machine::findReg(string regName) {

    for(auto i=0; i<registers.size(); i++) {
        if(get<0>(registers[i]) == regName) {
            return &registers[i];
        }
    }

    return nullptr;
}

void Machine::input(uint64_t inputValue) {
    get<1>(registers[0]) = inputValue;
}
