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

public:

    Parameter (std::string type, std::string name)
    : type(type), name(name) {};
    Parameter (std::string type, std::string pointer, std::string name)
            : type(type), pointer(pointer), name(name) {};
    Parameter (std::string type, std::string pointer, std::string name, std::string storedValue)
            : type(type), pointer(pointer), name(name), storedValue(storedValue) {};

    std::string getType() {
        return type;
    }
    std::string getName() {
        return name;
    }

    std::string toString() const{
        std::stringstream out;
        out << type << " ";
        if (pointer != "" )
            out << pointer;
        out << name;
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
};


#endif //CWITHCLASSES_PARAMETER_H
