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
            *v = (int) (*((int *) left_operand->value) == *(int *) right_operand->value);
            result->value = (void *) v;
            break;
        case T_INEQUALITY:
            *v = (int) (*((int *) left_operand->value) != *(int *) right_operand->value);
            result->value = (void *) v;
            break;
        case T_LT:
            *v = (int) (*((int *) left_operand->value) < *(int *) right_operand->value);
            result->value = (void *) v;
            break;
        case T_LTE:
            *v = (int) (*((int *) left_operand->value) <= *(int *) right_operand->value);
            result->value = (void *) v;
            break;
        case T_GT:
            *v = (int) (*((int *) left_operand->value) > *(int *) right_operand->value);
            result->value = (void *) v;
            break;
        case T_GTE:
            *v = (int) (*((int *) left_operand->value) >= *(int *) right_operand->value);
            result->value = (void *) v;
            break;
        default:
            throw std::runtime_error("Unknown comparison operator");
    }

    return result;
}

RValue *Parser::process_factor() {
    auto t = std::get<0>(accessor->get());
    int *buff = (int *) malloc(sizeof(int));
    RValue *result;
    if (t == T_PAREN_LEFT) {
        accessor->step();
        result = process_comparison();
    } else if (t == T_IDENTIFIER) {
        char const *name = (char *) std::get<1>(accessor->get());
        result = new RValue();
        result->type = T_INTEGER;
        result->value = (int *) (identifiers.find(name)->second->value);
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
        RValue *right_operand;
        if (t1 == T_PAREN_LEFT) {
            accessor->step();
            right_operand = process_comparison();
        } else if (t1 == T_IDENTIFIER) {
            char const *name = (char *) std::get<1>(accessor->get());
            std::cout << "identifier in expression: " << name << std::endl;
            right_operand = new RValue();
            right_operand->type = T_INTEGER;
            right_operand->value = (int *) (identifiers.find(name)->second->value);
        } else {
            right_operand = new RValue();
            right_operand->type = T_INTEGER;
            right_operand->value = (int *) std::get<1>(accessor->get());
        }
        if (t == T_MULTIPLY) {
            *buff = *(int *) result->value * *(int *) right_operand->value;
            result->value = (void *) buff;
        } else {
            throw std::runtime_error("Unknown multiplication operator");
        }
    }
}

RValue *Parser::process_sum() {
    int *buff = (int *) malloc(sizeof(int));
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
                *buff = *(int *) result->value + *(int *) right_operand->value;
                result->value = (void *) buff;
                break;
            case T_SUBTRACT:
                *buff = *(int *) result->value - *(int *) right_operand->value;
                result->value = (void *) buff;
                break;
            default:
                throw std::runtime_error("Unknown summation operator");
        }
    }
}

RValue *Parser::process_concatenation() {
    auto t = std::get<0>(accessor->get());
    RValue *left_operand;
    if (t == T_STRING) {
        left_operand = new RValue();
        left_operand->type = T_STRING;
        left_operand->value = (char *) std::get<1>(accessor->get());
        accessor->step();
    } else if (t == T_IDENTIFIER) {
        auto name = (char *) std::get<1>(accessor->get());
        left_operand = new RValue();
        left_operand->type = T_STRING;
        left_operand->value = (char *) (identifiers.find(name)->second->value);
        accessor->step();
    } else {
        throw std::runtime_error("Expected STRING or IDENTIFIER");
    }

    t = std::get<0>(accessor->get());
    if (t != T_ADD) {
        return left_operand;
    }
    accessor->step();
    auto right_operand = process_concatenation();

    int left_operand_length = 0;
    int right_operand_length = 0;
    while (((char *) left_operand->value)[left_operand_length] != '\0') {
        left_operand_length++;
    }
    while (((char *) right_operand->value)[right_operand_length] != '\0') {
        right_operand_length++;
    }

    char *s = (char *) malloc(sizeof(char) * (1 + left_operand_length + right_operand_length));
    memcpy(s, left_operand->value, left_operand_length);
    memcpy(s + left_operand_length, right_operand->value, right_operand_length);
    s[left_operand_length + right_operand_length] = '\0';

    auto result = new RValue();
    result->type = T_STRING;
    result->value = (void *) s;

    return result;
}

