//
// Created by ki11errabbit on 9/5/22.
//

#ifndef CWITHCLASSES_STRUCT_H
#define CWITHCLASSES_STRUCT_H

#include <string>
#include <vector>
#include <iostream>

#include "Parameter.h"

using namespace std;

class Struct {
private:
    string name;
    vector<Parameter> body;
public:
    Struct(string name, vector<Parameter> body)
    : name(name), body(body) {};

    Struct(string name)
            : name(name) {};

    void addBody(Parameter param) {
        body.push_back(param);
    }


    string toString() const{
        stringstream out;
        out << "struct " << name << " {" << endl;

        for (auto var : body) {
            out << var << ";" << endl;
        }
        out << "}";
        return out.str();
    }
};


#endif //CWITHCLASSES_STRUCT_H
