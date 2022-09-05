//
// Created by ki11errabbit on 9/4/22.
//

#ifndef CWITHCLASSES_CODEBLOCK_H
#define CWITHCLASSES_CODEBLOCK_H

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

class CodeBlock {
private:
    std::string line;
    std::vector<std::string> lines;

public:
    CodeBlock(std::vector<std::string> lines)
    : lines(lines) {}

    std::string toString() const{
        std::stringstream out;

        out << "{" << std::endl;
        for (auto line : lines) {
            out << line << std::endl;
        }
        out << "}" << std::endl;
    }

    friend std::ostream& operator<<(std::ostream& os, const CodeBlock& body)
    {
        os << body.toString();
        return os;
    }
};


#endif //CWITHCLASSES_CODEBLOCK_H
