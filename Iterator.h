#include <stdlib.h>



class Iterator {
    void *pointer;
    size_t elementSize;

    Iterator Iterator(void *pointer,size_t elementSize) {
        Iterator newIterator;
        newIterator.pointer = pointer;
        newIterator.elementSize = elementSize;

        return newIterator;
    }
    Iterator *newIterator(void *pointer,size_t elementSize) {
        Iterator *newIterator;
        newIterator->pointer = pointer;
        newIterator->elementSize = elementSize;

        return newIterator;
    }

    void *get() {
        return this->pointer;
    }
    size_t getItemSize() {
        return elementSize;
    }

    void next() {
        this->pointer = ((char*)this->pointer) + this->elementSize;
    }
    void previous() {
        this->pointer = ((char*)this->pointer) - this->elementSize;
    }
    void nextNodeList() {
        this->pointer = ((char*)this->pointer) + this->elementSize;
    }
    void previousNode() {
        this->pointer = ((char*)this->pointer) - this->elementSize;
    }
    int equals(Iterator *other) {
        if (this->pointer == other->pointer) {
            return 1;
        }
        return 0;
    }
};