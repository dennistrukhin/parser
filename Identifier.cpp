//
// Created by Dennis Trukhin on 2019-08-16.
//

#include "Identifier.h"

Identifier::Identifier(TokenEnumerator t, void * v) {
    type = t;
    value = v;
}
