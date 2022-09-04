#include <stdio.h>
#define letsGOOOO 8
#define MACRO(num, str) {\
            printf("%d", num);\
            printf(" is");\
            printf(" %s number", str);\
            printf("\n");\
           }


class test {
    int data;

    int getData() {
        return data;
    }

    void setData(int data) {
        this->data = data;
    }


};