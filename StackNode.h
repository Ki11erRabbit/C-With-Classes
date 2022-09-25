#include "Node.h"

class StackNode: Node {
    StackNode *nextNode = NULL;

    StackNode StackNode(void *data, size_t elemSize, StackNode *next) {
        StackNode node;
        node.nextNode = next;
        return node;
    }
    StackNode *newStackNode(void *data, size_t elemSize, StackNode *next) {
        StackNode *node;
        node->nextNode = next;
        return node;
    }


    StackNode *getNext() {
        return this->nextNode;
    }

    void setNext(StackNode *node) {
        this->nextNode = node;
    }
};