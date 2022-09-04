//
// Created by ki11errabbit on 9/4/22.
//

#ifndef CWITHCLASSES_METHODSTRUCTURE_H
#define CWITHCLASSES_METHODSTRUCTURE_H

#include <string>
#include <vector>
#include <sstream>

#include "MethodParameter.h"

using namespace std;

class MethodStructure {
private:
    string type;
    string methodName;
    vector<MethodParameter> parameters;
    vector<string> codeBody;

public:

    string printPointerForm() {
        stringstream out;
        out << type << " (*" << methodName << ")";
        for (size_t i = 0; i < parameters.size(); i++) {
            if (i == 0) {
                out << "(";
            }
            out << parameters.at(i).getType();
            if (i < parameters.size() -1) {
                out << ", ";
            }
        }
        out << ");" << endl;
    }

};


#endif //CWITHCLASSES_METHODSTRUCTURE_H
