#include "Node.h"

class ListNode:Node {
    ListNode *nextNode = NULL;
    ListNode *previousNode = NULL;

    ListNode ListNode(void *data, size_t elemSize) {
        ListNode node;
        return node;
    }
    ListNode *newListNode(void *data, size_t elemSize) {
        ListNode *node = (ListNode*)malloc(sizeof(ListNode));
        return node;
    }
    ListNode ListNodeInit(void *data, size_t elemSize,ListNode *next, ListNode *previous) {
        ListNode node;
        node.nextNode = next;
        node.previousNode = previous;
        return node;
    }
    ListNode *newListNodeInit(void *data, size_t elemSize,ListNode *next, ListNode *previous) {
        ListNode *node = (ListNode*)malloc(sizeof(ListNode));
        node->nextNode = next;
        node->previousNode = previous;
        return node;
    }

    ListNode *getNext() {
        return this->nextNode;
    }
    ListNode *getPrevious() {
        return this->previousNode;
    }
    void setNext(ListNode *node) {
        this->nextNode = node;
    }
    void setPrevious(ListNode *node) {
        this->previousNode = node;
    }

};