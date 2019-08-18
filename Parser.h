//
// Created by Dennis Trukhin on 2019-08-09.
//

#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H


#include <map>
#include "TokenAccessor.h"
#include "RValue.h"
#include "StringComparator.h"
#include "Identifier.h"

class Parser {
public:
    explicit Parser(TokenAccessor *a);

    void parse();

private:
    TokenAccessor *accessor;
    std::map<char *, TokenEnumerator, StringComparator> keyword_mapper;
    std::map<char const *, Identifier *, StringComparator> identifiers;

    RValue *process_sum();

    RValue *process_factor();

    RValue *process_r_value();

    RValue *process_concatenation();

    RValue *process_comparison();

    void process_for_block();

    void process_int_declaration();

    void process_string_declaration();

    void process_print();

    void process_statement();

    void process_statement_block();

    void set_identifier(char const *name, int value);

    void set_identifier(char const *name, const char *value);
};


#endif //PARSER_PARSER_H
