//
// Created by Dennis Trukhin on 2019-08-09.
//

#ifndef PARSER_TOKENACCESSOR_H
#define PARSER_TOKENACCESSOR_H


#include <vector>
#include "TokenEnumerator.h"

class TokenAccessor {
private:
    int p = 0;
    std::vector<std::tuple<TokenEnumerator, void *>> tokens;
public:
    explicit TokenAccessor(const std::vector<std::tuple<TokenEnumerator, void *>> &tokens);

    std::tuple<TokenEnumerator, void *> get();
    void step();
};


#endif //PARSER_TOKENACCESSOR_H
