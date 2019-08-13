//
// Created by Трухин Денис on 2019-08-13.
//

#ifndef PARSER_KEYWORDMAPPER_H
#define PARSER_KEYWORDMAPPER_H


#include <map>
#include "TokenEnumerator.h"

struct cmp_str
{
    bool operator()(char const *a, char const *b) const
    {
        return std::strcmp(a, b) < 0;
    }
};

class KeywordMapper {
public:
    TokenEnumerator get_token_by_keyword(char const *);
    const char * CP_FOR = "for";
    const char * CP_PRINT = "print";
    const char * CP_INTEGER = "int";
    const char * CP_FLOAT = "float";
    const char * CP_STRING = "string";
    const char * CP_IF = "if";
    const char * CP_ELSE = "else";
    const char * CP_PROGRAM = "program";

    KeywordMapper();
private:

    std::map<const char *, TokenEnumerator, cmp_str> keyword_mapper;
};


#endif //PARSER_KEYWORDMAPPER_H
