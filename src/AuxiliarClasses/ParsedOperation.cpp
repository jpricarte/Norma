//
// Created by pujol on 20/02/2021.
//

#include "ParsedOperation.h"

ParsedOperation::ParsedOperation(string lineId, OperationTypes lineType, Operations operation, string regName,
                                 string nextLine) {
    this->lineId=std::move(lineId);
    this->operation=operation;
    this->nextLine=std::move(nextLine);
    this->regName=std::move(regName);
    this->lineType=lineType;
}