//
// Created by Трухин Денис on 2019-08-05.
//

#ifndef PARSER_TOKENIZER_H
#define PARSER_TOKENIZER_H


#include <string>
#include <list>
#include <map>
#include <vector>
#include "TokenEnumerator.h"
#include "KeywordMapper.h"


class Tokenizer {
    enum TokenizerState {
        S_READY, S_IDENTIFIER, S_STRING, S_NUMBER_INT, S_NUMBER_FLOAT
    };

public:
    std::vector<std::tuple<TokenEnumerator, void *>> tokens;

    Tokenizer(char const *file_name, KeywordMapper * kw);

    void parse();

private:
    std::ifstream input_stream;
    KeywordMapper * keyword_mapper;
    char c = 0;
    TokenizerState state = S_READY;
    char sequence[255]{};
    int sequencePosition = 0;
    int n = 0;
    double f = 0.0;
    int f_digits = 0;

    void add_token_identifier(char s[255]);

    void add_token_keyword(TokenEnumerator t);

    void add_token_string(char const s[255]);

    void add_token_int(int number);

    void add_token_float(double number);

    bool get_char();

    void clear_buffers();
};


#endif //PARSER_TOKENIZER_H
