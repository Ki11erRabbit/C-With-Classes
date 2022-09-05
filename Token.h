//
// Created by ki11errabbit on 9/4/22.
//

#ifndef CWITHCLASSES_TOKEN_H
#define CWITHCLASSES_TOKEN_H

#include <string>
#include <sstream>

enum TokenType {
    IDENTIFIER, KEYWORD, CONSTANT, OPERATOR, SPECIALCHAR, STRING, UNDEFINED, COMMENT, BRACE, TERMINATOR,
};

enum SubTokenType {//for keywords
    NONE,TYPE, CONTROL, CODE
};

class Token {
private:
    TokenType type;
    SubTokenType subType;
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
            case COMMENT:
                return "COMMENT";
            case BRACE:
                return "BRACE";
            case TERMINATOR:
                return "TERMINATOR";
            case UNDEFINED:
            default:
                return "UNDEFINED";
        }
        return "ERROR";
    }

    std::string typeName (SubTokenType type) const {
        switch (type) {
            case NONE:
                return "";
            case TYPE:
                return "TYPE";
            case CONTROL:
                return "CONTROL";
            case CODE:
                return "CODE";
            default:
                return "UNDEFINED";
        }
        return "ERROR";
    }
public:
    Token (TokenType type, std::string value)
    : type(type), value(value) {}
    Token (TokenType type, SubTokenType subType, std::string value)
            : type(type), subType(subType), value(value) {}

    TokenType getType () const {
        return type;
    }
    std::string getValue() const {
        return value;
    }
    TokenType getSubType () const {
        return type;
    }
    void setSubType(SubTokenType type) {
        subType = type;
    }


    std::string toString() const {
        std::stringstream out;
        if (subType != NONE) {
            out << typeName(type) << ", " << typeName(subType) << ": " << value;
            return out.str();
        }
        out << typeName(type) << ": " << value;
        return out.str();
    }

    friend std::ostream& operator<<(std::ostream& os, const Token& token)
    {
        os << token.toString();
        return os;
    }
};


#endif //CWITHCLASSES_TOKEN_H
