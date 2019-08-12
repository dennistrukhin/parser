//
// Created by Dennis Trukhin on 2019-08-09.
//

#include <iostream>
#include "Parser.h"
#include "TokenAccessor.h"

RValue *Parser::process_equality() {
    auto left_operand = new RValue();
    left_operand->type = T_INTEGER;
    left_operand->value = (int *) std::get<1>(accessor->get());
    accessor->step();

    auto t = std::get<0>(accessor->get());
    if (t != T_EQUALITY && t != T_INEQUALITY && t != T_LT && t != T_LTE && t != T_GT && t != T_GTE) {
        return left_operand;
    }
    accessor->step();

    auto right_operand = new RValue();
    right_operand->type = T_INTEGER;
    right_operand->value = (int *) std::get<1>(accessor->get());
    accessor->step();

    auto result = new RValue();
    result->type = T_INTEGER;
    int *v = (int *) malloc(sizeof(int));
    switch (t) {
        case T_EQUALITY:
            *v = (int) (*((int *) left_operand->value) == *(int *)right_operand->value);
            result->value = (void *) v;
            break;
        case T_INEQUALITY:
            *v = (int) (*((int *) left_operand->value) != *(int *)right_operand->value);
            result->value = (void *) v;
            break;
        default:
            throw std::runtime_error("Unknown comparison operator");
    }

    return  result;
}

RValue *Parser::process_concatenation() {
    auto left_operand = new RValue();
    left_operand->type = T_STRING;
    left_operand->value = (char *) std::get<1>(accessor->get());
    accessor->step();

    auto t = std::get<0>(accessor->get());
    if (t != T_ADD) {
        return left_operand;
    }
    accessor->step();
    auto right_operand = process_concatenation();

    int left_operand_length = 0;
    int right_operand_length = 0;
    while (((char *)left_operand->value)[left_operand_length] != '\0') {
        left_operand_length++;
    }
    while (((char *)right_operand->value)[right_operand_length] != '\0') {
        right_operand_length++;
    }

    char * s = (char *)malloc(sizeof(char) * (1 + left_operand_length + right_operand_length));
    memcpy(s, left_operand->value, left_operand_length);
    memcpy(s + left_operand_length, right_operand->value, right_operand_length);
    s[left_operand_length + right_operand_length] = '\0';

    auto result = new RValue();
    result->type = T_STRING;
    result->value = (void *)s;

    return result;
}

RValue *Parser::process_r_value() {
    auto t = std::get<0>(accessor->get());
    if (t == T_STRING) {
        return process_concatenation();
    }
    return process_equality();
}

void Parser::process_print() {
    while (true) {
        auto t = std::get<0>(accessor->get());
        if (t != T_STRING && t != T_INTEGER && t != T_FLOAT && t != T_IDENTIFIER) {
            throw std::runtime_error("Expected STRING or IDENTIFIER or EXPRESSION");
        }
        auto r_value = process_r_value();

        switch (r_value->type) {
            case T_INTEGER:
                std::cout << *((int *) r_value->value);
                break;
            case T_STRING:
                std::cout << (char *) r_value->value;
                break;
            default:
                throw std::runtime_error("Unknown R_VALUE type");
        }

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
