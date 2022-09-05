//
// Created by ki11errabbit on 9/4/22.
//

#ifndef CWITHCLASSES_CLASS_H
#define CWITHCLASSES_CLASS_H

#include <string>
#include <vector>
#include <iostream>

#include "Parameter.h"
#include "Method.h"

using namespace std;

class Class {
private:
    string className;
    vector<Parameter> members;
    vector<Method> methods;
    Method constructor;
    Method constructorPointer;
    Method deconstructor;

    string makeStruct() {
        stringstream out;
        out << "typedef struct " << className << " {" << endl;
        for (auto member : members) {
            out << "\t" << member << ";" << endl;
        }
        out << "} " << className << ";" << endl;
    }
public:

    string makeSource() {

    }

    string makeHeader() {

    }
};


#endif //CWITHCLASSES_CLASS_H
