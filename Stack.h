#include <stdio.h>
#include "StackNode.h"


class Stack {
    StackNode *stackTop;
    size_t numNodes = 0;
    size_t elementSize = 1;

    int empty() {
        if (this->stackTop == NULL) {
            return 1;
        }
        return 0;
    }
    size_t size() {
        return this->numNodes;
    }
    void *top() {
        return this->stackTop->getData();
    }
    void push(void* val) {
        StackNode *newNode = StackNode__newStackNode(val, this->elementSize,this->stackTop);
        this->stackTop = newNode;
        numNodes++;
    }
    void pop() {
        StackNode newTop = this->stackTop->nextNode;
        StackNode___newStackNode(this->stackTop);
        this->stackTop = newTop;
        numNodes--;
    }
    void swap(Stack *other) {
        if (this->elementSize != other->elementSize) {
            fprintf(stderr, "%s", "Error: different type in stack");
            return;
        }
        size_t tempNodeCount = this->numNodes;
        StackNode *tempTop = this->stackTop;

        this->stackTop = other->stackTop;
        this->numNodes = other->numNodes;

        other->stackTop = tempTop;
        other->numNodes = tempNodeCount;

    }
};