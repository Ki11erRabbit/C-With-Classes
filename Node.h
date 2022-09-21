#include <stdlib.h>
#include <string.h>

class Node {
    void *data;
    size_t elementSize;

    Node Node(void *data, size_t elemSize) {
        Node node;
        node.elementSize = elemSize;
        node.data = malloc(elemSize);

        memcpy(node.data,data,elemSize);

        return node;
    }
    Node *newNode(void *data, size_t elemSize) {
        Node *node;
        node->elementSize = elemSize;
        node->data = malloc(elemSize);

        memcpy(node->data,data,elemSize);

        return node;
    }

    void _Node(Node *node) {
        free(node->data);
    }
    void _newNode(Node *node) {
        free(node->data);
        free(node);
    }
};