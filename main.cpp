#include <iostream>
#include "Classes/Intepreter.h"

int main(int argc, char** argv) {
    if(argc !=3) {
        cout << "one file and input needed" << endl;
        return 1;
    }

    auto *intepreter = new Intepreter();

    int i = intepreter->ReadFile(argv[1]);
    if(i < 0) return 2;

    intepreter->setInput(atoi(argv[2]));
    cout << intepreter->execute() << endl;

    return 0;
}
