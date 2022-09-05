//
// Created by ki11errabbit on 9/4/22.
//

#ifndef CWITHCLASSES_TOKEN_H
#define CWITHCLASSES_TOKEN_H

#include <string>
#include <sstream>

enum TokenType {
    IDENTIFIER, KEYWORD, CONSTANT, OPERATOR, SPECIALCHAR, STRING, UNDEFINED
};

class Token {
private:
    TokenType type;
    std::string value;

    std::string typeName (TokenType type) const {
        switch (type) {
            case IDENTIFIER:
                return "IDENTIFIER";
            case KEYWORD:
                return "KEYWORD";
            case CONSTANT:
                return "CONSTANT";
            case OPERATOR:
                return "OPERATOR";
            case SPECIALCHAR:
                return "SPECIALCHAR";
            case STRING:
                return "STRING";
            case UNDEFINED:
            default:
                return "UNDEFINED";
        }
        return "ERROR";
    }
public:
    Token (TokenType type, std::string value)
    : type(type), value(value) {}

    TokenType getType () const {
        return type;
    }
    std::string getValue() const {
        return value;
    }

    std::string toString() const {
        std::stringstream out;
        out << typeName(type) << " " << value;
        return out.str();
    }

    friend std::ostream& operator<<(std::ostream& os, const Token& token)
    {
        os << token.toString();
        return os;
    }
};


#endif //CWITHCLASSES_TOKEN_H
