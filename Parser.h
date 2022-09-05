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
    SubTokenType subTokenType() const {
        return tokens.at(0).getSubType();
    }
    SubTokenType nextSubTokenType() const {
        return tokens.at(1).getSubType();
    }
    SubTokenType nextNextSubTokenType() const {
        return tokens.at(2).getSubType();
    }

    string advanceToken() {
        string value = tokens.at(0).getValue();
        tokens.erase(tokens.begin());
        return value;
    }

    void throwError() {
        cout << endl << tokens.at(0) << endl;
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

                methodList();

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

    CodeBlock codeBlockForm() {
        match(BRACE); //opening brace
        vector<string> lines;
        while (tokenType() != BRACE) {
            lines.push_back(expressionForm());
        }
        match(BRACE);//closeing brace
        return CodeBlock(lines);
    }
    string expressionForm() {
        string expression;
        if(tokenType() == KEYWORD) {
            switch (subTokenType()) {
                case TYPE://TYPE declarations
                expression += match(KEYWORD);
                while (subTokenType() == TYPE) {// for things like unsigned long
                    expression += " " + match(KEYWORD);
                }
                if (tokenType() == OPERATOR) {//pointer
                    expression += " " + match(OPERATOR);
                }
                while (tokenType() != TERMINATOR) {// for multiple declarations
                    if (tokenType() == IDENTIFIER) {
                        if (nextTokenType() == SPECIALCHAR) {
                            expression += functionCallForm();
                        }
                        else {
                            expression += " " + match(IDENTIFIER);
                        }
                    }
                    if (subTokenType() == ASSIGNMENT) {// for variable assignment
                        expression +=  " " + match(OPERATOR);
                        expression + " " +match();
                    }
                    if (tokenType() == OPERATOR) {// comma variable declaration
                        expression += match(OPERATOR) + " ";
                    }
                }
                expression += match(TERMINATOR);
                cout << expression << endl;
                    return expression;
                case CONTROL://loops and conditionals
                    expression += match(KEYWORD);
                    if (expression == "if") {

                        expression += conditionalForm();

                        CodeBlock block = codeBlockForm();
                        expression += block.toString() + "\n";
                    }
                    else if (expression == "else") {
                        CodeBlock block = codeBlockForm();
                        expression += block.toString() +"\n";
                    }
                    else if (expression == "for") {
                        expression += match(SPECIALCHAR);//opening paren
                        while (tokenType() != SPECIALCHAR) {
                            while (tokenType() != TERMINATOR) {
                                expression +=match();//matches everything
                            }
                            expression += match(TERMINATOR);
                        }
                        expression += match(SPECIALCHAR);//closing paren
                        CodeBlock block = codeBlockForm();
                        expression += block.toString() +"\n";
                    }
                    else if (expression == "while") {
                        expression += match(SPECIALCHAR);

                        expression += conditionalForm();

                        CodeBlock block = codeBlockForm();
                        expression += block.toString() + "\n";
                    }
                    else if (expression == "do") {
                        CodeBlock block = codeBlockForm();
                        expression += block.toString();
                        expression += match(KEYWORD);

                        expression += conditionalForm();

                        expression += match(TERMINATOR) + "\n";
                    }
                    else if (expression == "goto") {
                        expression += " " +match(IDENTIFIER);
                        expression +=match(TERMINATOR);
                    }
                    cout << expression << endl;
                    return expression;
                case SIZEOF://special declarations should never get run
                expression += functionCallForm();
                expression += match(TERMINATOR);
                    cout << expression << endl;
                    return expression;
                case RETURN://return
                    expression += match(KEYWORD) + " ";
                    while (tokenType() != TERMINATOR) {
                        expression += " " +match();
                    }
                    expression += match(TERMINATOR);
                    cout << expression << endl;
                    return expression;
            }
        }//end of keywords
        else if (tokenType() == IDENTIFIER) {
            expression += match(IDENTIFIER);
            while (tokenType() != TERMINATOR) {
                expression += " " + match();//matches everything until semicolon
            }
            expression += match(TERMINATOR);
            cout << expression << endl;
            return expression;
        }

    }
    string conditionalForm() {// for (some logical expressions here)
        string expression;
        expression += match(SPECIALCHAR);
        while (tokenType() != SPECIALCHAR) {//closing paren
            expression += match(); //matches everything TODO: make sure this works
        }
        expression += match(SPECIALCHAR); //closing paren

        cout << expression << endl;
        return expression;
    }
    string functionCallForm() {
        string call;
        if (subTokenType() == SIZEOF) {
            call += match(KEYWORD);
        }
        else {
            call += match(IDENTIFIER);
        }
        call += match(SPECIALCHAR);
        while (tokenType() != SPECIALCHAR) {
            call += match();//hacky way to match everything
        }
        call += match(SPECIALCHAR);

        cout << call << endl;
        return call;
    }

    void parameterBlock() {

    }
    void methodList() {

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

public:

    Parser (vector<Token> tokens)
    : tokens(tokens) {}

    void startParsing() {

        classForm();
    }

};


#endif //CWITHCLASSES_PARSER_H
