#include <iostream>
#include <fstream>
#include <cmath>
#include "Tokenizer.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    char const * file_name = "/Users/dtrukhin/Projects/parser/input.txt";
    auto * tokenizer = new Tokenizer(file_name);
    tokenizer->parse();

    return 0;
}
