//
// Created by Dennis Trukhin on 2019-08-09.
//

#include <iostream>
#include "TokenAccessor.h"

TokenAccessor::TokenAccessor(const std::vector<std::tuple<TokenEnumerator, void *>> &t) {
    tokens = t;
}

std::tuple<TokenEnumerator, void *> TokenAccessor::get() {
    return tokens.at(p);
}

std::tuple<TokenEnumerator, void *> TokenAccessor::get(int marker) {
    std::cout << "<(" << marker << ") TOKEN: " << std::get<0>(tokens.at(p)) << " at position " << p << ">" << std::endl;
    return tokens.at(p);
}

void TokenAccessor::step() {
    ++p;
}
