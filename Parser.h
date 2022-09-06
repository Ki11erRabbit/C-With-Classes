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

    string expression() {

    }

    string statement() {

    }

    string functionMacro(string name) {
        string expression = name;
        expression += match(SPECIALCHAR);//(
        vector<string> list = identifierList();
        for (size_t i = 0; i < list.size(); i++) {
            expression += list.at(i);
            if (i < list.size()-1) {
                expression += ", ";
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
        expression += "\n";

        /*if (tokenType() == BRACE) { // {
            expression += " " + match(BRACE);
            vector<string> tokenString = matchUntil(BRACE);
            for (size_t i = 0; i < tokenString.size(); i++) {
                if (tokenString.at(i) == "\\") {
                    expression += tokenString.at(i) + "\n";
                }
                else {
                    expression += tokenString.at(i);
                }
            }
            expression += match(BRACE) + "\n";
        }
        else if (tokenType() == PREPROC) { //
            vector<string> tokenString = matchUntil(KEYWORD);
        }
        else if (tokenType() == OPERATOR) {// ++x
            expression += " " + match(OPERATOR);
            expression += match(IDENTIFIER);
        }
        else if (tokenType() == SPECIALCHAR) {//(x + y) or \ i.e expression

        }
        else {//everything else while, statements, ; ; ;

        }*/

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




public:

    Parser (vector<Token> tokens)
    : tokens(tokens) {}

    void startParsing() {
        vector<string> includes = includeList();
        vector<string> macros = macroList();

        for (auto include : includes) {
            cout << include << endl;
        }
        for (auto define : macros) {
            cout << define << endl;
        }

    }

};


#endif //CWITHCLASSES_PARSER_H
