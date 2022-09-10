//
// Created by ki11errabbit on 9/4/22.
//

#ifndef CWITHCLASSES_CLASS_H
#define CWITHCLASSES_CLASS_H

#include <string>
#include <vector>
#include <iostream>
#include <locale>

#include "Parameter.h"
#include "Method.h"
//#include "Method.h"

using namespace std;

class Class {
private:
    string className;
    vector<Parameter> members;
    vector<Method> methods;
    Method defaultConstructor;
    Method pointerConstructor;
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
            out << method.pointerForm(className) << endl;
        }
        out << "} " << className << ";" << endl;

        return out.str();
    }

    Method createDefaultConstructor() {
        vector<Parameter> memberNeedInit;
        for (size_t i = 0; i < members.size(); i++) {
            if (members.at(i).getStoredValue() != "") {
                memberNeedInit.push_back(members.at(i));
                members.at(i).setStoredValue("");
            }
        }
        vector<string> codeBlock;
        codeBlock.push_back(className + " newObject;");
        for (auto member : memberNeedInit) {
            codeBlock.push_back(className + "." + member.printWOType() + ";");
        }

        for (auto method : methods) {
            codeBlock.push_back(className + "." + method.getName() + " = " + method.getFunctionName(className) + ";");
        }
        codeBlock.push_back("return newObject;");

        return Method(className,className + "Default",CodeBlock(codeBlock));
    }

    Method createPointerConstructor() {
        vector<Parameter> memberNeedInit;
        for (size_t i = 0; i < members.size(); i++) {
            if (members.at(i).getStoredValue() != "") {
                memberNeedInit.push_back(members.at(i));
                members.at(i).setStoredValue("");
            }
        }
        vector<string> codeBlock;
        codeBlock.push_back(className + " newObject;");
        codeBlock.push_back("newObject = malloc(sizeof(" + className + "));");
        for (auto member : memberNeedInit) {
            codeBlock.push_back(className + "->" + member.printWOType() + ";");
        }

        for (auto method : methods) {
            codeBlock.push_back(className + "->" + method.getName() + " = " + method.getFunctionName(className) + ";");
        }
        codeBlock.push_back("return newObject;");

        return Method(className + "*",className + "Pointer",CodeBlock(codeBlock));
    }

    Method createDeconstructor() {
        vector<Parameter> param;

        param.push_back(Parameter(className,"*","object"));
        vector<string> codeBlock;
        codeBlock.push_back("free(object);");
        codeBlock.push_back("return NULL;");

        return Method("NULL",className + "Deconstructor",param,CodeBlock(codeBlock));
    }

public:
    Class();
    /*Class(const string &className, const vector<Parameter> &members, vector<Method> methods)
    : className(className), members(members), methods(methods) {};*/
    Class(const string &className, vector<Parameter> members, vector<Method> methods) {
        this->className = className;
        this->members = members;
        this->methods = methods;
        this->defaultConstructor = createDefaultConstructor();
        this->pointerConstructor = createPointerConstructor();
        this->deconstructor = createDeconstructor();
    }

    const string &getClassName() const {
        return className;
    }

    string makeSource() {
        stringstream out;
        //out << "#include \"" << className << ".h\"" << endl << endl;

        out << defaultConstructor.functionFormPlain("") << endl << endl;
        out << pointerConstructor.functionFormPlain("") << endl << endl;
        out << deconstructor.functionFormPlain(className) << endl << endl;

        for (auto method : methods) {
            out << method.functionForm(className) << endl << endl;
        }

        return out.str();
    }

    string makeHeader() {
        stringstream out;

        out << makeStruct() << endl << endl;

        out << defaultConstructor.definitionFormPlain(className) << endl << endl;
        out << pointerConstructor.definitionFormPlain(className) << endl << endl;
        out << deconstructor.definitionFormPlain(className) << endl << endl;

        for (auto method : methods) {
            out << method.definitionForm(className) << endl;
        }

        return out.str();
    }

    string toString(){
        stringstream out;

        out << "class " << className << " {" << endl;
        for (auto member: members) {
            out << member << ";" << endl;
        }
        out << endl;
        out << defaultConstructor.functionForm(className) << endl;
        out << pointerConstructor.functionForm(className) << endl;
        out << deconstructor.functionForm(className) << endl;

        for (auto method : methods) {
            out << method.functionForm(className) << endl;
        }
        out << "};" << endl;

        return out.str();
    }

    friend std::ostream& operator<<(std::ostream& os, Class& clasS)
    {
        os << clasS.toString();
        return os;
    }
};


#endif //CWITHCLASSES_CLASS_H
