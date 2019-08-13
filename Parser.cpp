//
// Created by Dennis Trukhin on 2019-08-09.
//

#include <iostream>
#include "Parser.h"
#include "TokenAccessor.h"

RValue *Parser::process_comparison() {
    auto left_operand = process_sum();

    auto t = std::get<0>(accessor->get());
    if (t != T_EQUALITY && t != T_INEQUALITY && t != T_LT && t != T_LTE && t != T_GT && t != T_GTE) {
        return left_operand;
    }
    accessor->step();

    auto right_operand = process_sum();

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
        case T_LT:
            *v = (int) (*((int *) left_operand->value) < *(int *)right_operand->value);
            result->value = (void *) v;
            break;
        case T_LTE:
            *v = (int) (*((int *) left_operand->value) <= *(int *)right_operand->value);
            result->value = (void *) v;
            break;
        case T_GT:
            *v = (int) (*((int *) left_operand->value) > *(int *)right_operand->value);
            result->value = (void *) v;
            break;
        case T_GTE:
            *v = (int) (*((int *) left_operand->value) >= *(int *)right_operand->value);
            result->value = (void *) v;
            break;
        default:
            throw std::runtime_error("Unknown comparison operator");
    }

    return  result;
}

RValue *Parser::process_factor() {
    auto t = std::get<0>(accessor->get());
    int *buff = (int *) malloc(sizeof(int));
    RValue * result;
    if (t == T_PAREN_LEFT) {
        accessor->step();
        result = process_comparison();
    } else {
        result = new RValue();
        result->type = T_INTEGER;
        result->value = (int *) std::get<1>(accessor->get());
    }

    while (true) {
        accessor->step();

        t = std::get<0>(accessor->get());
        if (t != T_MULTIPLY && t != T_DIVIDE) {
            return result;
        }

        accessor->step();
        auto t1 = std::get<0>(accessor->get());
        RValue * right_operand;
        if (t1 == T_PAREN_LEFT) {
            accessor->step();
            right_operand = process_comparison();
        } else {
            right_operand = new RValue();
            right_operand->type = T_INTEGER;
            right_operand->value = (int *) std::get<1>(accessor->get());
        }
        switch (t) {
            case T_MULTIPLY:
                *buff = *(int *)result->value * *(int *)right_operand->value;
                result->value = (void *)buff;
                break;
            default:
                throw std::runtime_error("Unknown multiplication operator");
        }
    }
}

RValue *Parser::process_sum() {
    int * buff = (int *)malloc(sizeof(int));
    auto result = process_factor();

    while (true) {

        auto t = std::get<0>(accessor->get());
        if (t != T_ADD && t != T_SUBTRACT) {
            return result;
        }

        accessor->step();
        auto right_operand = process_factor();
        auto t1 = std::get<0>(accessor->get());
        switch (t) {
            case T_ADD:
                *buff = *(int *)result->value + *(int *)right_operand->value;
                result->value = (void *)buff;
                break;
            case T_SUBTRACT:
                *buff = *(int *)result->value - *(int *)right_operand->value;
                result->value = (void *)buff;
                break;
            default:
                throw std::runtime_error("Unknown summation operator");
        }
    }
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
    return process_comparison();
}

void Parser::process_print() {
    while (true) {
        auto t = std::get<0>(accessor->get());
        if (t != T_STRING && t != T_INTEGER && t != T_FLOAT && t != T_IDENTIFIER && t != T_PAREN_LEFT) {
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
