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
    vector<string> preProcs;

public:

    typedef struct Node{
        char* data;
        struct Node* next;
        struct Node* previous;
    } Node;

    string getName() const {
        return className;
    }

    string buildHeader() {
        stringstream out;

        //puts all includes in the headerfile
        for (auto preProc : preProcs) {
            out << preProc << endl;
        }
        out << endl;

        //makes the struct needed for the class
        out << "typedef struct " << className << "{" << endl;
        for (auto data : classData) {
            out << "\t" << data << ";" << endl;
        }
        for (auto method : methods) {
            out << "\t" << method.printPointerForm();
        }

        out << "} " << className << ";" << endl;
        out << endl;

        //outputs function definitions needed for header;
        for (auto method : methods) {
            out << method.printFunctionDef(this) << endl;
        }

    }
    string buildSource() {
        stringstream out;

    }
    string toString() {




    }

};


#endif //CWITHCLASSES_CLASSSTRUCTURE_H
