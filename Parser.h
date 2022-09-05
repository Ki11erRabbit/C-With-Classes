//
// Created by ki11errabbit on 9/4/22.
//

#ifndef CWITHCLASSES_PARSER_H
#define CWITHCLASSES_PARSER_H

#include <string>
#include <vector>
#include <sstream>

#include "Token.h"
#include "Class.h"

#define headerFile pair<string,vector<string>>
using namespace std;

class Parser {
private:
    vector<Token> tokens;
    string className;

    TokenType tokenType() const {
        return tokens.at(0).getType();
    }
    TokenType nextTokenType() const {
        return tokens.at(1).getType();
    }
    TokenType nextNextTokenType() const {
        return tokens.at(2).getType();
    }

    string advanceToken() {
        string value = tokens.at(0).getValue();
        tokens.erase(tokens.begin());
        return value;
    }

    void throwError() {
        throw(tokens.at(0));
    }

    string match(TokenType type) {
        if (tokenType() == type) {
            return advanceToken();
        }
        else {
            throwError();
        }
    }
    string match() {
        return advanceToken();
    }

    void classForm() {
        string name;
        if (tokenType() == KEYWORD) {
            match(KEYWORD);
            name = match(IDENTIFIER);
            if (tokenType() == BRACE) {
                parameterBlock();

                methodBlock();

                if (tokenType() == SPECIALCHAR) {
                    if (tokenType() == SPECIALCHAR) {
                        return;
                    }
                }
            }
        }
        throwError();


    }
    Parameter parameterForm() {
        string argType, argName;
        string argPointer = "";

        argType = match(KEYWORD);
        if (tokenType() == OPERATOR) {//for pointers
            argPointer = match(OPERATOR);
        }
        argName = match(IDENTIFIER);
        return Parameter(argType, argPointer,argName);
    }
    void methodForm() {
        string returnType, name;

        returnType = match(KEYWORD);
        name = match(IDENTIFIER);
        vector<Parameter> param = parameterList();
        match(SPECIALCHAR);// closing paren

        codeBlockForm();

    }

    vector<string> codeBlockForm() {
        match(BRACE); //opening brace
        vector<string> lines;
        lines.push_back(expressionForm());

        match(BRACE);//closeing brace
        return lines;
    }
    string expressionForm() {
        if(tokenType() == KEYWORD) {
            if (nextTokenType() == IDENTIFIER) {//variable declaration

            }
            if (nextTokenType() == SPECIALCHAR && (nextNextTokenType() == IDENTIFIER || nextNextTokenType() == CONSTANT) {// loops

            }
        }
    }

    void parameterBlock() {

    }
    void methodBlock() {

    }

    vector<Parameter> parameterList() {
        vector<Parameter> params;
        if (tokenType() == SPECIALCHAR) {
            match(SPECIALCHAR);
            params.push_back(parameterForm());
        }
        if (tokenType() == OPERATOR) {
            match(OPERATOR);
            return parameterList(params);
        }
        else {
            return params;
        }

    }
    vector<Parameter> parameterList(vector<Parameter> params) {
        if (tokenType() == OPERATOR) {
            match(OPERATOR);
            params.push_back(parameterForm());
        }
        if (tokenType() == OPERATOR) {
            match(OPERATOR);
            return parameterList(params);
        }
        else {
            return params;
        }

    }
    void methodList() {

    }

public:

};


#endif //CWITHCLASSES_PARSER_H
