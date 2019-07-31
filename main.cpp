#include <iostream>
#include <fstream>

enum TokenizerState {S_READY, S_IDENTIFIER, S_STRING, S_NUMBER_INT, S_NUMBER_FLOAT};
enum Token {T_IDENTIFIER, T_KEYWORD, T_STRING, T_INTEGER, T_FLOAT,
        T_CURLY_OPEN, T_CURLY_CLOSE,
        T_COMMA};

void add_token_identifier(char s[255]) {
    std::cout << "Identifier: " << s << std::endl;
}

void add_token_string(char s[255]) {
    std::cout << "String: " << s << std::endl;
}

void add_token_control(Token type, const std::string& s) {
    std::cout << "Control: <" << type << " " << s << ">" << std::endl;
}

void add_token_control(Token type) {
    std::cout << "Control: <" << type << ">" << std::endl;
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    std::ifstream is("/Users/dtrukhin/Projects/parser/input.txt");

    char c = 0;
    TokenizerState state = S_READY;
    char sequence[255];
    int sequencePosition = 0;

    if (is.eof()) {
        return 1;
    }
    is.get(c);

    while (true) {
        if (c == '"') {
            if (state == S_READY) {
                state = S_STRING;
                if (is.eof()) break;
                is.get(c);
                continue;
            }
            if (state == S_STRING) {
                add_token_string(sequence);
                state = S_READY;
                if (is.eof()) break;
                is.get(c);
                continue;
            }
        }
        if (state == S_STRING) {
            sequence[sequencePosition++] = c;
            if (is.eof()) break;
            is.get(c);
            continue;
        }
        if (c >= 'A' && c <= 'z') {
            if (state == S_READY) {
                sequence[sequencePosition++] = c;
                state = S_IDENTIFIER;
                if (is.eof()) break;
                is.get(c);
                continue;
            }
            if (state == S_IDENTIFIER) {
                sequence[sequencePosition++] = c;
                if (is.eof()) break;
                is.get(c);
                continue;
            }
        }
        if (c >= '0' && c <= '9') {
            if (state == S_IDENTIFIER) {
                sequence[sequencePosition++] = c;
                if (is.eof()) break;
                is.get(c);
                continue;
            }
        }
        // Не буковка и не чиселко
        if (state == S_IDENTIFIER) {
            add_token_identifier(sequence);
        }

        if (c == '{') {
            add_token_control(T_CURLY_OPEN, "{");
            if (is.eof()) break;
            is.get(c);
            state = S_READY;
            continue;
        }
        if (c == '}') {
            add_token_control(T_CURLY_CLOSE, "}");
            if (is.eof()) break;
            is.get(c);
            state = S_READY;
            continue;
        }
        if (c == ',') {
            add_token_control(T_COMMA, ",");
            if (is.eof()) break;
            is.get(c);
            state = S_READY;
            continue;
        }

        if (is.eof()) break;
        is.get(c);

        sequencePosition = 0;
        for (int i = 0; i <= 255; i++) {
            sequence[i] = 0;
        }
        state = S_READY;
    }

    is.close();

    return 0;
}
