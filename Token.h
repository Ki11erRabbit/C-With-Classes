//
// Created by ki11errabbit on 9/4/22.
//

#ifndef CWITHCLASSES_TOKEN_H
#define CWITHCLASSES_TOKEN_H

#include <string>
#include <sstream>

enum TokenType {
    IDENTIFIER, KEYWORD, CONSTANT, OPERATOR, SPECIALCHAR, STRING, UNDEFINED, COMMENT, BRACE, TERMINATOR, PREPROC
};

enum SubTokenType {//for keywords and operators
    NONE=0,TYPE=1, CONTROL=2, CODE=3, ASSIGNMENT, SIZEOF, RETURN, COMMA, OPEN, CLOSE, ACCESSOR
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
            case PREPROC:
                return "PREPROC";
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
            case ASSIGNMENT:
                return "ASSIGNMENT";
            case SIZEOF:
                return "SIZEOF";
            case RETURN:
                return "RETURN";
            case COMMA:
                return "COMMA";
            case OPEN:
                return "OPEN";
            case CLOSE:
                return "CLOSE";
            case ACCESSOR:
                return "ACCESSOR";
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
    SubTokenType getSubType () const {
        return subType;
    }
    void setSubType(SubTokenType type){
        subType = type;
    }
    void setType(TokenType type) {
        this->type = type;
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
