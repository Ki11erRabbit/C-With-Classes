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
#if VERBOSE >= 2
    int getData() {
        return data;
    }
#endif
    void setData(int data) {
        this->data = data;
    }


};