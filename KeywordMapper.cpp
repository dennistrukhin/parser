//
// Created by Трухин Денис on 2019-08-13.
//

#include <map>
#include <iostream>
#include "KeywordMapper.h"

TokenEnumerator KeywordMapper::get_token_by_keyword(char const *s) {
    if (keyword_mapper.count(s)) {
        return keyword_mapper.find(s)->second;
    }

    return T_NONE;
}

KeywordMapper::KeywordMapper() {
    keyword_mapper = {
            {CP_PRINT, T_KW_PRINT},
            {CP_FOR, T_KW_FOR},
            {CP_INTEGER, T_KW_INTEGER},
            {CP_FLOAT, T_KW_FLOAT},
            {CP_STRING, T_KW_STRING},
            {CP_IF, T_KW_IF},
            {CP_ELSE, T_KW_ELSE},
            {CP_PROGRAM, T_KW_PROGRAM},
    };
}
