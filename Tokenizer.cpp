//
// Created by Трухин Денис on 2019-08-05.
//

#include <iostream>
#include <fstream>
#include <cmath>
#include "Tokenizer.h"

Tokenizer::Tokenizer(char const * file_name) {
    input_stream.open("/Users/dtrukhin/Projects/parser/input.txt");
}

bool Tokenizer::get_char() {
    if (input_stream.eof()) {
        return false;
    }
    input_stream.get(c);

    return true;
}

void Tokenizer::add_token_keyword(Keyword k, const std::string& s) {
    std::cout << "Keyword: " << k << ", " << s << std::endl;
}

void Tokenizer::add_token_operator(Token t, const std::string& s) {
    std::cout << "Operator: " << t << ", " << s << std::endl;
}

void Tokenizer::add_token_identifier(char s[255]) {
    if (strcmp(s, "int") == 0) {
        add_token_keyword(T_INT, s);
        return;
    }
    std::cout << "Identifier: " << s << std::endl;
}

void Tokenizer::add_token_keyword(Keyword k) {
    std::cout << "Keyword: " << k << std::endl;
}

void Tokenizer::add_token_string(char s[255]) {
    std::cout << "String: " << s << std::endl;
}

void Tokenizer::add_token_control(Token type, const std::string& s) {
    std::cout << "Control: <" << type << " " << s << ">" << std::endl;
}

void Tokenizer::add_token_control(Token type) {
    std::cout << "Control: <" << type << ">" << std::endl;
}

void Tokenizer::add_token_int(int number) {
    std::cout << "Integer: " << number << std::endl;
}

void Tokenizer::add_token_float(double number) {
    std::cout << "Float: " << number << std::endl;
}

void Tokenizer::clear_buffers() {
    sequencePosition = 0;
    for (int i = 0; i <= 255; i++) {
        sequence[i] = 0;
    }
    n = 0;
    f = 0.0;
    f_digits = 0;
}

void Tokenizer::parse() {
    if (!get_char()) return;

    while (true) {
        if (c == '"') {
            if (state == S_READY) {
                state = S_STRING;
                if (!get_char()) break;
                continue;
            }
            if (state == S_STRING) {
                add_token_string(sequence);
                state = S_READY;
                if (!get_char()) break;
                continue;
            }
        }
        if (state == S_STRING) {
            sequence[sequencePosition++] = c;
            if (!get_char()) break;
            continue;
        }
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_') {
            if (state == S_READY) {
                sequence[sequencePosition++] = c;
                state = S_IDENTIFIER;
                if (!get_char()) break;
                continue;
            }
            // otherwise it's identifier
            sequence[sequencePosition++] = c;
            if (!get_char()) break;
            continue;
        }
        if (c >= '0' && c <= '9') {
            if (state == S_IDENTIFIER) {
                sequence[sequencePosition++] = c;
                if (!get_char()) break;
                continue;
            }
            if (state == S_READY || state == S_NUMBER_INT) {
                state = S_NUMBER_INT;
                n = n * 10 + c - '0';
                if (!get_char()) break;
                continue;
            }
            if (state == S_NUMBER_FLOAT) {
                f = f + (c - '0') * pow(10, --f_digits);
                if (!get_char()) break;
                continue;
            }
        }
        if (c == '.') {
            if (state == S_NUMBER_INT) {
                state = S_NUMBER_FLOAT;
                if (!get_char()) break;
                continue;
            }
            throw std::runtime_error("Unexpected dot symbol. Terminating.");
        }
        // Не буковка и не чиселко
        if (state == S_IDENTIFIER) {
            add_token_identifier(sequence);
        }
        if (state == S_NUMBER_INT) {
            add_token_int(n);
        }
        if (state == S_NUMBER_FLOAT) {
            add_token_float(n + f);
        }

        if (c == '{') {
            add_token_control(T_CURLY_OPEN, "{");
            if (!get_char()) break;
            state = S_READY;
            continue;
        }
        if (c == '}') {
            add_token_control(T_CURLY_CLOSE, "}");
            if (!get_char()) break;
            state = S_READY;
            continue;
        }
        if (c == ',') {
            add_token_control(T_COMMA, ",");
            if (!get_char()) break;
            state = S_READY;
            continue;
        }
        if (c == '=') {
            if (!get_char()) break;
            if (c == '=') {
                add_token_operator(T_EQUALITY, "==");
                if (!get_char()) break;
            }
            add_token_control(T_ASSIGNMENT, "=");
            clear_buffers();
            continue;
        }

        if (!get_char()) break;

        clear_buffers();
        state = S_READY;
    }

    input_stream.close();
}