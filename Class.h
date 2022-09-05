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
            if (member.getName() == className) {
                out << "\t" << "struct " << member << ";" << endl;
            }
            else {
                out << "\t" << member << ";" << endl;
            }
        }
        for (auto method : methods) {
            out << method.pointerForm() << endl;
        }
        out << "} " << className << ";" << endl;
    }
public:

    string makeSource() {
        stringstream out;
        out << "#include \"" << className << ".h\"" << endl << endl;

        out << constructor.functionForm() << endl << endl;
        out << constructorPointer.functionForm() << endl << endl;
        out << deconstructor.functionForm() << endl << endl;

        for (auto method : methods) {
            out << method.functionForm() << endl << endl;
        }
    }

    string makeHeader() {
        stringstream out;

        out << makeStruct() << endl << endl;

        out << constructor.definitionForm() << endl << endl;
        out << constructorPointer.definitionForm() << endl << endl;
        out << deconstructor.definitionForm() << endl << endl;

    }
};


#endif //CWITHCLASSES_CLASS_H
