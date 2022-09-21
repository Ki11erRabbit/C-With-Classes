#include <stdlib.h>
#include <string.h>

class Node {
    void *data;
    size_t elementSize;

    Node Node(void *data, size_t elemSize) {
        Node node;
        newList.elementSize = elemSize;
        node.data = malloc(elemSize);

        memcopy(node.data,data,elemSize)
    }
};