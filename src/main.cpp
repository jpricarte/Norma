#include <iostream>
#include "Intepreter/Intepreter.h"
#include "Compiler/Compiler.h"

using namespace std;

int main(int argc, char** argv) {
    if(argc !=3) {
        cout << "one file and input needed" << endl;
        return 1;
    }

    auto *compiler = new Compiler();

    auto *intepreter = compiler->ReadFile(argv[1]);

    if(intepreter == nullptr) return 2;

    intepreter->setInput(atoi(argv[2]));
    cout << intepreter->execute() << endl;

    return 0;
}
