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

public:

    std::string getType() {
        return type;
    }
    std::string getName() {
        return name;
    }

    std::string toString() const{
        std::stringstream out;
        out << type << " " << name;

        out.str();
    }

    friend std::ostream& operator<<(std::ostream& os, const Parameter& pram)
    {
        os << param.toString();
        return os;
    }
};


#endif //CWITHCLASSES_PARAMETER_H
