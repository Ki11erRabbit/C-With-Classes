#include "Node.h"

class ListNode /*implements Node*/ {
    ListNode *nextNode = NULL;
    ListNode *previousNode = NULL;

    ListNode ListNode(void *data, size_t elemSize) {
        TreeNode node;
        return node;
    }
    ListNode *newListNode(void *data, size_t elemSize) {
        ListNode *node;
        return node;
    }

    ListNode getNext() {
        return *this->nextNode;
    }
    ListNode getPrevious() {
        return *this->previousNode;
    }
    void setNext(ListNode *node) {
        this->nextNode = node;
    }
    void setPrevious(ListNode *node) {
        this->previousNode = node;
    }

};