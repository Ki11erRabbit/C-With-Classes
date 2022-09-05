//
// Created by ki11errabbit on 9/4/22.
//

#ifndef CWITHCLASSES_SCANNER_H
#define CWITHCLASSES_SCANNER_H

#include <string>
#include <iostream>

#include "Token.h"

class Scanner {
private:
    std::string input;

    int findNextSpace() {
        for (size_t i = 0; i < input.size(); i++) {
            if (std::isspace(input.at(i)))
                return i;
        }
    }
    int findStringEnding() {
        for (size_t i = 1; i < input.size(); i++) {
            if (input.at(i) == '\"')
                return i+1;
        }
    }
    int findCharEnding() {
        for (size_t i = 1; i < input.size(); i++) {
            if (input.at(i) == '\'')
                return i+1;
        }
    }
    int findLineCommentEnding(){
        for (size_t i = 1; i < input.size(); i++) {
            if (input.at(i) == '\n')
                return i;
        }
    }
    int findMultiLineCommentEnding(){
        for (size_t i = 1; i < input.size(); i++) {
            if (input.at(i) == '*' && input.at(i+1) == '/')
                return i+1;
        }
    }
    int findNumEnding() {
        for (size_t i = 1; i < input.size(); i++) {
            if (!std::isdigit(input.at(i))) {
                if (i == 1 && input.at(i) == 'x')
                    continue;
                if (input.at(i) == '\n')
                    return i;
                return i;

            }
        }
    }
    size_t findEnding() {
        for (size_t i = 1; i < input.size(); i++) {
            switch (input.at(i)) {
                //Math Operators
                case '=':
                case '*':
                case '/':
                case '%':
                case '+':
                case '-':
                    return i;
                //logical and bitwise Operators
                case '&':
                case '^':
                case '|':
                case '>':
                case '<':
                    return i;
                case '\"':
                case '\'':
                case ' ':
                case '(':
                case ')':
                case '[':
                case ']':
                case '{':
                case '}':
                case ';':
                case ',':
                    return i;
                case '\\':
                    if (input.at(0) == '\'' || input.at(0) == '\"')
                        continue;
                    return i;
            }
        }
        return 0;
    }
    std::pair<int,std::pair<TokenType,SubTokenType>> setType() {
        if (std::isalpha(input.at(0))) {
            //KEYWORDS
            //Types
            if (input.find("char") == 0) {
                return {4, {KEYWORD, TYPE}};
            }
            else if (input.find("short") == 0) {
                return {5, {KEYWORD,TYPE}};
            }
            else if (input.find("int") == 0) {
                return {3, {KEYWORD,TYPE}};
            }
            else if (input.find("long") == 0) {
                return {4,{KEYWORD,TYPE}};
            }
            else if (input.find("float") == 0) {
                return {5, {KEYWORD,TYPE}};
            }
            else if (input.find("double") == 0) {
                return {6,{KEYWORD,TYPE}};
            }
            else if (input.find("unsigned") == 0) {
                return {8, {KEYWORD,TYPE}};
            }
            else if (input.find("signed") == 0) {
                return {6,{KEYWORD,TYPE}};
            }
            else if (input.find("void") == 0) {
                return {4, {KEYWORD,TYPE}};
            }
            else if (input.find("enum") == 0) {
                return {4,{KEYWORD,TYPE}};
            }
            else if (input.find("typedef") == 0) {//TODO: add in ability to create custom types
                return {7,{KEYWORD,TYPE}};
            }
            else if (input.find("auto") == 0) {
                return {4,{KEYWORD,TYPE}};
            }
            else if (input.find("struct") == 0) {
                return {6,{KEYWORD,TYPE}};
            }
            //Control Flow
            else if (input.find("if") == 0) {
                return {2, {KEYWORD,CONTROL}};
            }
            else if (input.find("else") == 0) {
                return {4, {KEYWORD,CONTROL}};
            }
            else if (input.find("while") == 0) {
                return {5,{KEYWORD,CONTROL}};
            }
            else if (input.find("for") == 0) {
                return {3, {KEYWORD,CONTROL}};
            }
            else if (input.find("do") == 0) {
                return {2,{KEYWORD,CONTROL}};
            }
            else if (input.find("case") == 0) {
                return {4, {KEYWORD,CONTROL}};
            }
            else if (input.find("break") == 0) {
                return {5, {KEYWORD,CONTROL}};
            }
            else if (input.find("continue") == 0) {
                return {8,{KEYWORD,CONTROL}};
            }
            else if (input.find("default") == 0) {
                return {7, {KEYWORD,CONTROL}};
            }
            else if (input.find("goto") == 0) {
                return {4, {KEYWORD,CONTROL}};
            }
            //Every other KEYWORD
            else if (input.find("register") == 0) {
                return {8,{KEYWORD,TYPE}};
            }
            else if (input.find("return") == 0) {
                return {6, {KEYWORD,RETURN}};
            }
            else if (input.find("extern") == 0) {
                return {6, {KEYWORD,TYPE}};
            }
            else if (input.find("static") == 0) {
                return {6,{KEYWORD,TYPE}};
            }
            else if (input.find("sizeof") == 0) {
                return {6, {KEYWORD,SIZEOF}};
            }
            else if (input.find("union") == 0) {
                return {5, {KEYWORD,TYPE}};
            }
            else if (input.find("volatile") == 0) {
                return {8, {KEYWORD,TYPE}};
            }
            //My KEYWORDS
            else if (input.find("this") == 0) {
                return {4, {KEYWORD,TYPE}};
            }
            else if (input.find("class") == 0) {//TODO: add next token as custom type
                return {5, {KEYWORD,TYPE}};
            }
            else if (input.find("new") == 0) {
                return {3, {KEYWORD,TYPE}};
            }
            else {
                return {findEnding(),{IDENTIFIER,NONE}};
            }
        }
        else if (isdigit(input.at(0))) {
            return {findNumEnding(), {CONSTANT,NONE}};
        }
        else {
            switch (input.at(0)) {
                case '\"':
                    return {findStringEnding(),{STRING,NONE}};
                case '[':
                case ']':
                    return {1,{SPECIALCHAR,NONE}};
                case '(':
                case ')':
                    return {1,{SPECIALCHAR,NONE}};
                case '{':
                case '}':
                    return {1, {BRACE, NONE}};
                case ';':
                    return {1,{TERMINATOR, NONE}};
                case ',':
                    return {1, {OPERATOR,NONE}};
                case '#':
                    return {1, {SPECIALCHAR,NONE}};
                case '\\':
                    return {1, {SPECIALCHAR,NONE}};
                case ':':
                case '?':
                case '.':
                    return {1, {OPERATOR,NONE}};
                case '/':
                    if (input.at(1) == '/')
                        return {findLineCommentEnding(), {COMMENT,NONE} };
                    if (input.at(1) == '*'){
                        return {findMultiLineCommentEnding(), {COMMENT,NONE}};
                    }
                case '*':
                case '=':
                case '%':
                case '+':
                    if (input.at(1) == '=')
                        return {2, {OPERATOR,NONE}};
                    return {1, {OPERATOR,ASSIGNMENT}};
                case '-':
                    if (input.at(1) == '=' || input.at(1) == '>')
                        return {2, {OPERATOR,NONE}};
                    return {1, {OPERATOR,NONE}};
                    //logical and bitwise Operators
                case '&':
                    if (input.at(1) == '&') {
                        return {2,{OPERATOR,NONE}};
                    }
                    return {1,{OPERATOR,NONE}};
                case '|':
                    if (input.at(1) == '|') {
                        return {2,{OPERATOR,NONE}};
                    }
                    return {1,{OPERATOR,NONE}};
                case '^':
                case '>':
                case '<':
                case '!':
                    if (input.at(1) == '=')
                        return {2, {OPERATOR,NONE}};
                    return {1, {OPERATOR,NONE}};
                case '\'':
                    return {findCharEnding(), {CONSTANT,NONE}};
                case '~':
                    return {findEnding(), {IDENTIFIER,NONE}};
                default:
                    return {findEnding(), {UNDEFINED,NONE}};
            }
        }
    }
public:

    Scanner(const std::string& input) : input (input) { }

    size_t getSize() const {
        return input.size();
    }

    Token scanToken() {
        while (std::isspace((input.at(0)))) {
            input = input.substr(1);
        }

        std::pair<int,std::pair<TokenType,SubTokenType>> pair = setType();
        std::string value;

        value = input.substr(0, pair.first);
        input = input.substr(pair.first);

        return Token(pair.second.first,pair.second.second,value);

    }

};


#endif //CWITHCLASSES_SCANNER_H
