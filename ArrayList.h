#include <stdlib.h>
#include <stdio.h>
#include <string.h>

class ArrayList {
    size_t length = 0;
    size_t maxLength = 0;
    size_t elementSize = 1;
    void* list;

    ArrayList ArrayListList(size_t elemSize) {
        ArrayList newList;
        newList.elementSize = elemSize;
    }
    ArrayList newArrayListList(size_t elemSize) {
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

    void assign() {

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
            size_t newSize = (this->maxLength * this->elementSize) * 1.5;
            if (newSize <= 1)
                newSize = (this->maxLength * this->elementSize) * 2;

            this->list = realloc(this->list,newSize);
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
            size_t newSize = (this->maxLength * this->elementSize) * 1.5;
            if (newSize <= 1)
                newSize = (this->maxLength * this->elementSize) * 2;

            this->list = realloc(this->list,newSize);
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
            size_t newSize = (this->maxLength * this->elementSize) * 1.5;
            if (newSize <= 1)
                newSize = (this->maxLength * this->elementSize) * 2;

            this->list = realloc(this->list,newSize);
            this->maxLength = newSize;
        }
        void* tempPoint = NULL;
        tempPoint = malloc((this->length - pos) * this->elementSize);
        memcpy(tempPoint, (char*)this->list + pos,(this->length - pos) * this->elementSize);
        memcpy((char*)this->list + (pos * this->elementSize),tempPoint,(this->length - pos) * this->elementSize);
        this->length++;
        free(tempPoint);
    }
    void erase(size_t pos) {
        void* tempPoint = NULL;
        tempPoint = malloc((this->length - pos) * this->elementSize);

        memcpy(tempPoint,(char*)this->length + (pos * this->elementSize),this->elementSize * (this->length - pos));
        free((char*)this->list + (pos * this->elementSize));
        memcpy((char*)this->list + (pos * this->elementSize),tempPoint, this->elementSize * (this->length - pos));
        this->length--;
        free(tempPoint);
    }
    void eraseRange(size_t pos, size_t len) {
        void* tempPoint = NULL;
        tempPoint = malloc((this->length - len) * this->elementSize);

        memcpy(tempPoint,(char*)this->length,this->elementSize * (this->length - len));
        free((char*)this->list + (pos * this->elementSize));
        memcpy((char*)this->list + (pos * this->elementSize),tempPoint,this->elementSize * (this->length - len));
        this->length = this->length - (len - pos);
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