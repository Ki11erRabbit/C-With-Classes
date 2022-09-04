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
        vector<string> includes;
        vector<string> preProcs;
        string className;

        for (size_t i = 0; i < tokens.size(); i++) {

            if (tokens.at(i)[0] == '#') {
                if (tokens.at(i).find("include") < tokens.at(i).size() || tokens.at(i).find("define") < tokens.at(i).size()) {
                    if (tokens.at(i).find("include") < tokens.at(i).size()) {
                        includes.push_back(tokens.at(i));
                    }
                    else {
                        if (tokens.at(i)[tokens.at(i).size() - 1] == '\\') {
                            size_t j;
                            for (j = i; j < tokens.size() && tokens.at(j)[tokens.at(i).size() - 1] == '\\'; j++) {
                                preProcs.push_back(tokens.at(j));
                            }
                            i = j -1;

                        }
                    }
                }

            }
            else if (tokens.at(i) == "class") {
                className = tokens.at(++i);
                if (tokens.at(i++) != "{"){
                    throw "ERROR no class name";
                }
                string varType;
                string varName;
                vector<pair<string,string>> classData;
                string methodType;
                string methodName;
                for (; i < tokens.size(); i++) {
                    if (tokens.at(i).find("(") >= tokens.at(i).size()) {
                        if (tokens.at(i).find("unsigned") < tokens.at(i).size() || tokens.at(i).find("signed") < tokens.at(i).size()) {
                            varType = tokens.at(i) + " " + tokens.at(++i);
                            varName = tokens.at(++i);
                            classData.push_back({varType,varName});
                        }
                        else if (tokens.at(i).find("long") < tokens.at(i).size() && tokens.at(i + 1) == "double") {
                            varType = tokens.at(i) + " " + tokens.at(++i);
                            varName = tokens.at(++i);
                            classData.push_back({varType,varName});
                        }
                        else {
                            varType = tokens.at(i);
                            varName = tokens.at(++i);

                            classData.push_back({varType,varName});
                        }

                    }
                    else {

                    }

                }
            }

        }
    }

};


#endif //CWITHCLASSES_PARSER_H
