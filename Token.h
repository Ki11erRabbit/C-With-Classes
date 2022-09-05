//
// Created by ki11errabbit on 9/4/22.
//

#ifndef CWITHCLASSES_TOKEN_H
#define CWITHCLASSES_TOKEN_H

#include <string>

enum TokenType {
    IDENTIFIER, KEYWORD, CONSTANT, OPERATOR, SPECIALCHAR, STRING
};

class Token {
private:
    TokenType type;

};


#endif //CWITHCLASSES_TOKEN_H
