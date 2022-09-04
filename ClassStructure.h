//
// Created by ki11errabbit on 9/4/22.
//

#ifndef CWITHCLASSES_CLASSSTRUCTURE_H
#define CWITHCLASSES_CLASSSTRUCTURE_H

#include <string>
#include <vector>
#include <sstream>

#include "MethodStructure.h"
using namespace std;

class ClassStructure {
private:
    string className;
    vector<string> classData;
    vector<MethodStructure> methods;

public:

    typedef struct Node{
        char* data;
        struct Node* next;
        struct Node* previous;
    } Node;
    string toString() {
        stringstream out;

        out << "typedef struct " << className << "{" << endl;
        for (auto data : classData) {
            out << "\t" << data << ";" << endl;
        }
        for (auto method : methods) {
            out << "\t" << method.printPointerForm();
        }

        out << "} " << className << ";" << endl;

    }

};


#endif //CWITHCLASSES_CLASSSTRUCTURE_H
