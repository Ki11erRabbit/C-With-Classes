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
#include <algorithm>
//#include "CwithClasses.h"
//#include "Method.h"

using namespace std;

class Class {
private:
    string className;
    vector<Parameter> members;
    vector<Method> methods;
    vector<pair<string,vector<Method>>> overloadedMethods;
    Method defaultConstructor;
    Method pointerConstructor;
    vector<Method> altConstructors;
    vector<Method> altPointerConstructors;
    Method deconstructor;
    Method pointerDeconstructor;
    string parentClass;
    vector<pair<string,Method>> inheritedMethods;
    string header;
    vector<string> includes;
    vector<Method> privateMethods;

    string makeStruct() {
        stringstream out;
        out << "typedef struct " << className << " {" << endl;
        for (auto member : members) {
            if (member.getName() == className) {
                out << "\t" << "struct " << member << ";" << endl;
            }
            else {
                out << "\t" << member.printWType() << ";" << endl;
            }
        }
        for (auto method : methods) {
            out << method.pointerForm(className) << endl;
        }
        for (auto overLoad : overloadedMethods) {
            out << overLoad.second.at(0).pointerForm(className,overLoad.first) << endl;
        }
        for (auto method : inheritedMethods) {
            out << method.second.pointerForm(method.first) << endl;
        }
        out << "} " << className << ";" << endl;

        return out.str();
    }

    Method createDefaultConstructor() {
        vector<Parameter> memberNeedInit;
        for (size_t i = 0; i < members.size(); i++) {
            if (members.at(i).getStoredValue() != "") {
                memberNeedInit.push_back(members.at(i));
            }
            if (members.at(i).getPointer() != "") {
                members.at(i).setStoredValue("NULL");
                memberNeedInit.push_back(members.at(i));
            }
        }
        vector<string> codeBlock;
        string temp;
        codeBlock.push_back("}{");
        Parameter tempObject(className,"newObject");
        for (auto member : memberNeedInit) {
            codeBlock.push_back(temp +"newObject" + "." + member.printWOTypePointer() + ";");
        }

        for (auto method : methods) {
            if (method.getName() == "new" + className || method.getName() == "_new" + className || method.getName() == "_" + className) {
                continue;
            }
            codeBlock.push_back(temp +"newObject" + "." + method.getName() + " = " + method.getFunctionName(className) + ";");
        }
        codeBlock.push_back("return newObject;");

        return Method(className,className,CodeBlock({tempObject},codeBlock));
    }

    Method createPointerConstructor() {
        vector<Parameter> memberNeedInit;
        for (size_t i = 0; i < members.size(); i++) {
            if (members.at(i).getStoredValue() != "") {
                memberNeedInit.push_back(members.at(i));
            }
            if (members.at(i).getPointer() != "") {
                members.at(i).setStoredValue("NULL");
                memberNeedInit.push_back(members.at(i));
            }
        }
        vector<string> codeBlock;
        string temp;
        codeBlock.push_back("}{");
        Parameter tempObject(className,"*","newObject");
        codeBlock.push_back(temp + "newObject = " + "(" + className + "*)malloc(sizeof(" + className + "));");
        for (auto member : memberNeedInit) {
            codeBlock.push_back(temp +"newObject" + "->" + member.printWOTypePointer() + ";");
        }

        for (auto method : methods) {
            if (method.getName() == "new" + className || method.getName() == "_new" + className || method.getName() == "_" + className) {
                continue;
            }
            codeBlock.push_back(temp + "newObject" + "->" + method.getName() + " = " + method.getFunctionName(className) + ";");
        }
        codeBlock.push_back("return newObject;");

        return Method(className + "*","new" + className,CodeBlock({tempObject},codeBlock));
    }

    Method createDeconstructor() {
        vector<Parameter> param;

        param.push_back(Parameter(className,"*","object"));
        vector<string> codeBlock;
        codeBlock.push_back("free(object);");

        return Method("void", "_" + className + "Deconstructor",param,CodeBlock(codeBlock));
    }
    Method createPointerDeconstructor() {
        vector<Parameter> param;

        param.push_back(Parameter(className,"*","object"));
        vector<string> codeBlock;
        codeBlock.push_back("free(object);");

        return Method("void","_new" + className + "Deconstructor",param,CodeBlock(codeBlock));
    }


