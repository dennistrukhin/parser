#include <iostream>
#include <fstream>
#include <cmath>
#include "Tokenizer.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    char const * file_name = "/Users/dtrukhin/Projects/parser/input.txt";
    auto * tokenizer = new Tokenizer(file_name);
    tokenizer->parse();

    TokenEnumerator t;
    int * p_int;
    float * p_float;
    char * p_char;
    for (auto & token : tokenizer->tokens) {
        t = std::get<0>(token);
        switch (t) {
            case T_IDENTIFIER:
                p_char = (char *)std::get<1>(token);
                std::cout << "Identifier: " << p_char << std::endl;
                break;
            case T_STRING:
                p_char = (char *)std::get<1>(token);
                std::cout << "String: \"" << p_char << "\"" << std::endl;
                break;
            case T_INTEGER:
                p_int = (int *)std::get<1>(token);
                std::cout << "Integer: " << *p_int << std::endl;
                break;
            case T_FLOAT:
                p_float = (float *)std::get<1>(token);
                std::cout << "Float: " << *p_float << std::endl;
                break;


            case T_KW_INTEGER:
                std::cout << "Keyword: int" << std::endl;
                break;
            case T_KW_FLOAT:
                std::cout << "Keyword: float" << std::endl;
                break;
            case T_KW_STRING:
                std::cout << "Keyword: string" << std::endl;
                break;
            case T_KW_PROGRAM:
                std::cout << "Keyword: program" << std::endl;
                break;
            case T_KW_ELSE:
                std::cout << "Keyword: else" << std::endl;
                break;
        }
    }

    return 0;
}
