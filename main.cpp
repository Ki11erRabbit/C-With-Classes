#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "Scanner.h"
#define headerFile pair<string,vector<string>>

using namespace std;

bool isCType(string line) {

    if (line.find("char") < line.size() || line.find("int") < line.size() || line.find("long") < line.size() ||
    line.find("short") < line.size() || line.find("float") < line.size() || line.find("double") < line.size() ||
    line.find("void") < line.size() || line.find("struct") < line.size()) {
        return true;
    }
    return false;
}

vector<string> scanFile(ifstream *in) {
    string line;
    vector<std::string> accumulator;
    bool function = false;
    while (getline(*in,line)) {

        if (line[0] == '#') {
            if (line[line.size() - 1] == '\\') {
                accumulator.push_back(line);
                while (line[line.size() - 1] == '\\') {
                    getline(*in,line);
                    accumulator.push_back(line);
                }
            }
            else {
                accumulator.push_back(line);
            }
        }
        else {
            if (function) {
                accumulator.push_back(line);
            }
            if (function && line[line.size() -1 ] == '}') {
                function = false;
            }
            else if (line.find('(') < line.size()) {
                accumulator.push_back(line);
                function = true;
            }
            else if (isCType(line)) {
                accumulator.push_back(line);
            }
            else if (!function){
                string token;
                stringstream ss(line);
                while (getline(ss,token, ' ')) {
                    if (token == "")
                        continue;

                    accumulator.push_back(token);
                }
            }

            //accumulator.push_back(line);
        }
    }
    return accumulator;
}


int main(int argc, char *argv[]) {

    std::cout << argc << " " << argv[1] << std::endl;


    ifstream in(argv[1]);
    string accumulator;
    char currentChar;
    while (in.peek() != EOF) {
        in.get(currentChar);
        accumulator += currentChar;
    }

    Scanner scanner(accumulator);
    vector<Token> tokens;
    Token currToken = scanner.scanToken();
    tokens.push_back(currToken);

    while (scanner.getSize() != 0) {
        currToken = scanner.scanToken();
        tokens.push_back(currToken);
        //cout << currToken << endl;
    }

    for (auto token : tokens) {
        cout << token << endl;
    }




    return 0;
}
