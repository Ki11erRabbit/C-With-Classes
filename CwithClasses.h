//
// Created by ki11errabbit on 9/9/22.
//

#ifndef CWITHCLASSES_CWITHCLASSES_H
#define CWITHCLASSES_CWITHCLASSES_H

#include <string>
#include <vector>
#include <iostream>

#include "Enum.h"
#include "Struct.h"
#include "Function.h"
#include "Class.h"

using namespace std;
class CwithClasses {
private:
    vector<string> includes;
    vector<string> macros;
    vector<string> typeDefs;
    vector<Enum> enums;
    vector<Struct> structs;
    vector<Function> functions;
    vector<Class> classes;
    string name;

public:
    CwithClasses(const vector<string> &includes, const vector<string> &macros, const vector<string> &typeDefs,
                 const vector<Enum> &enums, const vector<Struct> &structs, const vector<Function> &functions,
                 const vector<Class> &classes) : includes(includes), macros(macros), typeDefs(typeDefs), enums(enums),
                                                 structs(structs), functions(functions), classes(classes) {}

    void tempFunction() {

    }

    const string &getName() const {
        return name;
    }

    void setName(const string &name) {
        CwithClasses::name = name;
    }

    const vector<Class> &getClasses() const {
        return classes;
    }


    void setupInheritance(vector<CwithClasses> files) {
        vector<Class> classesFromFiles;
        for (auto file : files) {
            for (auto clasS : file.getClasses()) {
                classesFromFiles.push_back(clasS);
            }
        }
        for (auto clasS : classes) {
            clasS.findInheritance(classesFromFiles);
        }

    }

    string makeHeaderFile(){
        stringstream out;
        string headerName;
        /*for (auto clasS : classes) {
            for (size_t i = 0; i < clasS.getClassName().length(); i++) {
                headerName.push_back(toupper(clasS.getClassName()[i]));
            }
        }*/
        for (size_t i = 0; i < name.length(); i++) {
            headerName.push_back(toupper(name[i]));
        }

        out << "#ifndef " << headerName << endl;
        out << "#define " << headerName << endl << endl;

        for (auto include : includes) {
            out << include << endl;
        }
        for (auto macro : macros) {
            out << macro << endl;
        }
        for (auto typeDef : typeDefs) {
            out << typeDef << endl;
        }
        for (auto enuM : enums) {
            out << enuM << endl;
        }
        for (auto strucT : structs) {
            out << strucT << endl;
        }
        for (auto clasS : classes) {
            out << clasS.makeHeader();
        }
        for (auto function : functions) {
            out << function.definitionForm(name) << endl;
        }

        out << endl << "#endif" << endl;

        return out.str();
    }

    string makeSourceFile() {
        stringstream out;

        out << "#include \"" << name << ".h\"" << endl << endl;

        for (auto macro : macros) {
            out << macro << endl;
        }
        for (auto clasS : classes) {
            out << clasS.makeSource() << endl;
        }
        for (auto function : functions) {
            out << function.functionForm(name) << endl;
        }

        return out.str();
    }

};


#endif //CWITHCLASSES_CWITHCLASSES_H
