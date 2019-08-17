#include <iostream>
#include <fstream>
#include "Tokenizer.h"
#include "TokenAccessor.h"
#include "Parser.h"


int main() {
    std::cout << "Hello, World!" << std::endl;

    char const * file_name = "/Users/dennis/Projects/parser/input.txt";
    auto * tokenizer = new Tokenizer(file_name, new KeywordMapper());
    tokenizer->parse();

    auto accessor = new TokenAccessor(tokenizer->tokens);
    auto parser = new Parser(accessor);
    parser->parse();

    return 0;
}
