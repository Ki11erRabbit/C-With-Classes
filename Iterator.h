#include <stdlib.h>

#include "TreeNode.h"
#include "ListNode.h"


#define foreach(item,dataStructure) \
        for (Iterator iterator = dataStructure->begin(dataStructure); !iterator.equals(&iterator,dataStructure->end(dataStructure)); iterator.next(&iterator)) \
            for (char loopCheck = 1; loopCheck;)                    \
            for (item = iterator.get(&iterator); loopCheck;loopCheck = !loopCheck)

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
        return this->elementSize;
    }

    void next() {
        this->pointer = ((char*)this->pointer) + this->elementSize;
        this->position++;
    }
    void previous() {
        this->pointer = ((char*)this->pointer) - this->elementSize;
        this->position--;
    }
    void next() {
        this->pointer = ((ListNode*)this->pointer)->nextNode;
        this->position++;
    }
    void previous() {
        this->pointer = ((ListNode*)this->pointer)->previousNode;
        this->position--;
    }

    void add(size_t pos) {
        size_t index = pos;
        while (index <= pos) {
            this->next(this);
            index--;
        }
    }
    void subtract(size_t pos) {
        size_t index = pos;
        while (index <= pos) {
            this->previous(this);
            index--;
        }
    }

    int equals(Iterator other) {
        if (this->pointer == other.pointer) {
            return 1;
        }
        return 0;
    }
};