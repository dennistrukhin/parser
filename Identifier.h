//
// Created by Dennis Trukhin on 2019-08-16.
//

#ifndef PARSER_IDENTIFIER_H
#define PARSER_IDENTIFIER_H


#include "TokenEnumerator.h"

class Identifier {
public:
    TokenEnumerator type;
    void * value;

    Identifier(TokenEnumerator, void *);
};


#endif //PARSER_IDENTIFIER_H