    void findAndSetConstructors() {
        bool dConstruct = false, pConstruct = false;
        for (size_t i = 0; i < methods.size(); i++) {
            if (methods.at(i).getName() == className) {
                defaultConstructor = methods.at(i);
                methods.erase(methods.begin()+i);
                if (i > 0)
                    i--;
                injectVTable(defaultConstructor,0);
                dConstruct = true;
            }
            if (methods.at(i).getName().find(className) == 0) {
                altConstructors.push_back(methods.at(i));
                methods.erase(methods.begin()+i);
                //injectVTable(altConstructors.back(),0);
                if (i > 0)
                    i--;
            }
            if (methods.at(i).getName() == "new" + className) {
                pointerConstructor = methods.at(i);
                methods.erase(methods.begin()+i);
                if (i > 0)
                    i--;
                injectVTable(pointerConstructor,1);
                pConstruct = true;
            }
            if (methods.at(i).getName().find("new" + className) == 0) {
                altPointerConstructors.push_back(methods.at(i));
                methods.erase(methods.begin()+i);
                //injectVTable(altPointerConstructors.back(),1);
                if (i > 0)
                    i--;
            }
        }
        if (!dConstruct)
            defaultConstructor = createDefaultConstructor();

        if (!pConstruct)
            pointerConstructor = createPointerConstructor();
    }

    void injectVTable(Method& constructor, char key) {
        CodeBlock oldBody = constructor.getBody();
        vector<Parameter> parameters = oldBody.getVariables();
        vector<string> lines = oldBody.getLines();
        Parameter newObject("temp","temp");
        for (auto var : parameters) {
            if (var.getType() == className) {
                newObject = var;
            }
        }
        if (key == 0) {
            vector<Parameter> memberNeedInit;
            for (size_t i = 0; i < members.size(); i++) {
                if (members.at(i).getStoredValue() != "") {
                    memberNeedInit.push_back(members.at(i));
                }
                if (members.at(i).getPointer() != "") {
                    members.at(i).setStoredValue("NULL");
                    memberNeedInit.push_back(members.at(i));
                }
            }
            vector<string> codeBlock;
            string temp;
            codeBlock.push_back(className + " " + newObject.getName() +";");
            for (auto member : memberNeedInit) {
                if (lines.size() > 0)
                    lines.insert(lines.begin() + 1,newObject.getName() + "." + member.printWOTypePointer() + ";");
                else
                    lines.insert(lines.begin(),newObject.getName() + "." + member.printWOTypePointer() + ";");
            }

            for (auto method : methods) {
                if (method.getName() == "new" + className || method.getName() == "_new" + className || method.getName() == "_" + className) {
                    continue;
                }
                else {
                    lines.insert(lines.end() - 1, newObject.getName() + "." + method.getName() + " = " + method.getFunctionName(className) + ";");
                }
            }

            for (auto overload : overloadedMethods) {
                int i = 0;
                for (auto method : overload.second) {
                    lines.insert(lines.end() -1,"if (overloadCheck == " + to_string(i) + ")");
                    lines.insert(lines.end()-1, newObject.getName()+ "." + overload.first + " = " + method.getFunctionName(className) + ";");
                    i++;
                }
            }
            lines.insert(lines.end(), "return " + newObject.getName() + ";");
            constructor.setBody(CodeBlock("",parameters,lines,oldBody.getCodeBlocks()));
        }
        if (key == 1) {
            for (size_t i = 0; i < parameters.size(); i++) {
                if (parameters.at(i).getType() == className) {
                    parameters.at(i).setStoredValue("malloc(sizeof("+ className +"))");
                }
            }
            vector<Parameter> memberNeedInit;
            for (size_t i = 0; i < members.size(); i++) {
                if (members.at(i).getStoredValue() != "") {
                    memberNeedInit.push_back(members.at(i));
                }
            }
            vector<string> codeBlock;
            string temp;
            codeBlock.push_back(className + " " + newObject.getName() +";");
            for (auto member : memberNeedInit) {
                if (lines.size() > 0)
                    lines.insert(lines.begin() + 1,temp + newObject.getName() + "->" + member.printWOTypePointer() + ";");
                else
                    lines.insert(lines.begin(),temp + newObject.getName() + "->" + member.printWOTypePointer() + ";");
            }

            for (auto method : methods) {
                if (method.getName() == "new" + className || method.getName() == "_new" + className || method.getName() == "_" + className) {
                    continue;
                }
                else {
                    lines.insert(lines.end() - 1, temp + newObject.getName() + "->" + method.getName() + " = " +
                                                  method.getFunctionName(className) + ";");
                }
            }
            for (auto overload : overloadedMethods) {
                int i = 0;
                for (auto method : overload.second) {
                    lines.insert(lines.end() -1,"if (overloadCheck == " + to_string(i) + ")");
                    lines.insert(lines.end()-1, newObject.getName()+ "->" + overload.first + " = " + method.getFunctionName(className) + ";");
                    i++;
                }
            }

            constructor.setBody(CodeBlock("",parameters,lines,oldBody.getCodeBlocks()));
        }

    }

