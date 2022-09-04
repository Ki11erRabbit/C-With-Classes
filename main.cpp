#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#define headerFile pair<string,vector<string>>

using namespace std;


vector<string> scanFile(ifstream *in) {
    string line;
    vector<std::string> accumulator;
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
            string token;
            stringstream ss(line);

            while (getline(ss,token, ' ')) {
                if (token == "")
                    continue;

                accumulator.push_back(token);
            }
        }
    }
    return accumulator;
}


int main(int argc, char *argv[]) {

    std::cout << argc << " " << argv[1] << std::endl;

    vector<headerFile> headers;

    for (int i = 1; i < argc; i++) {
        ifstream in(argv[1]);
        headers.push_back({argv[i],scanFile(&in)});
    }

    for (auto header : headers) {
        std::cout << header.first << std::endl;
        for (auto body : header.second) {
            cout << "\t" << body << endl;
        }
    }






    return 0;
}
