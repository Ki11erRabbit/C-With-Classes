//
// Created by ki11errabbit on 9/4/22.
//

#ifndef CWITHCLASSES_CODEBLOCK_H
#define CWITHCLASSES_CODEBLOCK_H

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "Parameter.h"
#include "Enum.h"

class CodeBlock {
private:
    std::string statement;//for like if, else, for, while
    std::vector<CodeBlock> codeBlocks;
    std::vector<std::string> lines;// code blocks get injected in order of discovery
    std::vector<Parameter> variables;
    std::vector<Enum> enums;
    size_t currentBlock = 0;
    size_t currentVar = 0;

public:

    CodeBlock() {

    }
    CodeBlock(std::string statement, std::vector<Parameter> variables, std::vector<Enum> enums, std::vector<std::string> lines, std::vector<CodeBlock> codeBlocks)
    : statement(statement),variables(variables),enums(enums), lines(lines),codeBlocks(codeBlocks) {}

    CodeBlock(std::string statement, std::vector<Parameter> variables,std::vector<std::string> lines, std::vector<CodeBlock> codeBlocks)
            : statement(statement),variables(variables), lines(lines),codeBlocks(codeBlocks) {}

            CodeBlock(std::vector<std::string> lines)
            : lines(lines) {}
    CodeBlock(std::vector<Parameter> vars,std::vector<std::string> lines)
            : variables(vars), lines(lines) {}
    void incrementCurrentBlock() {
        currentBlock += 1;
    }
    void incrementCurrentVar() {
        currentVar += 1;
    }

    const vector<CodeBlock> &getCodeBlocks() const {
        return codeBlocks;
    }

    void setCodeBlocks(const vector<CodeBlock> &codeBlocks) {
        CodeBlock::codeBlocks = codeBlocks;
    }

    const vector<std::string> &getLines() const {
        return lines;
    }

    void setLines(const vector<std::string> &lines) {
        CodeBlock::lines = lines;
    }

    const vector<Parameter> &getVariables() const {
        return variables;
    }

    void setVariables(const vector<Parameter> &variables) {
        CodeBlock::variables = variables;
    }

    void insertLine(string line, size_t index) {
        lines.insert(lines.begin()+index, line);
    }

    std::string toString() {
        std::stringstream out;
        if (statement != "")
            out << statement << " ";
        out << "{" << std::endl;
        /*for (auto var : variables) {
            out << var << ";" << std::endl;
        }*/
        for (auto enuM : enums) {
            out << enuM << endl;
        }
        for (auto line : lines) {
            if ( line == "{}") {
                out << codeBlocks.at(currentBlock);
                incrementCurrentBlock();
            }
            else if (line == "}{") {
                out << variables.at(currentVar) << ";" << endl;
                incrementCurrentVar();
            }
            else {
                out << line << std::endl;
            }
        }
        out << "}" << std::endl;
        return out.str();
    }

    friend std::ostream& operator<<(std::ostream& os, CodeBlock& body)
    {
        os << body.toString();
        return os;
    }
};


#endif //CWITHCLASSES_CODEBLOCK_H
