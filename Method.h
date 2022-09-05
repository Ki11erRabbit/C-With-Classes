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
    string returnType;
    string methodName;
    string className;
    vector<Parameter> parameters;
    CodeBlock body;

public:
    string pointerForm() {
        stringstream out;

        out << returnType << " (*" << methodName << ")(";
        for (size_t i = 0; i < parameters.size(); i++) {
            out << parameters.at(i).getType();
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
