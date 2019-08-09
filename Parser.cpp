//
// Created by Dennis Trukhin on 2019-08-09.
//

#include <iostream>
#include "Parser.h"
#include "TokenAccessor.h"

void Parser::process_print() {
    char * p_char;
    while (true) {
        auto t = std::get<0>(accessor->get());
        if (t == T_STRING) {
            p_char = (char *) std::get<1>(accessor->get());
            std::cout << p_char;
        } else if (t == T_IDENTIFIER) {
            std::cout << "CAN'T PRINT IDENTIFIERS";
        } else if (t == T_INTEGER) {
            std::cout << "CAN'T PRINT IDENTIFIERS";
        } else {
            throw std::runtime_error("Expected STRING or IDENTIFIER or EXPRESSION");
        }

        accessor->step();

        if (std::get<0>(accessor->get()) == T_COMMA) {
            accessor->step();
            continue;
        }

        if (std::get<0>(accessor->get()) == T_TERMINATOR) {
            std::cout << std::endl;
            return;
        }

        throw std::runtime_error("Expected ; or ,");
    }
}

void Parser::process_statement() {
    while (true) {
        if (std::get<0>(accessor->get()) == T_KW_PRINT) {
            accessor->step();
            process_print();
            accessor->step();
            return;
        }
        if (std::get<0>(accessor->get()) == T_TERMINATOR) {
            std::cout << "EXPRESSION PROCESSED" << std::endl;
            accessor->step();
            return;
        }
        accessor->step();
    }
}

void Parser::process_statement_block() {
    while (std::get<0>(accessor->get()) != T_CURLY_CLOSE) {
        process_statement();
    }
}

Parser::Parser(TokenAccessor *a) {
    accessor = a;
}

void Parser::parse() {
    if (std::get<0>(accessor->get()) == T_KW_PROGRAM) {
        accessor->step();
        if (std::get<0>(accessor->get()) != T_CURLY_OPEN) {
            throw std::runtime_error("Expected {");
        }
        accessor->step();
        process_statement_block();
        std::cout << "Execution finished with code 0" << std::endl;
    }
}
