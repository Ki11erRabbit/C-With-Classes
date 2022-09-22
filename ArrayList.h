#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Iterator.h"

#define foreach(item,arrayList) \
        for(size_t loopCheck = 1, count = 0, size = arrayList->length; loopCheck && count != size; loopCheck = !loopCheck, count++) \
            for(item = (arrayList->list) + count * arrayList->elementSize; loopCheck; loopCheck = !loopCheck)

class ArrayList {
    size_t length = 0;
    size_t maxLength = 0;
    size_t elementSize = 1;
    void* list;

    ArrayList ArrayList(size_t elemSize) {
        ArrayList newList;
        newList.elementSize = elemSize;
    }
    ArrayList newArrayList(size_t elemSize) {
        ArrayList *newList;
        newList->elementSize = elemSize;
    }
    void _ArrayList(ArrayList* list) {
        free(list->list);
    }
    void _newArrayList(ArrayList* list) {
        free(list->list);
        free(list);
    }

    Iterator begin() {
        return Iterator(this->list,this->elementSize,0);
    }
    Iterator end() {
        return Iterator(((char*)this->list) + this->elementSize * (this->length -1),this->elementSize,this->length -1)
    }

    size_t size() {
        return this->length;
    }
    void resize(size_t n) {
        this->list = realloc(this->list,n * this->elementSize);
        this->length = n;
        this->maxLength = n;
    }
    size_t capacity() {
        return this->maxLength;
    }
    int empty() {
        if (this->length == 0) {
            return 1;
        }
        return 0;
    }
    void reserve(size_t n) {
        if (this->list == NULL) {
            this->list = malloc(this->elementSize);
            this->maxLength++;
        }
        if (n > this->maxLength) {
            this->list = realloc(this->list, n * this->elementSize);
            this->maxLength = n;
        }
    }
    void shrink_to_fit() {
        if (this->length < this->maxLength) {
            this->list = realloc(this->list,this->length * this->elementSize);
            this->maxLength = this->length;
        }
    }

    void* at(size_t pos) {
        if (pos > this->length) {
            fprintf(stderr, "%s", "Error: out of bounds");
            exit(1);
        }

        return (char*)(this->list) + (pos * this->elementSize);
    }
    void* front() {
        return (char*)(this->list);
    }
    void* back() {
        return (char*)(this->list) + (this->elementSize * (this->length - 1));
    }
    void* data() {
        return this->list;
    }

    void assign(void* array, size_t arraySize) {
        if (this->list == NULL) {
            this->list = malloc(arraySize * this->elementSize);
            this->maxLength = arraySize;
        }
        if (arraySize > this->maxLength) {
            this->list = realloc(this->list,this->elementSize * arraySize);
            this->maxLength = arraySize;
        }
        for (size_t i = 0; i < arraySize * this->elementSize; i++) {
            ((char*)this->list)[i] = ((char*)array)[i];
        }
        this->length = arraySize;
    }
    void assignArrayList(ArrayList* array) {
        if (this->list == NULL) {
            this->list = malloc(array->length * this->elementSize);
            this->maxLength = array->length;
        }
        if (array->length > this->maxLength) {
            this->list = realloc(this->list,this->elementSize * array->length);
            this->maxLength = array->length;
        }
        for (size_t i = 0; i < array->length * this->elementSize; i++) {
            ((char*)this->list)[i] = ((char*)array->list)[i];
        }
        this->length = array->length;
    }
    void push_back(void* val) {
        if (this->list == NULL) {
            this->list = malloc(this->elementSize);
            memcpy(this->list,val,this->elementSize);
            this->maxLength++;
            this->length++;
            return;
        }
        else if (this->length == this->maxLength){
            size_t newSize = this->maxLength * 1.5;
            if (newSize <= 1)
                newSize = this->maxLength * 2;

            this->list = realloc(this->list,newSize * this->elementSize);
            this->maxLength = newSize;
        }
        memcpy((char*)(this->list)+this->length*this->elementSize,val,this->elementSize);
        this->length++;
    }
    void push_front(void* val) {
        if (this->list == NULL) {
            this->list = malloc(this->elementSize);
            memcpy(this->list,val,this->elementSize);
            this->maxLength++;
            this->length++;
            return;
        }
        else if (this->length == this->maxLength){
            size_t newSize = this->maxLength * 1.5;
            if (newSize <= 1)
                newSize = this->maxLength * 2;

            this->list = realloc(this->list,newSize* this->elementSize);
            this->maxLength = newSize;
        }
        void* tempPoint = NULL;
        tempPoint = malloc(this->length * this->elementSize);
        memcpy(tempPoint,this->list,this->length * this->elementSize);
        memcpy(this->list,val,this->elementSize);

        memcpy((char*)this->list + this->elementSize,tempPoint,this->length * this->elementSize);
        this->length++;
        free(tempPoint);
    }
    void insert(size_t pos, void* val) {
        if (this->list == NULL) {
            this->list = malloc(this->elementSize);
            memcpy(this->list,val,this->elementSize);
            this->maxLength++;
            this->length++;
            return;
        }
        else if (this->length == this->maxLength){
            size_t newSize = this->maxLength * 1.5;
            if (newSize <= 1)
                newSize = this->maxLength * 2;

            this->list = realloc(this->list,newSize * this->elementSize);
            this->maxLength = newSize;
        }
        void* tempPoint = NULL;
        tempPoint = malloc((this->length - pos) * this->elementSize);
        memcpy(tempPoint, (char*)this->list + pos,(this->length - pos) * this->elementSize);
        memcpy((char*)this->list + (pos * this->elementSize),val,this->elementSize);
        memcpy((char*)this->list + ((pos + 1)* this->elementSize),tempPoint,(this->length - pos) * this->elementSize);
        this->length++;
        free(tempPoint);
    }
    void erase(size_t pos) {
        void* tempPoint = NULL;
        tempPoint = malloc((this->length - pos) * this->elementSize);

        memcpy(tempPoint,(char*)this->list + ((pos +1 ) * this->elementSize),this->elementSize * (this->length - pos - 1));

        memcpy((char*)this->list + (pos * this->elementSize),tempPoint, this->elementSize * (this->length - pos));
        this->length--;
        free(tempPoint);
    }
    void eraseRange(size_t pos, size_t len) {
        void* tempPoint = NULL;
        tempPoint = malloc((this->length - len) * this->elementSize);

        memcpy(tempPoint,(char*)this->list + (this->elementSize * (len + 1)),this->elementSize * (this->length - len -1));

        memcpy((char*)this->list + (pos * this->elementSize),tempPoint,this->elementSize * (this->length - len -1));
        this->length = this->length - (len - pos) - 1;
        free(tempPoint);
    }
    void swap(ArrayList *list) {
        if (this->elementSize != list->elementSize) {
            fprintf(stderr, "%s", "Error: different type in vector");
            return;
        }

        free(this->list);
        this->list = NULL;
        this->list = list->list;
        this->length = list->length;
        this->maxLength = list->maxLength;
    }
    void clear() {
        for(size_t i = 0; i < this->length; i++) {
            ((char*)this->list)[i] = '\0';
        }
        this->length = 0;
    }

};