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
#include "Method.h"
#include "Enum.h"
#include "Function.h"
#include "CwithClasses.h"

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
        cout << tokens.size() << " tokens remaining" << endl;
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

        if (tokenType() == SPECIALCHAR && subTokenType() == OPEN) {//type casts or nested expressions
            statement += match(SPECIALCHAR);
            if (tokenType() == KEYWORD) {//typecast
                if (peek() == "struct") {
                    statement += match(KEYWORD);
                    statement += " " + match(IDENTIFIER);
                }
                else {
                    statement += match(KEYWORD);
                }
                if (tokenType() == OPERATOR) {//pointer casts
                    statement += match(OPERATOR);
                }
                if (tokenType() == OPERATOR) {//pointer of pointers casts
                    statement += match(OPERATOR);
                }
            }
            else {//nested expressions
                statement += expression();
            }
            statement+= match(SPECIALCHAR);//closing paren
        }
        else if (tokenType() == OPERATOR) {//operators
            if (subTokenType() == SIZEOF) {
                statement += sizeOf();
            }
            else {
                statement += match(OPERATOR);
            }
        }
        else if (tokenType() == BRACE) {
            statement += match(BRACE);
            vector<string> list = matchUntil(BRACE);
            for (auto item : list) {
                statement += item;
            }
            statement += match(BRACE);
        }
        else if (tokenType() == CONSTANT) {
            statement += match(CONSTANT);
        }
        else if (tokenType() == STRING) {
            statement += match(STRING);
        }
        else if (tokenType() == IDENTIFIER) {
            if (nextTokenType() == SPECIALCHAR && nextSubTokenType() == OPEN) {
                statement += functionCall();
            }
            else {
                statement += match(IDENTIFIER);
            }

        }
        else if (tokenType() == KEYWORD) {
            if (peek() == "return") {
                statement += match(KEYWORD) + " ";
            }
        }
        else if (tokenType() == SPECIALCHAR && peek() == "["){
            statement += match(SPECIALCHAR);
            statement += expression();
            statement += match(SPECIALCHAR);
        }

        if (subTokenType() == CLOSE || subTokenType() == COMMA || tokenType() == TERMINATOR) {
            if (subTokenType() == COMMA) {
                match(OPERATOR);
            }

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
                if (tokenType() == KEYWORD && subTokenType() == TYPE) {
                    type += " ";
                }
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
                    if (tokenType() == IDENTIFIER && nextTokenType() == SPECIALCHAR) {//function
                        contents = functionCall();
                    }
                    else {
                        contents = expression();
                    }
                }
                else {
                    //match(OPERATOR);
                }
                if (tokenType() == OPERATOR && subTokenType() == COMMA) {
                    match(OPERATOR);
                    varList.push_back(Parameter(varType,pointer,varName,contents));
                    return variableDeclaration(varList, varType);
                }

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
                else {
                    contents = expression();
                }
            }
            else {
                match(OPERATOR);
            }
            if (tokenType() == OPERATOR && subTokenType() == COMMA) {
                varList.push_back(Parameter(varType,pointer,varName,contents));
                return variableDeclaration(varList, varType);
            }
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

    vector<vector<Parameter>> variableDeclarationList() {
        vector<vector<Parameter>> list;
        if (tokenType() == KEYWORD && subTokenType() == TYPE && isMember()) {
            list.push_back(variableDeclaration());
            return variableDeclarationList(list);
        }
        else {
            return list;
        }
    }
    vector<vector<Parameter>> variableDeclarationList(vector<vector<Parameter>> list) {
        if (tokenType() == KEYWORD && subTokenType() == TYPE && isMember()) {
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
                if (this->tokens.at(i).getValue() == identifier && this->tokens.at(i+1).getValue() != "(") {
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
        if (tokens.at(i).getType() == OPERATOR) {//pointer
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
        if (tokens.at(i).getType() == OPERATOR) {//pointer
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
    bool isFunction() {
        if (tokens.empty())
            return false;
        size_t i = 0;
        while (tokens.at(i).getSubType() == TYPE) {
            i++;
        }
        if (tokens.at(i).getType() == OPERATOR) {//pointer
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

    Parameter methodParameter() {
        string parameterType, parameterName, parameterPointer;
        if (tokenType() == KEYWORD) {
            parameterType = type();

            if (tokenType() == OPERATOR) {
                parameterPointer = match(OPERATOR);
            }

            parameterName = match(IDENTIFIER);
            return Parameter(parameterType, parameterPointer,parameterName);
        }
        else if (tokenType() == OPERATOR && subTokenType() == TYPE) {//for variable length arguments
            parameterName = match(OPERATOR);
            if (tokenType() != SPECIALCHAR) //for when ... is not the last parameter
                throwError();
            return Parameter("", parameterName);
        }
        else {
            throwError();
        }
    }

    vector<Parameter> methodParameterList() {
        vector<Parameter> list;
        if (tokenType() == KEYWORD) {
            list.push_back(methodParameter());
            return methodParameterList(list);
        }
        else if (tokenType() == OPERATOR && subTokenType() == COMMA) {// for commas
            match(OPERATOR);
            list.push_back(methodParameter());
            return methodParameterList(list);
        }
        else if (tokenType() == OPERATOR && subTokenType() == TYPE) {//for variable length functions
            list.push_back(methodParameter());
            return methodParameterList(list);
        }
        else {
            return list;
        }
    }
    vector<Parameter> methodParameterList(vector<Parameter> list) {
        if (tokenType() == KEYWORD) {
            list.push_back(methodParameter());
            return methodParameterList(list);
        }
        else if (tokenType() == OPERATOR && subTokenType() == COMMA) {// for commas
            match(OPERATOR);
            list.push_back(methodParameter());
            return methodParameterList(list);
        }
        else if (tokenType() == OPERATOR && subTokenType() == TYPE) {//for variable length functions
            list.push_back(methodParameter());
            return methodParameterList(list);
        }
        else {
            return list;
        }
    }

    string controlFlow() {// for ifs, else, for, while, switch
        string statement;
        if (tokenType() == KEYWORD) {
            if (peek() == "for") {
                statement += match(KEYWORD);
                statement += match((SPECIALCHAR));
                if (tokenType() == KEYWORD || tokenType() == IDENTIFIER) {
                    vector<vector<Parameter>> vars = variableDeclarationList();
                    for (size_t i = 0; i < vars.size(); i++) {
                        for (size_t j = 0; j < vars.at(i).size(); j++) {
                            statement += vars.at(i).at(j).toString();
                            if (i < vars.size()-1 && j < vars.at(i).size()-1) {
                                statement += ",";
                            }
                        }
                    }
                }
                //statement += match(TERMINATOR);
                statement += ";";
                if (tokenType() == IDENTIFIER || tokenType() == CONSTANT) {
                    statement += expression();
                }

                statement += match(TERMINATOR);
                while (tokenType() != SPECIALCHAR) {
                    statement += expression() + ",";
                }
                statement.pop_back();//removes the extra , at the end
                statement += match(SPECIALCHAR);

            }
            if (peek() == "else") {
                statement += match(KEYWORD);
            }
            else if (peek() == "switch") {
                statement += match(KEYWORD);
                statement += match(SPECIALCHAR);
                statement += expression();
                statement += match(SPECIALCHAR);
            }
            else if (peek() == "while" || peek() == "if"){//for while and if
                statement += match(KEYWORD);
                statement += match(SPECIALCHAR);
                statement += expression();
                statement += match(SPECIALCHAR);
            }
            return statement;
        }
        else {
            throwError();
        }
    }

    CodeBlock codeBlock() {
        vector<Parameter> variables;
        vector<CodeBlock> codeBlocks;
        vector<Enum> enums;
        vector<string> lines;
        string line;
        string statement;

        if (tokenType() == BRACE) {
            match(BRACE);

            do {//makes sure all variables outside of codeblocks are declared
                if (tokenType() == KEYWORD && subTokenType() == TYPE) {
                    if (peek() == "enum") {
                        enums.push_back(enumM());
                        match(TERMINATOR);
                    }
                    vector<vector<Parameter>> temp = variableDeclarationList();
                    for (auto list : temp) {
                        for (auto param : list) {
                            variables.push_back(param);
                        }
                    }
                    /*if (enums.empty()){
                        match(TERMINATOR);
                    }*/
                }
                if (tokenType() == KEYWORD && subTokenType() == CONTROL) {
                    if (peek() == "case" || peek() == "default") {//for within switch statements
                        line += match(KEYWORD);
                        if (tokenType() == CONSTANT) {
                            line += " " + match(CONSTANT);
                        }
                        line += match(OPERATOR);
                        lines.push_back(line);
                        line = "";
                    }
                    else if (peek() == "break" || peek() == "continue") {
                        line += match(KEYWORD);
                        line += match(TERMINATOR);
                        lines.push_back(line);
                        line = "";
                    }
                    else {// for conditionals
                        if (peek() == "do") {
                            lines.push_back(match(KEYWORD));
                            lines.push_back("{}");
                            codeBlocks.push_back(codeBlock());
                            lines.push_back(controlFlow());
                        }
                        else {
                            lines.push_back(controlFlow());
                            if (tokenType() == BRACE) {
                                lines.push_back("{}");
                                codeBlocks.push_back(codeBlock());
                            }
                            else {//not sure why commenting this out works but ok
                                //lines.push_back(expression());
                            }
                        }

                    }
                }
                else if (tokenType() == BRACE && subTokenType() == OPEN) {
                    lines.push_back("{}");
                    codeBlocks.push_back(codeBlock());
                }
                else if (tokenType() != BRACE){//statements and whatnot
                    line = expression();
                    line += match(TERMINATOR);
                    lines.push_back(line);
                    line = "";
                }
                if (tokens.empty()) {
                    break;
                }
            } while ((tokenType() != BRACE && subTokenType() != CLOSE));
            match(BRACE);
            return CodeBlock(statement, variables, enums, lines, codeBlocks);
        }
        else {
            throwError();
        }

    }

    Method method() {
        string returnType, methodName;
        vector<Parameter> parameters;
        if (tokenType() == KEYWORD) {
            returnType = type();
            if (peek() == "*") {
                returnType += match(OPERATOR);
            }
            methodName = match(IDENTIFIER);
            match(SPECIALCHAR);

            parameters = methodParameterList();

            match(SPECIALCHAR);
            CodeBlock body = codeBlock();

            return Method(returnType, methodName, parameters, body);
        }
        else {
            throwError();
        }
    }

    vector<Method> methodList() {
        vector<Method> list;
        if (tokenType() == KEYWORD) {
            list.push_back(method());
            return methodList(list);
        }
        else {
            return list;
        }
    }
    vector<Method> methodList(vector<Method> list) {
        if (tokenType() == KEYWORD) {
            list.push_back(method());
            return methodList(list);
        }
        else {
            return list;
        }
    }

    Function function() {
        string returnType, functionName;
        vector<Parameter> parameters;
        if (tokenType() == KEYWORD && isFunction()) {
            returnType = type();
            functionName = match(IDENTIFIER);
            match(SPECIALCHAR);

            parameters = methodParameterList();

            match(SPECIALCHAR);
            CodeBlock body = codeBlock();

            return Function(returnType, functionName, parameters, body);
        }
        else {
            throwError();
        }
    }

    vector<Function> functionList() {
        vector<Function> list;
        if (!tokens.empty() && tokenType() == KEYWORD && isFunction()) {
            list.push_back(function());
            return functionList(list);
        }
        else {
            return list;
        }
    }
    vector<Function> functionList(vector<Function> list) {
        if (!tokens.empty() && tokenType() == KEYWORD && isFunction()) {
            list.push_back(function());
            return functionList(list);
        }
        else {
            return list;
        }
    }

    Class classM() {
        string className;
        vector<Parameter> classMembers;
        vector<Method> classMethods;
        if (tokenType() == KEYWORD && peek() == "class") {
            match(KEYWORD);
            className = match(IDENTIFIER);
            updateType(className);
            match(BRACE);
            while (tokenType() != BRACE && nextTokenType() != TERMINATOR) {
                if (isMember()) {//for class members
                    vector<vector<Parameter>> temp = variableDeclarationList();
                    if (tokenType() == COMMENT) {
                        match(COMMENT);
                    }
                    //match(TERMINATOR);
                    for (auto list : temp) {
                        for (auto param : list) {
                            classMembers.push_back(param);
                        }
                    }
                }
                else if (isMethod()){//for method building
                    /*matchUntil(BRACE);
                    match(BRACE);
                    matchUntil(BRACE);
                    match(BRACE);*/
                    vector<Method> tempMethod = methodList();
                    for (auto method : tempMethod) {
                        classMethods.push_back(method);
                    }

                }
                else {
                    if (tokenType() == COMMENT) {
                        matchAll();
                        continue;
                    }
                    matchAll();//for testing purposes in the future it should match a TERMINATOR
                    //throwError();
                }
            }
            match(BRACE);
            match(TERMINATOR);
            return Class(className, classMembers, classMethods);
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

    void updateType(string constant, TokenType type1, SubTokenType type2) {
        for (size_t i = 0; i < tokens.size(); i++) {
            if (this->tokens.at(i).getType() == IDENTIFIER) {
                if (this->tokens.at(i).getValue() == constant) {
                    this->tokens.at(i).setType(type1);
                    this->tokens.at(i).setSubType(type2);
                }
            }
        }
    }

    Enum enumM() {
        string name;
        string variable;
        vector<Parameter> enumVals;
        if (tokenType() == KEYWORD && peek() == "enum") {
            match(KEYWORD);
            name = match(IDENTIFIER);
            updateType(name);
            match(BRACE);
            while (tokenType() != BRACE) {
                string enumValName, enumValue;

                enumValName = match(IDENTIFIER);
                updateType(enumValName,CONSTANT,NONE);
                if (tokenType() == OPERATOR && subTokenType() == ASSIGNMENT) {
                    match(OPERATOR);
                    enumValue = match(CONSTANT);
                }
                enumVals.push_back(Parameter("","",enumValName,enumValue));
                if (tokenType() == OPERATOR) {
                    match(OPERATOR);
                }
            }
            match(BRACE);
            if (tokenType() == IDENTIFIER) {
                variable = match(IDENTIFIER);
            }
            match(TERMINATOR);
            return Enum(name,enumVals,variable);
        }
        else {
            throwError();
        }
    }
    vector<Enum> enumList() {
        vector<Enum> list;
        if (tokenType() == KEYWORD && peek() == "enum") {
            list.push_back(enumM());
            return enumList(list);
        }
        else {
            return list;
        }
    }
    vector<Enum> enumList(vector<Enum> list) {
        if (tokenType() == KEYWORD && peek() == "enum") {
            list.push_back(enumM());
            return enumList(list);
        }
        else {
            return list;
        }
    }



public:

    Parser (vector<Token> tokens)
    : tokens(tokens) {}

    CwithClasses startParsing() {
        vector<string> includes;
        vector<string> macros;
        vector<string> typeDefs;
        vector<Enum> enums;
        vector<Struct> structs;
        vector<Function> functions;
        vector<Class> classes;
        size_t i = 0;
        while (!tokens.empty()) {
            if (!tokens.empty() && tokenType() == PREPROC) {
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
            if (!tokens.empty() && tokenType() == KEYWORD) {
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
                if (peek() == "enum") {
                    vector<Enum> tempEnums = enumList();
                    for (auto enuM : tempEnums) {
                        enums.push_back(enuM);
                    }
                }
                if (peek() == "class") {
                    vector<Class> tempClasses = classList();
                    for (auto clasS : tempClasses) {
                        classes.push_back(clasS);
                    }
                }
                if (isFunction()) {
                    vector<Function> tempFunctions = functionList();
                    for (auto function : tempFunctions) {
                        functions.push_back(function);
                    }
                }
            }
            if (!tokens.empty() && tokenType() == COMMENT) {
                match(COMMENT);
            }
            //cout << ++i << " Passes" << endl;
        }

        /*for (auto include : includes) {
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
        for (auto enuM : enums) {
            cout << enuM << endl;
        }
        for (auto clasS : classes) {
            cout << clasS << endl;
        }

        for (size_t i = 0; i < tokens.size(); i++) {
            cout << tokens.at(i) << endl;
        }*/

        //cout << classes.at(0).makeHeader() << endl << endl;

        //cout <<classes.at(0).makeSource();
        return CwithClasses(includes,macros,typeDefs,enums,structs,functions,classes);
    }

};


#endif //CWITHCLASSES_PARSER_H