    void findAndSetDeconstructors() {
        bool dDeconstruct = false, pDeconstruct = false;
        for (size_t i = 0; i < methods.size(); i++) {
            if (methods.at(i).getName() == "_" + className) {
                deconstructor = methods.at(i);
                methods.erase(methods.begin()+i);
                if (i > 0)
                    i--;
                dDeconstruct = true;
            }
            if (methods.at(i).getName() == "_new" + className) {
                pointerDeconstructor = methods.at(i);
                methods.erase(methods.begin()+i);
                if (i > 0)
                    i--;
                pDeconstruct = true;
            }
        }
        if (!dDeconstruct)
            deconstructor = createDeconstructor();

        if (!pDeconstruct)
            pointerDeconstructor = createPointerDeconstructor();
    }

    void findOverloadedMethods() {

        for (size_t i = 0; i < methods.size(); i++) {
            string methodName = methods.at(i).getName();
            vector<Method> overMethods;
            overMethods.push_back(methods.at(i));
            for (size_t j = i+1; j < methods.size(); j++) {
                if (methodName == methods.at(j).getName()) {
                    overMethods.push_back(methods.at(j));
                    methods.erase(methods.begin()+j);
                    j--;
                }
            }
            if (overMethods.size() > 1) {
                methods.erase(methods.begin()+i);
                i--;
                for (size_t x = 0; x < overMethods.size(); x++) {
                    overMethods.at(x).setMethodName(overMethods.at(x).getName() + to_string(x));
                }
                overloadedMethods.push_back({methodName,overMethods});
            }
        }
    }
    void setupOverload() {
        Parameter overloadCheck("char","overloadCheck");
        defaultConstructor.insertParameter(overloadCheck);
        pointerConstructor.insertParameter(overloadCheck);
        for (auto& construct : altConstructors) {
            construct.insertParameter(overloadCheck);
        }
        for (auto& construct : altPointerConstructors) {
            construct.insertParameter(overloadCheck);
        }
    }

public:
    Class();
    /*Class(const string &className, const vector<Parameter> &members, vector<Method> methods)
    : className(className), members(members), methods(methods) {};*/
    Class(const string &className, vector<Parameter> members, vector<Method> methods,string parentClass) {
        this->className = className;
        this->members = members;
        this->methods = methods;
        this->parentClass = parentClass;
        findOverloadedMethods();
        initializeConstructDeconstruct();
        setupOverload();
    }
    Class(const string &className, vector<Parameter> members, vector<Method> methods,string parentClass,vector<Method> privateMethods) {
        this->className = className;
        this->members = members;
        this->methods = methods;
        this->parentClass = parentClass;
        this->privateMethods = privateMethods;
        findOverloadedMethods();
        initializeConstructDeconstruct();
        setupOverload();
    }

    void initializeConstructDeconstruct() {
        findAndSetConstructors();
        findAndSetDeconstructors();
        for (auto & altConstructor : altConstructors) {
            injectVTable(altConstructor,0);
        }
        for (auto & altPointerConstructor : altPointerConstructors) {
            injectVTable(altPointerConstructor,1);
        }
    }

    const string &getClassName() const {
        return className;
    }

    const vector<Parameter> &getMembers() const {
        return members;
    }

    const vector<Method> &getMethods() const {
        return methods;
    }

    const string &getParentClass() const {
        return parentClass;
    }

    string getHeader() {
        return header;
    }

