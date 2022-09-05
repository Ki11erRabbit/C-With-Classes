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
    TokenType tokenTypeAt(size_t i) const {
        return tokens.at(i).getType();
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
        cout << endl << endl << "\t" << tokens.at(0) << endl;
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
        vector<string> param;
        vector<vector<Parameter>> members;
        if (tokenType() == KEYWORD) {
            match(KEYWORD);
            name = match(IDENTIFIER);
            if (tokenType() == BRACE) {
                match(BRACE);
                members = formMembers();
                param = parameterBlock();

                methodList();

                if (tokenType() == SPECIALCHAR) {
                    if (tokenType() == SPECIALCHAR) {
                        return;
                    }
                }
            }
        }
        else {
            throwError();
        }
    }

    vector<vector<Parameter>> formMembers() {//can't have volatile,register, or static members
        vector<vector<Parameter>> members;
        varList();

        return members;
    }
    vector<Parameter> varList() {
        vector<Parameter> variables;
        string type;
        string name;
        string value;
        string pointer;
        string temp;
        if (tokenType() == KEYWORD) {
            while (tokenType() != TERMINATOR) {
                if (tokenType() == KEYWORD) {//sets variable type
                    temp = match(KEYWORD);
                    if (temp == "volatile" || temp == "register" || temp == "extern" || temp == "const") {
                        type += temp + " ";
                    }
                    else if (temp == "unsigned" || temp == "signed") {
                        type += temp + " ";
                    }
                    else {
                        type += match(KEYWORD);//should be something like char, int, long, etc
                    }
                }
                else if (tokenType() == OPERATOR) {// ONLY for pointers
                    if (nextTokenType() == IDENTIFIER && subTokenType() != COMMA) {// for pointers
                        temp = match(OPERATOR);
                        pointer = "*";
                        name = match(IDENTIFIER);
                        if (tokenType() == OPERATOR && subTokenType() == ASSIGNMENT) {
                            match(OPERATOR); //matches assignment operator
                            temp = "";
                            while (tokenType() != TERMINATOR || tokenType() != OPERATOR) {
                                temp += match();
                            }
                        }
                    }
                    else {//for commas
                        variables.push_back(Parameter(type,name,pointer,value));
                        match(OPERATOR);
                    }
                }
                else if (tokenType() == IDENTIFIER) {
                    name = match(IDENTIFIER);

                    if (tokenType() == OPERATOR) {
                        if (subTokenType() == COMMA) {
                            match(OPERATOR);
                        }
                    }
                }


            }
            match(TERMINATOR);
            variables.push_back(Parameter(type,name,pointer,value));
        }
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
                expression = variableDeclaration();
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
    string variableDeclaration() {
        string expression;
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
    }


    vector<string> parameterBlock() {
        vector<string> params;
        string expression;
        if (tokenType() == KEYWORD) {
            expression = variableDeclaration();
        }
    }
    void methodList() {

    }



public:

    Parser (vector<Token> tokens)
    : tokens(tokens) {}

    void startParsing() {

        classForm();
    }

};


#endif //CWITHCLASSES_PARSER_H
