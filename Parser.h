//
// Created by Dennis Trukhin on 2019-08-09.
//

#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H


#include <map>
#include "TokenAccessor.h"
#include "RValue.h"
#include "StringComparator.h"

class Parser {
public:
    explicit Parser(TokenAccessor *a);

    void parse();

private:
    TokenAccessor *accessor;
    std::map<char *, TokenEnumerator, StringComparator> keyword_mapper;

    RValue *process_sum();

    RValue *process_factor();

    RValue *process_r_value();

    RValue *process_concatenation();

    RValue *process_comparison();

    void process_int_declaration();
    void process_print();

    void process_statement();

    void process_statement_block();
};


#endif //PARSER_PARSER_H
