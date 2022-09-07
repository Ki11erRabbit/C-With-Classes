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
    vector<string> matchUntil(TokenType type1, SubTokenType type2) {
        vector<string> tokenString;
        while (!(tokenType() == type1 || subTokenType() == type2)) {
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

        vector<string> tokenString = matchUntil(PREPROC,TYPE);

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
        if (tokenType() == PREPROC && peek() == "#define") {
            list.push_back(macro());
            return macroList(list);
        }
        else {
            return list;
        }
    }
    vector<string> macroList(vector<string> list) {
        if (tokenType() == PREPROC && peek() == "#define") {
            list.push_back(macro());
            return macroList(list);
        }
        else {
            return list;
        }
    }


    string expression() {
        string statement;
        if (tokenType() == SPECIALCHAR && subTokenType() == OPENPAREN) {
            statement += match(SPECIALCHAR);
            if (tokenType() == KEYWORD) {//typecast
                statement += match(KEYWORD);
                if (tokenType() == OPERATOR) //pointer
                    statement += match(OPERATOR);
                match(SPECIALCHAR);//CLOSE Paren
                statement += expression();
            }
            else {//for additional statements
                statement += expression();
            }
        }
        else if (tokenType() == IDENTIFIER) {
            statement += match(IDENTIFIER);
        }
        else if (tokenType() == CONSTANT) {
            statement += match(CONSTANT);
        }
        else if (tokenType() == STRING) {
            statement += match(STRING);
        }
        else if (tokenType() == OPERATOR) { // ++var or *var dereferense
            if (peek() == "*" || peek() == "++" || peek() == "--") {
                statement += match(OPERATOR);
                if (peek() == "*") {
                    statement += match(OPERATOR);
                }
            }
        }
        if (tokenType() == OPERATOR && (peek() == "++" || peek() == "--")) {
            statement += match(OPERATOR);
        }
        // operator
        if (tokenType() == OPERATOR) {
            if (subTokenType() == ASSIGNMENT) {
                statement += match(OPERATOR);
                statement += expression();
            }
            if (subTokenType() == SIZEOF) {
                statement += sizeOf();
                statement += expression();
            }
            else {
                statement += match(OPERATOR);
            }
        }
        if (tokenType() == BRACE) {
            statement += match(BRACE);
            vector<string> tokenString = matchUntil(BRACE);
            for (auto token : tokenString) {
                statement += token;
            }
        }

        if (subTokenType() == CLOSEPAREN || subTokenType() == COMMA || tokenType() == TERMINATOR) {
            return statement;
        }
        statement += expression();
    }

    string sizeOf() {
        string expression;
        expression += match(OPERATOR);
        expression += match(SPECIALCHAR);
        vector<string> statement = matchUntil(SPECIALCHAR);
        for (auto state : statement) {
            expression += state;
        }
        expression += match(SPECIALCHAR);
        return expression;
    }

    string parameter() {
        string output;
        output += expression();
        return  output;
        if (tokenType() == IDENTIFIER) {//variable
            output += match(IDENTIFIER);
            if (subTokenType() == ASSIGNMENT) {//for structs -> or .
                output += match(OPERATOR);
                output += match(IDENTIFIER);
            }
        }
        else if (tokenType() == OPERATOR && nextTokenType() == IDENTIFIER) {//dereference/reference
            output += match(OPERATOR);
            output += match(IDENTIFIER);
        }
        else if (subTokenType() == SIZEOF) {
            output += sizeOf();
        }
        else if (tokenType() == STRING) {
            output += match(STRING);
        }
        else if (tokenType() == CONSTANT) {
            output += match(CONSTANT);
        }
        else {
            output += expression();
        }
        return output;
    }
    vector<string> parameterList() {
        vector<string> list;
        if (tokenType() == IDENTIFIER || (tokenType() == OPERATOR && peek() == "*")) {
            list.push_back(parameter());
            return parameterList(list);
        }
        else if (tokenType() == CONSTANT) {
            list.push_back(parameter());
            return parameterList(list);
        }
        else if (tokenType() == STRING) {
            list.push_back(parameter());
            return parameterList(list);
        }
        else {
            return list;
        }
    }
    vector<string> parameterList(vector<string> list) {
        if (tokenType() == IDENTIFIER || (tokenType() == OPERATOR && peek() == "*")) {
            list.push_back(parameter());
            return parameterList(list);
        }
        else if (tokenType() == CONSTANT) {
            list.push_back(parameter());
            return parameterList(list);
        }
        else if (tokenType() == STRING) {
            list.push_back(parameter());
            return parameterList(list);
        }
        else {
            return list;
        }
    }

    string functionCall() {
        if (tokenType() == IDENTIFIER) {
            string expression;
            expression += match(IDENTIFIER);
            expression += match(SPECIALCHAR);
            vector<string> paramList = parameterList();
            for (size_t i = 0; i < paramList.size(); i++) {
                expression += paramList.at(i);
                if (i < paramList.size() - 1) {
                    expression += ",";
                }
            }
            expression += match(SPECIALCHAR);

            return expression;
        }
        else {
            throwError();
        }
    }

    string type() {//mashes type tokens together
        string type;
        if (tokenType() == KEYWORD) {
            if (peek() == "struct") {
                type += match(KEYWORD);
                type += " " + match(IDENTIFIER);
                return type;
            }
            while (tokenType() == KEYWORD && subTokenType() == TYPE) {
                type += match(KEYWORD);
                if (nextTokenType() == KEYWORD && nextSubTokenType() == TYPE) {
                    type += " ";
                }
            }
            return type;
        }
        else {
            throwError();
        }
    }

    vector<Parameter> variableDeclaration() {//TODO: make use expression()
        string varType,varName, pointer, contents;
        vector<Parameter> varList;
        if (tokenType() == KEYWORD) {
            varType = type();
            if (tokenType() == OPERATOR) {//for pointers
                pointer = match(OPERATOR);
                if (tokenType() == OPERATOR)//for pointer of pointers
                    pointer += match(OPERATOR);
            }
            if (tokenType() == IDENTIFIER) {
                varName = match(IDENTIFIER);
            }
            else if (tokenType() == TERMINATOR) {
                return varList;
            }
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
                    else if (tokenType() == OPERATOR && nextTokenType() == IDENTIFIER) {//dereference * or address &
                        contents += match(OPERATOR);
                        contents += match(IDENTIFIER);
                    }
                    else if (subTokenType() == SIZEOF) {
                        contents += sizeOf();
                    }
                }
                else {
                    match(OPERATOR);
                }
                varList.push_back(Parameter(varType,pointer,varName,contents));
                return variableDeclaration(varList, varType);
            }
            else if (tokenType() == SPECIALCHAR) {//for array declaration
                varName += match(SPECIALCHAR);
                if (tokenType() == CONSTANT) {
                    varName += match(CONSTANT);
                }
                varName += match(SPECIALCHAR);
                if (tokenType() == SPECIALCHAR) {//for multidimentional array declaration
                    varName += match(SPECIALCHAR);
                    if (tokenType() == CONSTANT) {
                        varName += match(CONSTANT);
                    }
                    varName += match(SPECIALCHAR);
                }
                if (tokenType() == BRACE) {
                    contents += expression();
                }
            }
            varList.push_back(Parameter(varType,pointer,varName,contents));
            if (tokenType() == TERMINATOR) {
                match(TERMINATOR);
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

        if (tokenType() == OPERATOR) {//for pointers
            pointer = match(OPERATOR);
        }
        if (tokenType() == IDENTIFIER) {
            varName = match(IDENTIFIER);
        }
        else if (tokenType() == TERMINATOR) {
            return varList;
        }
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
            else {
                match(OPERATOR);
            }
            varList.push_back(Parameter(varType,pointer,varName,contents));
            return variableDeclaration(varList, varType);
        }
        varList.push_back(Parameter(varType,pointer,varName,contents));
        if (tokenType() == TERMINATOR) {
            match(TERMINATOR);
            return varList;
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
        for (size_t i = 0; i < tokens.size(); i++) {
            if (this->tokens.at(i).getType() == IDENTIFIER) {
                if (this->tokens.at(i).getValue() == identifier) {
                    this->tokens.at(i).setType(KEYWORD);
                    this->tokens.at(i).setSubType(TYPE);
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
            match(BRACE);
            return Struct(name,body);;
        }
        else {
            throwError();
        }
    }

    string typeDef() {
        string expression;
        string newType;
        if (tokenType() == KEYWORD && peek() == "typedef") {
            expression += match(KEYWORD);
            if (peek() == "struct") {
                if (nextTokenType() == IDENTIFIER && nextNextTokenType() == IDENTIFIER) {
                    expression += " " + match(KEYWORD) + " ";
                    expression += match(IDENTIFIER) + " ";
                    newType = match(IDENTIFIER) + " ";
                    expression += newType + match(TERMINATOR);
                }
                else {
                    Struct temp = structM();
                    expression += " " + temp.toString();
                    newType = match(IDENTIFIER);
                    expression += " " + newType;
                    expression += match(TERMINATOR);
                }
            }
            else {
                expression += " " + type();
                newType = match(IDENTIFIER);
                expression += " " + newType;
                expression += match(TERMINATOR);
            }
            updateType(newType);
            return expression;
        }
        else {
            throwError();
        }
    }
    vector<string> typedefList() {
        vector<string> list;
        if (tokenType() == KEYWORD && peek() == "typedef") {
            list.push_back(typeDef());
            return typedefList(list);
        }
        else {
            return list;
        }
    }
    vector<string> typedefList(vector<string> list) {
        if (tokenType() == KEYWORD && peek() == "typedef") {
            list.push_back(typeDef());
            return typedefList(list);
        }
        else {
            return list;
        }
    }

    vector<Struct> structList() {
        vector<Struct> list;
        if (tokenType() == KEYWORD && peek() == "struct") {
            list.push_back(structM());
            match(TERMINATOR);
            return structList(list);
        }
        else {
            return list;
        }
    }
    vector<Struct> structList(vector<Struct> list) {
        if (tokenType() == KEYWORD && peek() == "struct") {
            list.push_back(structM());
            match(TERMINATOR);
            return structList(list);
        }
        else {
            return list;
        }
    }

    bool isMethod() {
        size_t i = 0;
        while (tokens.at(i).getSubType() == TYPE) {
            i++;
        }
        if (tokens.at(i).getType() == IDENTIFIER) {
            if (tokens.at(i+1).getType() == SPECIALCHAR)
                return true;
            else
                return false;
        }
        else {
            false;
        }
    }
    bool isMember() {
        size_t i = 0;
        while (tokens.at(i).getSubType() == TYPE) {
            i++;
        }
        if (tokens.at(i).getType() == IDENTIFIER) {
            if (tokens.at(i+1).getType() == OPERATOR || tokens.at(i+1).getType() == TERMINATOR)
                return true;
            else
                return false;
        }
        else {
            false;
        }
    }

    Class classM() {
        string className;
        vector<Parameter> classMembers;
        if (tokenType() == KEYWORD && peek() == "class") {
            match(KEYWORD);
            className = match(IDENTIFIER);
            match(BRACE);
            while (tokenType() != BRACE && nextTokenType() != TERMINATOR) {
                if (isMember()) {//for class members
                    vector<vector<Parameter>> temp = variableDeclarationList();
                    for (auto list : temp) {
                        for (auto param : list) {
                            classMembers.push_back(param);
                        }
                    }
                }
                else if (isMethod()){//for method building
                    matchUntil(BRACE);
                    match(BRACE);
                    matchUntil(BRACE);
                    match(BRACE);
                }
                else {
                    if (tokenType() == COMMENT) {
                        matchAll();
                        continue;
                    }
                    matchAll();//for testing purposes
                    //throwError();
                }
            }
            match(BRACE);
            match(TERMINATOR);
            return Class(className, classMembers);
        }
        else {
            throwError();
        }
    }

    vector<Class> classList() {
        vector<Class> list;
        if (tokenType() == KEYWORD && peek() == "class") {
            list.push_back(classM());
            return classList(list);
        }
        else {
            return list;
        }
    }
    vector<Class> classList(vector<Class> list) {
        if (tokens.size() == 0) {
            return list;
        }
        if (tokenType() == KEYWORD && peek() == "class") {
            list.push_back(classM());
            return classList(list);
        }
        else {
            return list;
        }
    }


public:

    Parser (vector<Token> tokens)
    : tokens(tokens) {}

    void startParsing() {
        vector<string> includes;
        vector<string> macros;
        vector<string> typeDefs;
        vector<Struct> structs;
        vector<Class> classes;
        int i = 0;
        while (!tokens.empty()) {
            if (tokenType() == PREPROC) {
                if (peek() == "#include") {
                    vector<string> tempIncludes = includeList();
                    for (auto include : tempIncludes) {
                        includes.push_back(include);
                    }
                }
                else {
                    vector<string> tempMacros = macroList();
                    for (auto macro : tempMacros) {
                        macros.push_back(macro);
                    }
                }
            }
            if (tokenType() == KEYWORD) {
                if (peek() == "typedef") {
                    vector<string> tempTypeDefs = typedefList();
                    for (auto typDef : tempTypeDefs) {
                        typeDefs.push_back(typDef);
                    }
                }
                if (peek() == "struct") {
                    vector<Struct> tempStructs = structList();
                    for (auto strucT : tempStructs) {
                        structs.push_back(strucT);
                    }
                }
                if (peek() == "class") {
                    vector<Class> tempClasses = classList();
                    for (auto clasS : tempClasses) {
                        classes.push_back(clasS);
                    }
                }
            }
            i++;
        }

        for (auto include : includes) {
            cout << include << endl;
        }
        for (auto define : macros) {
            cout << define << endl;
        }
        for (auto typeDef : typeDefs) {
            cout << typeDef << endl;
        }
        for (auto strucT : structs) {
            cout << strucT << endl;
        }
        for (auto clasS : classes) {
            cout << clasS << endl;
        }

        for (size_t i = 0; i < tokens.size(); i++) {
            cout << tokens.at(i) << endl;
        }

    }

};


#endif //CWITHCLASSES_PARSER_H