    void injectBaseConstructor(Method construct, string parentName) {
        Parameter newObject("temp", "temp");
        CodeBlock body = defaultConstructor.getBody();
        for (auto var: body.getVariables()) {
            if (var.getType() == className) {
                newObject = var;
            }
        }
        {
            stringstream line;
            line << newObject.getName() << "." << "base = " << construct.callForm(parentName) << ";";
            defaultConstructor.insertCodeLine(line.str(), 1);
        }
        for (auto& constrct : altConstructors) {
            for (auto var : construct.getBody().getVariables()) {
                if (var.getType() == className) {
                    newObject = var;
                }
            }
            stringstream line;
            line << newObject.getName() << "." << "base = " << construct.callForm(parentName) << ";";
            constrct.insertCodeLine(line.str(),1);
        }
    }
    void injectBasePointerConstructor(Method construct, string parentName) {
        Parameter newObject("temp", "temp");
        CodeBlock body = pointerConstructor.getBody();
        for (auto var: body.getVariables()) {
            if (var.getType() == className) {
                newObject = var;
            }
        }
        {
            stringstream line;
            line << newObject.getName() << "->" << "base = " << construct.callForm(parentName) << ";";
            pointerConstructor.insertCodeLine(line.str(), 1);
        }
        for (auto& constrct : altPointerConstructors) {
            for (auto var : construct.getBody().getVariables()) {
                if (var.getType() == className) {
                    newObject = var;
                }
            }
            stringstream line;
            line << newObject.getName() << "->" << "base = " << construct.callForm(parentName) << ";";
            constrct.insertCodeLine(line.str(),1);
        }
    }

    void initializeInheritance(Class parent) {
        vector<Method> methodsToAdd;

        if (find(includes.begin(),includes.end(),parent.getHeader()) == includes.end()) {
            includes.push_back(parent.getHeader());
        }


        if (members.empty() || members.at(0).getType() != parent.className) {
            members.insert(members.begin(),Parameter(parent.getClassName(),"base"));

            injectBaseConstructor(parent.defaultConstructor, parent.className);
            injectBasePointerConstructor(parent.defaultConstructor,parent.className);

            for(const auto& method : parent.getMethods()) {
                if (find(this->methods.begin(),methods.end(), method) == methods.end()) {
                    methodsToAdd.push_back(method);
                }
            }
            for (auto method : methodsToAdd) {
                inheritedMethods.push_back({parent.getClassName(),method});
            }
        }

        for (auto method : parent.inheritedMethods) {
            inheritedMethods.push_back({method.first,method.second});
        }

    }

    void findInheritance(vector<Class> parents) {


        for (auto parent : parents) {
            if (parentClass == parent.getClassName()) {
                initializeInheritance(parent);
            }
        }


    }

    string makeSource() {
        stringstream out;
        //out << "#include \"" << className << ".h\"" << endl << endl;

        out << defaultConstructor.functionFormPlain(className) << endl << endl;
        for (auto altConstruct : altConstructors) {
            out << altConstruct.functionFormPlain(className) << endl << endl;
        }
        out << pointerConstructor.functionFormPlain(className) << endl << endl;
        for (auto altPointConstruct : altPointerConstructors) {
            out << altPointConstruct.functionFormPlain(className) << endl << endl;
        }

        out << deconstructor.functionFormPlain(className) << endl << endl;
        out << pointerDeconstructor.functionFormPlain(className) << endl << endl;

        for (auto method : privateMethods) {
            out << "static " << method.functionFormNamespaceless(className) << endl << endl;
        }

        for (auto method : methods) {
            out << method.functionForm(className) << endl << endl;
        }
        for (auto overload : overloadedMethods) {
            for (auto method : overload.second)
                out << method.functionForm(className) << endl;
        }

        return out.str();
    }

    string makeHeader() {
        stringstream out;
        for (auto include : includes) {
            out << "#include \"" << include << "\"" << endl;
        }
        out << endl;
        out << makeStruct() << endl << endl;

        out << defaultConstructor.definitionFormPlain(className) << endl << endl;
        for (auto altConstruct : altConstructors) {
            out << altConstruct.definitionFormPlain(className) << endl << endl;
        }
        out << pointerConstructor.definitionFormPlain(className) << endl << endl;
        for (auto altPointConstruct : altPointerConstructors) {
            out << altPointConstruct.definitionFormPlain(className) << endl << endl;
        }
        out << deconstructor.definitionFormPlain(className) << endl << endl;
        out << pointerDeconstructor.definitionFormPlain(className) << endl << endl;

        for (auto method : methods) {
            out << method.definitionForm(className) << endl;
        }
        for (auto overload : overloadedMethods) {
            for (auto method : overload.second)
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

    void setHeader(char *string) {
        header = string;
    }
};


#endif //CWITHCLASSES_CLASS_H
