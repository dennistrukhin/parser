//
// Created by Трухин Денис on 2019-08-14.
//

#include <cstring>
#include "StringComparator.h"

bool StringComparator::operator()(char const *a, char const *b) const {
    return std::strcmp(a, b) < 0;
}
