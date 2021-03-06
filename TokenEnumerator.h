//
// Created by Dennis Trukhin on 2019-08-09.
//

#ifndef PARSER_TOKENENUMERATOR_H
#define PARSER_TOKENENUMERATOR_H

enum TokenEnumerator {
    T_NONE,
    T_VOID,
    T_IDENTIFIER, T_STRING, T_INTEGER, T_FLOAT,
    T_CURLY_OPEN, T_CURLY_CLOSE, T_PAREN_LEFT, T_PAREN_RIGHT,
    T_COMMA, T_TERMINATOR,
    T_EQUALITY, T_INEQUALITY, T_ASSIGNMENT, T_GT, T_LT, T_GTE, T_LTE,
    T_ADD, T_SUBTRACT, T_MULTIPLY, T_DIVIDE,
    T_KW_PROGRAM, T_KW_INTEGER, T_KW_FLOAT, T_KW_STRING, T_KW_PRINT,
    T_KW_FOR, T_KW_IF, T_KW_ELSE,
};

#endif //PARSER_TOKENENUMERATOR_H
