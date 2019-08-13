#include <iostream>
#include <fstream>
#include "Tokenizer.h"
#include "TokenAccessor.h"
#include "Parser.h"


int main() {
    std::cout << "Hello, World!" << std::endl;

    char const * file_name = "/Users/dtrukhin/Projects/parser/input.txt";
    auto * tokenizer = new Tokenizer(file_name, new KeywordMapper());
    tokenizer->parse();

    auto accessor = new TokenAccessor(tokenizer->tokens);
    auto parser = new Parser(accessor);
    parser->parse();


    if (false) {
        TokenEnumerator t;
        int *p_int;
        float *p_float;
        char *p_char;
        for (auto &token : tokenizer->tokens) {
            t = std::get<0>(token);
            switch (t) {
                case T_IDENTIFIER:
                    p_char = (char *) std::get<1>(token);
                    std::cout << "Identifier: " << p_char << std::endl;
                    break;
                case T_STRING:
                    p_char = (char *) std::get<1>(token);
                    std::cout << "String: \"" << p_char << "\"" << std::endl;
                    break;
                case T_INTEGER:
                    p_int = (int *) std::get<1>(token);
                    std::cout << "Integer: " << *p_int << std::endl;
                    break;
                case T_FLOAT:
                    p_float = (float *) std::get<1>(token);
                    std::cout << "Float: " << *p_float << std::endl;
                    break;
                case T_ASSIGNMENT:
                    std::cout << "Operator: =" << std::endl;
                    break;
                case T_EQUALITY:
                    std::cout << "Operator: ==" << std::endl;
                    break;
                case T_TERMINATOR:
                    std::cout << "Syntax: ;" << std::endl;
                    break;
                case T_COMMA:
                    std::cout << "Syntax: ," << std::endl;
                    break;
                case T_CURLY_OPEN:
                    std::cout << "Syntax: {" << std::endl;
                    break;
                case T_CURLY_CLOSE:
                    std::cout << "Syntax: }" << std::endl;
                    break;
                case T_PAREN_LEFT:
                    std::cout << "Syntax: (" << std::endl;
                    break;
                case T_PAREN_RIGHT:
                    std::cout << "Syntax: )" << std::endl;
                    break;
                case T_ADD:
                    std::cout << "Operator: +" << std::endl;
                    break;
                case T_SUBTRACT:
                    std::cout << "Operator: -" << std::endl;
                    break;
                case T_MULTIPLY:
                    std::cout << "Operator: *" << std::endl;
                    break;
                case T_DIVIDE:
                    std::cout << "Operator: /" << std::endl;
                    break;
                case T_GT:
                    std::cout << "Operator: >" << std::endl;
                    break;
                case T_GTE:
                    std::cout << "Operator: >=" << std::endl;
                    break;
                case T_LT:
                    std::cout << "Operator: <" << std::endl;
                    break;
                case T_LTE:
                    std::cout << "Operator: <=" << std::endl;
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
                case T_KW_FOR:
                    std::cout << "Keyword: for" << std::endl;
                    break;
                case T_KW_IF:
                    std::cout << "Keyword: if" << std::endl;
                    break;
                case T_KW_ELSE:
                    std::cout << "Keyword: else" << std::endl;
                    break;
                case T_KW_PRINT:
                    std::cout << "Keyword: print" << std::endl;
                    break;
            }
        }
    }

    return 0;
}
