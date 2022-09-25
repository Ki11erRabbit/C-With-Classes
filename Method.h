//
// Created by ki11errabbit on 9/4/22.
//

#ifndef CWITHCLASSES_METHOD_H
#define CWITHCLASSES_METHOD_H

#include <string>
#include <vector>
#include <iostream>

#include "Token.h"
#include "Parameter.h"
#include "CodeBlock.h"

using namespace std;
class Method {
private:
    string returnType, methodName;
    vector<Parameter> parameters;
    CodeBlock body;

public:
    Method() {
        returnType = "";
        methodName = "";
        vector<string> temp = {""};
        body = CodeBlock(temp);
    }
    Method(string returnType, string methodName, vector<Parameter> parameters,CodeBlock body)
    : returnType(returnType),methodName(methodName),parameters(parameters), body(body) {}

    Method(string returnType, string methodName,CodeBlock body)
            : returnType(returnType),methodName(methodName), body(body) {}

    string getFunctionName(string className) {
        return className + "__" + methodName;
    }
    string getName() {
        return methodName;
    }
    CodeBlock getBody() {
        return body;
    }
    void setBody(CodeBlock body) {
        this->body = body;
    }

    const string &getMethodName() const {
        return methodName;
    }

    void setMethodName(const string &methodName) {
        Method::methodName = methodName;
    }
    void insertParameter(Parameter param) {
        parameters.push_back(param);
    }

    void insertCodeLine(string line, size_t index) {
        body.insertLine(line,index);
    }

    string callForm(string className) {
        stringstream out;

        out << className << "__" << methodName << "(";
        for (size_t i = 0; i < parameters.size(); i++) {
            out << parameters.at(i).getName();
            if (i < parameters.size()-1)
                out << ", ";
        }
        out << ")";

        return out.str();
    }

    string pointerForm(string className){
        stringstream out;

        if (returnType == className) {
            out << "struct ";
        }
        out << returnType << " (*" << methodName << ")(";
        out << "void" << "*";
        if (!parameters.empty())
            out << ", ";
        for (size_t i = 0; i < parameters.size(); i++) {
            string param;
            if (parameters.at(i).getType() == className) {
                param = "struct ";
            }
            param += parameters.at(i).getType() + parameters.at(i).getPointer();
            out << param;
            if (i < parameters.size()-1)
                out << ", ";
        }
        out << ");";

        return out.str();
    }
    string pointerForm(string className, string overloadName){
        stringstream out;

        if (returnType == className) {
            out << "struct ";
        }
        out << returnType << " (*" << overloadName << ")(";
        out << "void" << "*";
        if (!parameters.empty())
            out << ", ";
        for (size_t i = 0; i < parameters.size(); i++) {
            string param;
            if (parameters.at(i).getType() == className) {
                param = "struct ";
            }
            param += parameters.at(i).getType() + parameters.at(i).getPointer();
            out << param;
            if (i < parameters.size()-1)
                out << ", ";
        }
        out << ");";

        return out.str();
    }
    string functionFormPlain(string className) {
        stringstream out;

        out << returnType << " " << className << "__" << methodName << "(";
        for (size_t i = 0; i < parameters.size(); i++) {
            out << parameters.at(i).getType() << " " << parameters.at(i).getPointer()  << parameters.at(i).getName();
            if (i < parameters.size()-1)
                out << ", ";
        }
        out << ") " << body;

        return out.str();
    }

    string functionForm(string className) {
        stringstream out;

        out << returnType << " " << className << "__" << methodName << "(";
        out << className << "* " << "this";
        if (!parameters.empty())
            out << ", ";
        for (size_t i = 0; i < parameters.size(); i++) {
            out << parameters.at(i).getType() << " " << parameters.at(i).getPointer()  << parameters.at(i).getName();
            if (i < parameters.size()-1)
                out << ", ";
        }
        out << ") " << body;

        return out.str();
    }

    string definitionForm(string className) {
        stringstream out;

        out << returnType << " " << className << "__" << methodName << "(";
        out << className << "* " << "this";
        if (!parameters.empty())
            out << ", ";
        for (size_t i = 0; i < parameters.size(); i++) {
            out << parameters.at(i).getType() << " " << parameters.at(i).getPointer()  << parameters.at(i).getName();
            if (i < parameters.size()-1)
                out << ", ";
        }
        out << ");";

        return out.str();
    }
    string definitionFormPlain(string className) {
        stringstream out;

        out << returnType << " " << className << "__" << methodName << "(";

        for (size_t i = 0; i < parameters.size(); i++) {
            out << parameters.at(i).getType() << " " << parameters.at(i).getPointer() << parameters.at(i).getName();
            if (i < parameters.size()-1)
                out << ", ";
        }
        out << ");";

        return out.str();
    }

    Method(Method const &method) {
        this->parameters = method.parameters;
        this->methodName = method.methodName;
        this->returnType = method.returnType;
        this->body = method.body;
    }

    bool operator==(const Method& other) const
    {
        return this->methodName == other.methodName;
    }
};


#endif //CWITHCLASSES_METHOD_H
