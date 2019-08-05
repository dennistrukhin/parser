#include <iostream>
#include <fstream>
#include <cmath>
#include "Tokenizer.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    char const * file_name = "/Users/dennis/Projects/parser/input.txt";
    auto * tokenizer = new Tokenizer(file_name);
    tokenizer->parse();

    TokenEnumerator t;
    int * p_int;
    float * p_float;
    for (auto & token : tokenizer->tokens) {
        t = std::get<0>(token);
        switch (t) {
            case T_INTEGER:
                p_int = (int *)std::get<1>(token);
                std::cout << std::get<0>(token) << ' ' << *p_int << std::endl;
                break;
            case T_FLOAT:
                p_float = (float *)std::get<1>(token);
                std::cout << std::get<0>(token) << ' ' << *p_float << std::endl;
                break;
        }
    }

    return 0;
}
