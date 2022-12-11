//
// Created by ki11errabbit on 9/4/22.
//

#ifndef CWITHCLASSES_PARAMETER_H
#define CWITHCLASSES_PARAMETER_H

#include <string>
#include <iostream>

#include "Token.h"

class Parameter {
private:
    std::string type;
    std::string name;
    std::string pointer;
    std::string storedValue;
    std::string functionPointerParam;
    bool functionPointer = false;

public:

    Parameter();
    Parameter (std::string type, std::string name)
    : type(type), name(name) {};
    Parameter (std::string type, std::string pointer, std::string name)
            : type(type), pointer(pointer), name(name) {};
    Parameter (std::string type, std::string pointer, std::string name, std::string storedValue)
            : type(type), pointer(pointer), name(name), storedValue(storedValue) {};
    Parameter (std::string type, std::string pointer, std::string name, std::string storedValue, bool funcPointer)
            : type(type), pointer(pointer), name(name), storedValue(storedValue), functionPointer(funcPointer) {};

    Parameter (std::string type, std::string pointer, std::string name, std::string storedValue, bool funcPointer, std::string functionPointerParam)
            : type(type), pointer(pointer), name(name), storedValue(storedValue), functionPointer(funcPointer), functionPointerParam(functionPointerParam) {};


    std::string getType() {
        return type;
    }
    std::string getName() {
        return name;
    }
    std::string getStoredValue() {
        return storedValue;
    }
    std::string getPointer() {
        return pointer;
    }
    void setStoredValue(std::string value) {
        storedValue = value;
    }

    std::string printWOType() const{
        std::stringstream out;
        if (pointer != "" )
            out << pointer;
        if (functionPointer) {
            out << "(*" << name << ")" << functionPointerParam;
        }
        else {
            out << name;
        }
        if (storedValue != "") {
            out << " = " << storedValue;
        }

        return out.str();
    }
    std::string printWOTypePointer() const{
        std::stringstream out;
        /*if (functionPointer) {
            out << "(*" << name << ")" << functionPointerParam;
        }
        else {
            out << name;
        }*/
        out << name;
        if (storedValue != "") {
            out << " = " << storedValue;
        }

        return out.str();
    }
    std::string printWType() const{
        std::stringstream out;
        out << type << " ";
        if (pointer != "" && !functionPointer)
            out << pointer;
        if (functionPointer) {
            out << "(*" << name << ")" << functionPointerParam;
        }
        else {
            out << name;
        }

        return out.str();
    }

    std::string toString() const{
        std::stringstream out;
        out << type << " ";
        if (pointer != "" )
            out << pointer;
        if (functionPointer) {
            out << "(*" << name << ")" << functionPointerParam;
        }
        else {
            out << name;
        }
        if (storedValue != "") {
            out << " = " << storedValue;
        }

        return out.str();
    }

    friend std::ostream& operator<<(std::ostream& os, const Parameter& param)
    {
        os << param.toString();
        return os;
    }
    bool operator==(const Parameter& other) const
    {
        return (this->name == other.name) && (this->type == other.type);
    }
};


#endif //CWITHCLASSES_PARAMETER_H
