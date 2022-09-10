//
// Created by ki11errabbit on 9/9/22.
//

#ifndef CWITHCLASSES_FUNCTION_H
#define CWITHCLASSES_FUNCTION_H

#include <string>
#include <vector>
#include <iostream>

#include "Token.h"
#include "Parameter.h"
#include "CodeBlock.h"


class Function {
private:
    string returnType, functionName;
    vector<Parameter> parameters;
    CodeBlock body;

public:
    Function() {
        returnType = "";
        functionName = "";
        vector<string> temp = {""};
        body = CodeBlock(temp);
    }
    Function(string returnType, string functionName, vector<Parameter> parameters,CodeBlock body)
    : returnType(returnType),functionName(functionName),parameters(parameters), body(body) {}

    Function(string returnType, string functionName,CodeBlock body)
    : returnType(returnType),functionName(functionName), body(body) {}

    string getFunctionName() {
        return functionName;
    }
    string getName() {
        return functionName;
    }

    string pointerForm(){
        stringstream out;

        out << returnType << " (*" << functionName << ")(";
        for (size_t i = 0; i < parameters.size(); i++) {
            string param = parameters.at(i).getType() + parameters.at(i).getPointer();
            out << param;
            if (i < parameters.size()-1)
                out << ", ";
        }
        out << ");";

        return out.str();
    }
    string functionFormPlain(string className) {
        stringstream out;

        out << returnType << " " << functionName << "(";
        for (size_t i = 0; i < parameters.size(); i++) {
            out << parameters.at(i).getType() << " " << parameters.at(i).getPointer()  << parameters.at(i).getName();
            if (i < parameters.size()-1)
                out << ", ";
        }
        out << ") " << body;

        return out.str();
    }

    string functionForm(string className) {
        stringstream out;

        out << returnType << " " << functionName << "(";
        for (size_t i = 0; i < parameters.size(); i++) {
            out << parameters.at(i).getType() << " " << parameters.at(i).getPointer()  << parameters.at(i).getName();
            if (i < parameters.size()-1)
                out << ", ";
        }
        out << ") " << body;

        return out.str();
    }

    string definitionForm(string className) {
        stringstream out;

        out << returnType << " " << functionName << "(";
        for (size_t i = 0; i < parameters.size(); i++) {
            out << parameters.at(i).getType() << " " << parameters.at(i).getPointer()  << parameters.at(i).getName();
            if (i < parameters.size()-1)
                out << ", ";
        }
        out << ")";

        return out.str();
    }
    string definitionFormPlain(string className) {
        stringstream out;

        out << returnType << " " << functionName << "(";

        for (size_t i = 0; i < parameters.size(); i++) {
            out << parameters.at(i).getType() << " " << parameters.at(i).getPointer() << parameters.at(i).getName();
            if (i < parameters.size()-1)
                out << ", ";
        }
        out << ")";

        return out.str();
    }

};


#endif //CWITHCLASSES_FUNCTION_H
