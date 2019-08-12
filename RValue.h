//
// Created by Трухин Денис on 2019-08-12.
//

#ifndef PARSER_RVALUE_H
#define PARSER_RVALUE_H


#include "TokenEnumerator.h"

class RValue {
public:
    TokenEnumerator type = T_VOID;
    void * value = nullptr;
};


#endif //PARSER_RVALUE_H
