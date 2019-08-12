//
// Created by Dennis Trukhin on 2019-08-09.
//

#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H


#include "TokenAccessor.h"
#include "RValue.h"

class Parser {
public:
    explicit Parser(TokenAccessor * a);

    void process_print();
    void process_statement();
    void process_statement_block();
    RValue * process_r_value();
    RValue * process_concatenation();
    RValue * process_equality();
    void parse();

private:
    TokenAccessor * accessor;
//    TokenEnumerator r_value_type;
//    void * r_value;
};


#endif //PARSER_PARSER_H
