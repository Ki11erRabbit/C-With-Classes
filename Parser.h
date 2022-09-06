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
#include "Struct.h"

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
    string peek() const {
        return tokens.at(0).getValue();
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
    string matchAll() {
        return advanceToken();
    }
    vector<string> matchUntil(TokenType type) {
        vector<string> tokenString;
        while (tokenType() != type) {
            tokenString.push_back(advanceToken());
        }
        if (tokens.size() == 0) {
            throwError();
        }
        else {
            return tokenString;
        }
    }
    vector<string> matchUntil(TokenType type1, TokenType type2) {
        vector<string> tokenString;
        while (!(tokenType() == type1 || tokenType() == type2)) {
            tokenString.push_back(advanceToken());
        }
        if (tokens.size() == 0) {
            throwError();
        }
        else {
            return tokenString;
        }
    }

    string include() {
        if (peek() == "#include") {
            string expression;
            expression += match(PREPROC);//#include
            expression += " ";
            if (tokenType() == OPERATOR) {
                expression += match(OPERATOR);//<
                expression += match(IDENTIFIER);
                expression += match(OPERATOR);
            }
            else if (tokenType() == STRING){
                expression += match(STRING);

            }
            return expression;
        }
        else {
            throwError();
        }
    }
    vector<string> includeList() {
        vector<string> list;
        if (peek() == "#include") {
            list.push_back(include());
            return includeList(list);
        }
        else {
            return list;
        }
    }
    vector<string> includeList(vector<string> list) {
        if (peek() == "#include") {
            list.push_back(include());
            return includeList(list);
        }
        else {
            return list;
        }
    }

    string identifier() {
        if (tokenType() == IDENTIFIER) {
            return match(IDENTIFIER);
        }
        else {
            throwError();
        }
    }

    vector<string> identifierList() {
        vector<string> list;
        list.push_back(identifier());
        if (tokenType() == OPERATOR) {
            match(OPERATOR);
            return identifierList(list);
        }
        else {
            return list;
        }
    }
    vector<string> identifierList(vector<string> list) {
        list.push_back(identifier());
        if (tokenType() == OPERATOR) {
            match(OPERATOR);
            return identifierList(list);
        }
        else {
            return list;
        }
    }

    string functionMacro(string name) {
        string expression = name;
        expression += match(SPECIALCHAR);//(
        vector<string> list = identifierList();
        for (size_t i = 0; i < list.size(); i++) {
            expression += list.at(i);
            if (i < list.size()-1) {
                expression += ",";
            }
        }
        expression += match(SPECIALCHAR);//)

        expression += " ";

        vector<string> tokenString = matchUntil(KEYWORD,PREPROC);

        for (size_t i = 0; i < tokenString.size(); i++) {
            if (tokenString.at(i) == "\\") {
                expression += tokenString.at(i) + "\n";
            }
            else {
                expression += tokenString.at(i);
            }
        }
        return expression;
    }

    string macro() {
        string expression;
        if (peek() == "#define") {
            expression += match(PREPROC);//#define
            expression += " ";
            if (tokenType() == IDENTIFIER) {
                string temp;
                temp = match(IDENTIFIER);
                if (tokenType() == CONSTANT) {
                    expression += temp + " " + match(CONSTANT);
                }
                else if (tokenType() == STRING) {
                    expression += temp + " " + match(STRING);
                }
                else if (tokenType() == OPERATOR) {//for like #define AND &&
                    expression += temp + " " + match(OPERATOR);
                }
                else if (tokenType() == SPECIALCHAR) { //for macros like MACRO(x,y) (x + y)
                    expression += functionMacro(temp);
                }
                return expression;
            }
            else
                throwError();
        }
        else {
            throwError();
        }
    }
    vector<string> macroList() {
        vector<string> list;
        if (tokenType() == PREPROC) {
            list.push_back(macro());
            return macroList(list);
        }
        else {
            return list;
        }
    }
    vector<string> macroList(vector<string> list) {
        if (tokenType() == PREPROC) {
            list.push_back(macro());
            return macroList(list);
        }
        else {
            return list;
        }
    }

    string functionCall() {

    }

    string type() {//mashes type tokens together
        string type;
        if (tokenType() == KEYWORD) {
            while (tokenType() == KEYWORD && subTokenType() == TYPE) {
                type += match(KEYWORD) + " ";
            }
            return type;
        }
        else {
            throwError();
        }
    }

    vector<Parameter> variableDeclaration() {
        string varType,varName, pointer, contents;
        vector<Parameter> varList;
        if (tokenType() == KEYWORD) {
            varType = type();
            if (tokenType() == OPERATOR) {//for pointers
                pointer = match(OPERATOR);
            }
            varName = match(IDENTIFIER);
            if (tokenType() == OPERATOR) {
                if (subTokenType() == ASSIGNMENT) {
                    match(OPERATOR);
                    if (tokenType() == IDENTIFIER) {//function
                        contents = functionCall();
                    }
                    else if (tokenType() == CONSTANT) {
                        contents = match(CONSTANT);
                    }
                    else if (tokenType() == STRING) {
                        contents = match(STRING);
                    }
                }
                varList.push_back(Parameter(varType,pointer,varName,contents));
                return variableDeclaration(varList, varType);
            }
            if (tokenType() == TERMINATOR) {
                return varList;
            }
            else {
                throwError();
            }
        }
        else {
            throwError();
        }
    }
    vector<Parameter> variableDeclaration(vector<Parameter> varList, string varType) {
        string varName, pointer, contents;
        if (tokenType() == KEYWORD) {
            varType = type();
            if (tokenType() == OPERATOR) {//for pointers
                pointer = match(OPERATOR);
            }
            varName = match(IDENTIFIER);
            if (tokenType() == OPERATOR) {
                if (subTokenType() == ASSIGNMENT) {
                    match(OPERATOR);
                    if (tokenType() == IDENTIFIER) {//function
                        contents = functionCall();
                    }
                    else if (tokenType() == CONSTANT) {
                        contents = match(CONSTANT);
                    }
                    else if (tokenType() == STRING) {
                        contents = match(STRING);
                    }
                }
                varList.push_back(Parameter(varType,pointer,varName,contents));
                return variableDeclaration(varList, varType);
            }
            if (tokenType() == TERMINATOR) {
                return varList;
            }
            else {
                throwError();
            }
        }
        else {
            throwError();
        }
    }

    vector<vector<Parameter>> variableDeclarationList() {
        vector<vector<Parameter>> list;
        if (tokenType() == KEYWORD) {
            list.push_back(variableDeclaration());
            return variableDeclarationList(list);
        }
        else {
            return list;
        }
    }
    vector<vector<Parameter>> variableDeclarationList(vector<vector<Parameter>> list) {
        if (tokenType() == KEYWORD) {
            list.push_back(variableDeclaration());
            return variableDeclarationList(list);
        }
        else {
            return list;
        }
    }



    void updateType(string identifier) {
        for (auto token : tokens) {
            if (token.getType() == IDENTIFIER) {
                if (token.getValue() == identifier) {
                    token.setType(KEYWORD);
                    token.setSubType(TYPE);
                }
            }
        }
    }
    Struct structM() {
        if (tokenType() == KEYWORD) {
            match(KEYWORD);
            string name = match(IDENTIFIER);
            match(BRACE);
            vector<vector<Parameter>> tempBody = variableDeclarationList();
            vector<Parameter> body;
            for (auto list : tempBody) {
                for (auto var : list) {
                    body.push_back(var);
                }
            }
            return Struct(name,body);
        }
        else {
            throwError();
        }
    }

    string typeDef() {
        string expression;
        if (tokenType() == KEYWORD) {
            expression += match(KEYWORD);
            if (peek() == "struct") {
                if (nextTokenType() == IDENTIFIER && nextNextTokenType() == IDENTIFIER) {
                    expression += match(KEYWORD) + " " + match(IDENTIFIER);
                    expression += match(IDENTIFIER) + match(TERMINATOR);
                }
                else {
                    Struct temp =structM();
                }
            }
            string temp = type();
            if (temp == "struct") {
                string structName;
                expression += " " + temp;
                structName = match(IDENTIFIER);
                expression += " " + structName;
                if (tokenType() == IDENTIFIER) {
                    temp = match(IDENTIFIER);
                    updateType(temp);
                    expression += " " + temp;
                    expression += match(TERMINATOR);
                }
                else if (tokenType() == BRACE) {

                }
            }
            else {
                expression += " " + match(IDENTIFIER);
                expression += match(TERMINATOR);
            }
            return expression;
        }
        else {
            throwError();
        }
    }
    vector<string> typedefList() {

    }




public:

    Parser (vector<Token> tokens)
    : tokens(tokens) {}

    void startParsing() {
        vector<string> includes = includeList();
        vector<string> macros = macroList();
        vector<string> typeDef = typedefList();

        for (auto include : includes) {
            cout << include << endl;
        }
        for (auto define : macros) {
            cout << define << endl;
        }

        for (auto token : tokens) {
            cout << token << endl;
        }

    }

};


#endif //CWITHCLASSES_PARSER_H
