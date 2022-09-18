#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <sys/stat.h>
#include <cstring>

#include "Scanner.h"
#include "Parser.h"


using namespace std;

int main(int argc, char *argv[]) {

    std::cout << argc << " " << argv[1] << std::endl;
    vector<CwithClasses> files;
    for (int i = 1; i < argc; i++) {
        ifstream in(argv[i]);
        string accumulator;
        char currentChar;
        while (in.peek() != EOF) {
            in.get(currentChar);
            accumulator += currentChar;
        }
        cout << "Making Tokens" << endl;
        Scanner scanner(accumulator);
        vector<Token> tokens;
        Token currToken = scanner.scanToken();
        tokens.push_back(currToken);

        while (scanner.getSize() != 0) {
            currToken = scanner.scanToken();
            tokens.push_back(currToken);
            //cout << currToken << endl;
        }
        cout << tokens.size() << " tokens" << endl;

        /*for (auto token : tokens) {
            //cout << token << endl;
        }*/
        stringstream headerName = stringstream(argv[i]);
        string name;
        getline(headerName,name,'.');
        cout << "Parsing Tokens" << endl;
        Parser parser(tokens);
        CwithClasses file = parser.startParsing();
        file.setName(name);
        files.push_back(file);
        in.close();
    }

    for(size_t i = files.size()-1; i < files.size(); i--) {
        files.at(i).setupInheritance(files);
    }

    for (auto& file : files) {
        for (auto& claSS : file.getClasses()) {
            claSS.initializeConstructDeconstruct();
        }
    }

    if (mkdir("output", 0777) == -1)
        cerr << "Error : " << strerror(errno) << endl;
    else
        cout << "Directory created";

    for (auto file :files) {
        ofstream header("output/" + file.getName() + ".h");
        ofstream source("output/" + file.getName() + ".c");

        header << file.makeHeaderFile();
        source << file.makeSourceFile();
        header.close();
        source.close();
    }
    return 0;
}
