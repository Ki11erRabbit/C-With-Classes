//
// Created by ki11errabbit on 9/8/22.
//

#ifndef CWITHCLASSES_ENUM_H
#define CWITHCLASSES_ENUM_H

#include <string>
#include <vector>
#include <iostream>

#include "Parameter.h"

using namespace std;

class Enum {
private:
    string name;
    vector<Parameter> constants;
    string variable;

public:
    Enum(const string &name, const vector<Parameter> &constants)
    : name(name), constants(constants) {}
    Enum(const string &name, const vector<Parameter> &constants, string variable)
            : name(name), constants(constants), variable(variable) {}


    string toString() const{
        stringstream out;
        out << "enum " << name << "{" << endl;
        for (size_t i = 0; i < constants.size(); i++) {
            out << constants.at(i);
            if (i < constants.size() - 1) {
                out << ", ";
            }
        }
        out << endl << "}" << variable << ";";

        return out.str();
    }

    friend std::ostream& operator<<(std::ostream& os, const Enum& enuM)
    {
        os << enuM.toString();
        return os;
    }
};


#endif //CWITHCLASSES_ENUM_H
