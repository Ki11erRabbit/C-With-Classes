#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
int main(int argc, char *argv[]) {

    std::cout << argc << " " << argv[1] << std::endl;

    ifstream in(argv[1]);
    string line;
    vector<std::string> accumulator;
    while (getline(in,line)) {

        if (line[0] == '#') {
            if (line[line.size() - 1] == '\\') {
                accumulator.push_back(line);
                while (line[line.size() - 1] == '\\') {
                    getline(in,line);
                    accumulator.push_back(line);
                }

            }
            else {
                accumulator.push_back(line);
            }
        }
    }







    for (auto body : accumulator) {
        std::cout << body << std::endl;
    }






    return 0;
}
