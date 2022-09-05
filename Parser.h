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
    string className;
    vector<string> classMembers;
    vector<MethodStructure> methods;
    MethodStructure constructor;
    MethodStructure deconstructor;
    vector<string> macros;
    vector<string> includes;

public:

    Parser(headerFile header) {
        headerName = header.first;
        tokens = header.second;
    }

    void setIncludes() {
        for (size_t i = 0; i < tokens.size(); i++) {
            auto token = tokens.at(i);
            if (token.find("include") < token.size()) {
                includes.push_back(token);
                tokens.erase(tokens.begin()+i);
                i--;
            }
        }
    }
    void setMacros() {
        for (size_t i = 0; i < tokens.size(); i++) {
            auto token = tokens.at(i);
            if (token.find("define") < token.size() || token[token.size() -1] == '\\') {
                includes.push_back(token);
                tokens.erase(tokens.begin()+i);
                i--;
            }
        }
    }

    bool isCType(string line) {

        if (line.find("char") == 0 || line.find("int") == 0 || line.find("long") == 0 ||
            line.find("short") == 0 || line.find("float") == 0 || line.find("double") == 0 ||
            line.find("void") == 0 || line.find("struct") == 0) {
            return true;
        }
        return false;
    }

    void setClassMembers() {
        for (size_t i = 0; i < tokens.size(); i++) {
            auto token = tokens.at(i);
            if (isCType(token) && token.find('(') >= token.size()) {
                classMembers.push_back(token);
                tokens.erase(tokens.begin()+i);
                i--;
            }
        }
    }

    string getMethodType(string line) {
        vector<string> tokens;
        string type;
        {
            string token;
            stringstream ss(line);
            while (getline(ss, token, ' ')) {
                tokens.push_back(token);
            }
        }
        for (size_t i = 0; i < tokens.size(); i++) {
            auto token = tokens.at(i);
            if (isCType(token)) {
                type += token + " ";
                if (token == "struct") {
                    type += tokens.at(i+1);
                    i++;
                }
            }
        }


    }

    void setClassMethods() {
        for (size_t i = 0; i < tokens.size(); i++) {
            auto token = tokens.at(i);

        }
    }

    ClassStructure buildClass () {
        vector<string> includes;
        vector<string> preProcs;
        string className = "NULL";
        bool classMode = false;
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
            else if (tokens.at(i) == "class" && !classMode) {
                classMode = true;
            }
            else if (classMode) {
                if (className == "NULL") {
                    className = tokens.at(i);
                    i++;
                }





            }

        }
    }

};


#endif //CWITHCLASSES_PARSER_H
