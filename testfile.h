#include <stdio.h>
#define letsGOOOO 8
#define MACRO(num, str) {\
            printf("%d", num);\
            printf(" is");\
            printf(" %s number", str);\
            printf("\n");\
           }

typedef struct temp {
    int x;
    int y;
} Temp;

class test {
    int data;

    //returns data
    int getData() {
        return data;
    }

    /*
     * multiline comment
     */
    void setData(int data) {
        this->data = data;
    }


};