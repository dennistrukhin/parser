//
// Created by Трухин Денис on 2019-08-05.
//

#ifndef PARSER_TOKENIZER_H
#define PARSER_TOKENIZER_H


#include <string>
#include <list>
#include <map>
#include <vector>

enum TokenEnumerator {
    T_IDENTIFIER, T_STRING, T_INTEGER, T_FLOAT,
    T_CURLY_OPEN, T_CURLY_CLOSE,
    T_COMMA, T_EQUALITY, T_ASSIGNMENT
};

class Tokenizer {
    enum TokenizerState {
        S_READY, S_IDENTIFIER, S_STRING, S_NUMBER_INT, S_NUMBER_FLOAT, S_EQUALITY
    };
    enum Keyword {
        T_INT
    };

public:
    std::vector<std::tuple<TokenEnumerator, void *>> tokens;

    explicit Tokenizer(char const *file_name);

    void parse();

private:
    std::ifstream input_stream;
    char c = 0;
    TokenizerState state = S_READY;
    char sequence[255]{};
    int sequencePosition = 0;
    int n = 0;
    double f = 0.0;
    int f_digits = 0;

    void add_token_keyword(Keyword k, const std::string &s);

    void add_token_operator(TokenEnumerator t, const std::string &s);

    void add_token_identifier(char s[255]);

    void add_token_keyword(Keyword k);

    void add_token_string(char s[255]);

    void add_token_control(TokenEnumerator type, const std::string &s);

    void add_token_control(TokenEnumerator type);

    void add_token_int(int number);

    void add_token_float(double number);

    bool get_char();

    void clear_buffers();
};


#endif //PARSER_TOKENIZER_H
