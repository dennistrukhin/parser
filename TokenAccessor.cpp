//
// Created by Dennis Trukhin on 2019-08-09.
//

#include "TokenAccessor.h"

TokenAccessor::TokenAccessor(const std::vector<std::tuple<TokenEnumerator, void *>> &t) {
    tokens = t;
}

std::tuple<TokenEnumerator, void *> TokenAccessor::get() {
    return tokens.at(p);
}

void TokenAccessor::step() {
    ++p;
}
