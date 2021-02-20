//
// Created by pujol on 20/02/2021.
//

#ifndef NORMA_PARSEDOPERATION_H
#define NORMA_PARSEDOPERATION_H

#include "../Enums/Operations.h"
#include "../Enums/OperationTypes.h"
#include <iostream>

using namespace std;

class ParsedOperation {
public:
    string lineId;
    OperationTypes lineType;
    Operations operation;
    string regName;
    string nextLine;

    ParsedOperation(string lineId, OperationTypes lineType, Operations operation, string regName, string nextLine);
};


#endif //NORMA_PARSEDOPERATION_H
