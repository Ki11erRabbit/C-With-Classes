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
    vector<pair<string,string>> classData;
    vector<MethodStructure> methods;
    MethodStructure constructor;
    MethodStructure deconstructor;
    vector<string> preProcs;
    vector<string> includes;

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
        for (auto include : includes) {
            out << include << endl;
        }
        out << endl;

        //makes the struct needed for the class
        out << "typedef struct " << className << "{" << endl;
        for (auto data : classData) {
            out << "\t" << data << ";" << endl;
        }
        out << "\t" << constructor.printPointerForm();
        out << "\t" << deconstructor.printPointerForm();
        for (auto method : methods) {
            out << "\t" << method.printPointerForm();
        }

        out << "} " << className << ";" << endl;
        out << endl;

        //outputs function definitions needed for header;
        out << constructor.printFunctionDef(this) << endl;
        out << deconstructor.printFunctionDef(this) << endl;
        for (auto method : methods) {
            out << method.printFunctionDef(this) << endl;
        }

    }
    string buildSource() {
        stringstream out;
        out << "#include \"" << className << ".h\"" << endl << endl;
        for (auto preProc : preProcs) {
            out << preProc << endl;
        }
        out << endl;

        out << constructor.printFunctionForm(this) << endl << endl;
        out << constructor.printFunctionForm(this) << endl << endl;
        for (auto method : methods) {
            out << method.printFunctionForm(this) << endl << endl;
        }

    }
    string toString() {




    }

};


#endif //CWITHCLASSES_CLASSSTRUCTURE_H
