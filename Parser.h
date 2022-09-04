//
// Created by ki11errabbit on 9/4/22.
//

#ifndef CWITHCLASSES_PARSER_H
#define CWITHCLASSES_PARSER_H

#include <string>
#include <vector>
#include <sstream>

#include "ClassStructure.h"

#define headerFile pair<string,vector<string>>
using namespace std;

class Parser {
private:
    string headerName;
    vector<string> tokens;

public:

    Parser(headerFile header) {
        headerName = header.first;
        tokens = header.second;
    }

    ClassStructure buildClass () {

    }

};


#endif //CWITHCLASSES_PARSER_H
