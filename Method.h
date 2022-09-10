//
// Created by ki11errabbit on 9/4/22.
//

#ifndef CWITHCLASSES_METHOD_H
#define CWITHCLASSES_METHOD_H

#include <string>
#include <vector>
#include <iostream>

#include "Token.h"
#include "Parameter.h"
#include "CodeBlock.h"

using namespace std;
class Method {
private:
    string returnType, methodName;
    vector<Parameter> parameters;
    CodeBlock body;

public:
    Method() {
        returnType = "";
        methodName = "";
        vector<string> temp = {""};
        body = CodeBlock(temp);
    }
    Method(string returnType, string methodName, vector<Parameter> parameters,CodeBlock body)
    : returnType(returnType),methodName(methodName),parameters(parameters), body(body) {}

    Method(string returnType, string methodName,CodeBlock body)
            : returnType(returnType),methodName(methodName), body(body) {}

    string getFunctionName(string className) {
        return className + "__" + methodName;
    }
    string getName() {
        return methodName;
    }

    string pointerForm(string className){
        stringstream out;

        out << returnType << " (*" << methodName << ")(";
        out << "struct " << className << "*";
        if (!parameters.empty())
            out << ", ";
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

        out << returnType << " " << className << "__" << methodName << "(";
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

        out << returnType << " " << className << "__" << methodName << "(";
        out << className << "* " << "this";
        if (!parameters.empty())
            out << ", ";
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

        out << returnType << " " << className << "__" << methodName << "(";
        out << className << "* " << "this";
        if (!parameters.empty())
            out << ", ";
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

        out << returnType << " " << className << "__" << methodName << "(";

        for (size_t i = 0; i < parameters.size(); i++) {
            out << parameters.at(i).getType() << " " << parameters.at(i).getPointer() << parameters.at(i).getName();
            if (i < parameters.size()-1)
                out << ", ";
        }
        out << ")";

        return out.str();
    }
};


#endif //CWITHCLASSES_METHOD_H
