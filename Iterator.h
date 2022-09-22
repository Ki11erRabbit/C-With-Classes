#include <stdlib.h>

#include "TreeNode.h"
#include "ListNode.h"


#define foreach(item,structure) \
        for (Iterator iterator = structure->begin(); !iterator->equals(structure->end()); iterator->next()) \
            for (char loopCheck = 1; loopCheck;)                    \
            for (item = iterator.get(); loopCheck;loopCheck = !loopCheck)

class Iterator {
    void *pointer;
    size_t elementSize;
    size_t position = 0;

    Iterator Iterator(void *pointer,size_t elementSize, size_t pos) {
        Iterator newIterator;
        newIterator.pointer = pointer;
        newIterator.elementSize = elementSize;
        newIterator.position = pos;

        return newIterator;
    }
    Iterator *newIterator(void *pointer,size_t elementSize, size_t pos) {
        Iterator *newIterator;
        newIterator->pointer = pointer;
        newIterator->elementSize = elementSize;
        newIterator->position = pos;

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
        this->position++;
    }
    void previous() {
        this->pointer = ((char*)this->pointer) - this->elementSize;
        this->position--;
    }
    void nextListNode() {
        this->pointer = ((ListNode*)this->pointer)->nextNode;
        this->position++;
    }
    void previousNode() {
        this->pointer = ((ListNode*)this->pointer)->previousNode;
        this->position--;
    }

    int equals(Iterator *other) {
        if (this->pointer == other->pointer) {
            return 1;
        }
        return 0;
    }
};