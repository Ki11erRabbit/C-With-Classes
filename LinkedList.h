#include "ListNode.h"
#include "Iterator.h"
#include <stdio.h>


class LinkedList {
    ListNode *head;
    ListNode *tail;
    size_t numNodes = 0;
    size_t elemSize = 1;

    LinkedList LinkedList(size_t elemSize) {
        LinkedList newList;
        newList.elemSize = elemSize;
        return newList;
    }
    LinkedList *newLinkedList(size_t elemSize) {
        LinkedList *newList;
        newList->elemSize = elemSize;

        return newList;
    }
    void _LinkedList(LinkedList *list) {
        size_t maxSize = list->numNodes;
        while (list->numNodes < maxSize) {
            list->pop_back(list);
        }
        list->head = NULL;
        list->tail = NULL;
    }
    void _newLinkedList(LinkedList *list) {
        size_t maxSize = list->numNodes;
        while (list->numNodes < maxSize) {
            list->pop_back(list);
        }
        list->head = NULL;
        list->tail = NULL;
        free(list);
        list = NULL;
    }


    Iterator begin() {
        return Iterator__Iterator(head,this->elemSize,0,1);
    }
    Iterator end() {
        return Iterator__Iterator(tail,this->elemSize,this->numNodes,1);
    }

    int empty() {
        if (this->head == NULL && this->tail == NULL) {
            return 1;
        }
        return 0;
    }
    size_t size() {
        return this->numNodes;
    }

    void *front() {
        return this->head->getData();
    }
    void *back() {
        return this->tail->getData();
    }

    void assign(size_t n, void *val) {

    }

    void push_front(const void *val) {
        ListNode *newNode = ListNode__newListNodeInit(val,this->elemSize,this->head,NULL);
        if (this->numNodes == 0) {
            this->tail = newNode;
        }
        else {
            this->head->previousNode = newNode;
        }
        this->head = newNode;
        numNodes++;
    }
    void pop_front() {
        if (this->head == NULL) {
            printf("Empty Linked Lis!");
            return;
        }
        ListNode *newHead = this->head->nextNode;
        if (newHead == NULL) {
            this->tail = NULL;
        }
        else {
            newHead->previousNode = NULL;
        }

        ListNode___newListNode(this->head);
        this->head = newHead;
        numNodes--;
    }
    void push_back(const void *val) {
        ListNode *newNode = ListNode__newListNodeInit(val,this->elemSize,NULL,this->tail);
        if (this->numNodes == 0) {
            this->head = newNode;
        }
        else {
            this->tail->nextNode = newNode;
        }
        this->tail = newNode;
        numNodes++;
    }
    void pop_back() {
        if (this->tail == NULL) {
            printf("Empyt Linked List!");
            return;
        }
        ListNode *newTail = this->tail->previousNode;
        if (newTail == NULL) {
            this->head = NULL;
        }
        else {
            newTail->nextNode = NULL;
        }

        ListNode___newListNode(tail);
        this->tail = newTail;
        numNodes--;
    }
    void swap(LinkedList *other) {
        if (this->elemSize != other->elemSize) {
            fprintf(stderr, "%s", "Error: different type in stack");
            return;
        }

        size_t tempCount = this->numNodes;
        ListNode *tempHead = this->head;
        ListNode *tempTail = this->tail;

        this->head = other->head;
        this->tail = other->tail;
        this->numNodes = other->numNodes;

        other->head = tempHead;
        other->tail = tempTail;
        other->numNodes = tempCount;

    }
    void clear() {
        if (this->head != NULL) {
            ListNode *currNode = this->head;
            while (currNode != NULL) {
                Node *nextNode = currNode->next;
                ListNode___newListNode(currNode);
            }
            this->head = NULL;
            this->tail = NULL;
            printf("List Cleared");
            this->numNodes = 0;
        }
        else {
            printf("There are no nodes.");
        }
    }
};