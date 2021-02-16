//
// Created by jordi on 16/02/2021.
//

#ifndef NORMA_MACHINE_H
#define NORMA_MACHINE_H

#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

class Machine {
public:
    // Create a new machine with 2 registers X and Y
    Machine();
    // Set the input in the machine
    void input(uint64_t inputValue);
    // Increment 1 in the register, creating it if not exists
    void inc(string regName);
    // Decrement 1 in the register (min 0), creating it if not exists
    void dec(string regName);
    // Test if the register is 0, returning true if it not exists
    bool isZero(string regName);
    uint64_t finalResult();


private:
    vector<tuple<string,uint64_t>> registers;
    tuple<string, uint64_t> *findReg(string regName);
};


#endif //NORMA_MACHINE_H
