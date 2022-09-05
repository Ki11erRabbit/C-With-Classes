//
// Created by ki11errabbit on 9/4/22.
//

#ifndef CWITHCLASSES_CODEBLOCK_H
#define CWITHCLASSES_CODEBLOCK_H

#include <string>
#include <iostream>

class CodeBlock {

public:
    std::string toString() const{

    }

    friend std::ostream& operator<<(std::ostream& os, const CodeBlock& body)
    {
        os << body.toString();
        return os;
    }
};


#endif //CWITHCLASSES_CODEBLOCK_H
