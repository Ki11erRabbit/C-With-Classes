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
    string returnType, methodName, className;
    vector<Parameter> parameters;
    CodeBlock body;

public:
    Method(string returnType, string methodName, vector<Parameter> parameters,CodeBlock body)
    : returnType(returnType),methodName(methodName),parameters(parameters), body(body) {}

    string pointerForm() {
        stringstream out;

        out << returnType << " (*" << methodName << ")(";
        for (size_t i = 0; i < parameters.size(); i++) {
            string param = parameters.at(i).getType();
            if (param.find(className) < param.size()) {
                out << "struct " << param;
            }
            else {
                out << param;
            }
            if (i < parameters.size()-1)
                out << ", ";
        }
        out << ");";

        return out.str();
    }

    string functionForm() {
        stringstream out;

        out << returnType << className << "__" << methodName << "(";
        for (size_t i = 0; i < parameters.size(); i++) {
            out << parameters.at(i).getType() << " " << parameters.at(i).getName();
            if (i < parameters.size()-1)
                out << ", ";
        }
        out << ") " << body;

        return out.str();
    }

    string definitionForm() {
        stringstream out;

        out << returnType << className << "__" << methodName << "(";
        for (size_t i = 0; i < parameters.size(); i++) {
            out << parameters.at(i).getType() << " " << parameters.at(i).getName();
            if (i < parameters.size()-1)
                out << ", ";
        }
        out << ")";

        return out.str();
    }
};


#endif //CWITHCLASSES_METHOD_H