RValue *Parser::process_r_value() {
    auto t = std::get<0>(accessor->get());
    if (t == T_STRING) {
        return process_concatenation();
    } else if (t == T_IDENTIFIER) {
        auto name = (char *) std::get<1>(accessor->get());
        TokenEnumerator type = (identifiers.find(name)->second->type);
        if (type == T_STRING) {
            return process_concatenation();
        }
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

void Parser::process_string_declaration() {
    char *name;
    while (true) {
        auto t = std::get<0>(accessor->get());
        if (t != T_IDENTIFIER) {
            throw std::runtime_error("Expected identifier name");
        }
        name = (char *) std::get<1>(accessor->get());
        accessor->step();
        t = std::get<0>(accessor->get());
        if (t == T_ASSIGNMENT) {
            accessor->step();
            auto v = process_concatenation();
            t = std::get<0>(accessor->get());
            set_identifier(name, (char *) v->value);
        } else if (t == T_COMMA || t == T_TERMINATOR) {
            auto s = (char *)malloc(sizeof(char));
            s[0] = '\0';
            set_identifier(name, s);
        }
        if (t == T_COMMA) {
            accessor->step();
            continue;
        }
        if (t == T_TERMINATOR) {
            accessor->step();
            return;
        }
    }
}

void Parser::process_for_block() {
    auto t = std::get<0>(accessor->get());
    if (t != T_PAREN_LEFT) {
        throw std::runtime_error("Expected (");
    }
    accessor->step();
    process_statement();
    int p_condition = accessor->get_position();
    while (t != T_TERMINATOR) {
        accessor->step();
        t = std::get<0>(accessor->get());
    }
    accessor->step();
    int p_after = accessor->get_position();

    while (t != T_PAREN_RIGHT) {
        accessor->step();
        t = std::get<0>(accessor->get());
    }
    accessor->step();
    int p_block = accessor->get_position();

    accessor->step();
    accessor->step();
    int curly_balance = 1;
    while (curly_balance > 0) {
        accessor->step();
        t = std::get<0>(accessor->get());
        if (t == T_CURLY_OPEN) {
            curly_balance++;
        }
        if (t == T_CURLY_CLOSE) {
            curly_balance--;
        }
    }

    accessor->step();
    int out_of_block = accessor->get_position();

    while (accessor->stepTo(p_condition), *(int *) process_comparison()->value > 0) {
        accessor->stepTo(p_block);
        process_statement_block();
        accessor->stepTo(p_after);
        process_statement();
    }
    accessor->stepTo(out_of_block);

}

void Parser::process_if_block() {
    auto t = std::get<0>(accessor->get());
    if (t != T_PAREN_LEFT) {
        throw std::runtime_error("Expected (");
    }
    accessor->step();
    auto v = process_comparison();
    t = std::get<0>(accessor->get());
    if (t != T_PAREN_RIGHT) {
        throw std::runtime_error("Expected )");
    }
    accessor->step();

    if (*(int *)v->value) {
        process_statement_block();
        t = std::get<0>(accessor->get());
        if (t == T_KW_ELSE) {
            accessor->step();
            accessor->step();
            int curly_balance = 1;
            while (curly_balance > 0) {
                accessor->step();
                t = std::get<0>(accessor->get());
                if (t == T_CURLY_OPEN) {
                    curly_balance++;
                }
                if (t == T_CURLY_CLOSE) {
                    curly_balance--;
                }
            }
            accessor->step();
        }
    } else {
        int curly_balance = 1;
        while (curly_balance > 0) {
            accessor->step();
            t = std::get<0>(accessor->get());
            if (t == T_CURLY_OPEN) {
                curly_balance++;
            }
            if (t == T_CURLY_CLOSE) {
                curly_balance--;
            }
        }
        accessor->step();
        t = std::get<0>(accessor->get());
        if (t == T_KW_ELSE) {
            accessor->step();
            process_statement_block();
        }
    }
}

void Parser::process_int_declaration() {
    char *name;
    while (true) {
        auto t = std::get<0>(accessor->get());
        if (t != T_IDENTIFIER) {
            throw std::runtime_error("Expected identifier name");
        }
        name = (char *) std::get<1>(accessor->get());
        accessor->step();
        t = std::get<0>(accessor->get());
        if (t == T_ASSIGNMENT) {
            accessor->step();
            auto v = process_comparison();
            t = std::get<0>(accessor->get());
            set_identifier(name, *(int *) v->value);
        } else if (t == T_COMMA || t == T_TERMINATOR) {
            set_identifier(name, 0);
        }
        if (t == T_COMMA) {
            accessor->step();
            continue;
        }
        if (t == T_TERMINATOR) {
            accessor->step();
            return;
        }
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
        if (std::get<0>(accessor->get()) == T_KW_INTEGER) {
            accessor->step();
            process_int_declaration();
            return;
        }
        if (std::get<0>(accessor->get()) == T_KW_STRING) {
            accessor->step();
            process_string_declaration();
            return;
        }
        if (std::get<0>(accessor->get()) == T_IDENTIFIER) {
            auto name = (char *) std::get<1>(accessor->get());
            accessor->step();
            if (std::get<0>(accessor->get()) != T_ASSIGNMENT) {
                throw std::runtime_error("Expected = after identifier");
            }
            accessor->step();
            if (identifiers.find(name)->second->type == T_INTEGER) {
                auto value = process_comparison();
                set_identifier(name, *(int *) value->value);
            } else {
                auto value = process_concatenation();
                set_identifier(name, (char *) value->value);
            }
            accessor->step();
            return;
        }
        if (std::get<0>(accessor->get()) == T_KW_FOR) {
            accessor->step();
            process_for_block();
            return;
        }
        if (std::get<0>(accessor->get()) == T_KW_IF) {
            accessor->step();
            process_if_block();
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
    if (std::get<0>(accessor->get()) != T_CURLY_OPEN) {
        throw std::runtime_error("Expected {");
    }
    accessor->step();
    while (std::get<0>(accessor->get()) != T_CURLY_CLOSE) {
        process_statement();
    }
    accessor->step();
}

Parser::Parser(TokenAccessor *a) {
    accessor = a;
}

void Parser::parse() {
    if (std::get<0>(accessor->get()) == T_KW_PROGRAM) {
        accessor->step();
        process_statement_block();
        std::cout << "Execution finished with code 0" << std::endl;
    }
}

void Parser::set_identifier(char const *name, int value) {
    int *v = (int *) malloc(sizeof(int));
    *v = value;

    auto it = identifiers.find(name);
    if (it != identifiers.end()) {
        it->second->value = (void *) v;
        return;
    }

    auto i = new Identifier(T_INTEGER, (void *) v);

    identifiers.insert(std::pair<char const *, Identifier *>(name, i));
}

void Parser::set_identifier(char const *name, const char *value) {
    int str_length = 0;
    while (value[str_length]) {
        str_length++;
    }
    char *v = (char *) malloc(sizeof(char) * (str_length + 1));
    memcpy(v, value, str_length);
    v[str_length] = '\0';

    auto it = identifiers.find(name);
    if (it != identifiers.end()) {
        it->second->value = (void *) v;
        return;
    }

    auto i = new Identifier(T_STRING, (void *) v);

    identifiers.insert(std::pair<char const *, Identifier *>(name, i));
}
