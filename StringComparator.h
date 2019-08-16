//
// Created by Трухин Денис on 2019-08-14.
//

#ifndef PARSER_STRINGCOMPARATOR_H
#define PARSER_STRINGCOMPARATOR_H


class StringComparator {
public:
    bool operator()(char const *a, char const *b) const;
};


#endif //PARSER_STRINGCOMPARATOR_H