//
// Created by ki11errabbit on 9/4/22.
//

#ifndef CWITHCLASSES_METHODSTRUCTURE_H
#define CWITHCLASSES_METHODSTRUCTURE_H

#include <string>
#include <vector>
#include <sstream>

#include "MethodParameter.h"
#include "ClassStructure.h"

using namespace std;

class MethodStructure {
private:
    string type;
    string methodName;
    vector<MethodParameter> parameters;
    vector<string> codeBody;

public:

    string printPointerForm(ClassStructure classStruct) {
        stringstream out;
        out << type << " (*" << methodName << ")";
        out << "(" << "struct *" << classStruct.getName() << ", ";
        for (size_t i = 0; i < parameters.size(); i++) {
            out << parameters.at(i).getType();
            if (i < parameters.size() -1) {
                out << ", ";
            }
        }
        out << ");" << endl;
    }

    string printFunctionForm(ClassStructure classStruct) {
        stringstream out;

        out << type << " " << classStruct.getName() << "__" << methodName << "(" << "struct *" << classStruct.getName() << " this, ";
        for (size_t i = 0; i < parameters.size(); i++) {
            out << parameters.at(i).getType() << " " << parameters.at(i).getName();
            if (i < parameters.size() -1) {
                out << ", ";
            }
        }
        out << ") {" << endl;

        for (auto line : codeBody) {
            out << line << endl;
        }
        out << "}" << endl;
    }

    string printFunctionDef(ClassStructure classStruct) {
        stringstream out;

        out << type << " " << classStruct.getName() << "__" << methodName << "(" << "struct *" << classStruct.getName() << " this, ";
        for (size_t i = 0; i < parameters.size(); i++) {
            out << parameters.at(i).getType() << " " << parameters.at(i).getName();
            if (i < parameters.size() -1) {
                out << ", ";
            }
        }
        out << ");" << endl;
    }

};


#endif //CWITHCLASSES_METHODSTRUCTURE_H
