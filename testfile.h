#include <stdio.h>
#include "test.h"
#define letsGOOOO 8
#define SUM(x,y) (x+y)
#define SWITCH(x,y) x^y; x^y
#define MACRO(num, str) {\
            printf("%d", num);\
            printf(" is");\
            printf(" %s number", str);\
            printf("\n");\
           }
#define MAC(a,lim) while (a < lim) { \
            {\
            printf("Aqsa ");\
            a++;\
            }
typedef struct temp {
    int x,z;
    int y;
} Temp;
typedef long int hello;
typedef struct jes newStruct;

struct jes {
    int x,z;
    int y;
};
struct phil {
    int x,z;
    int y;
};
struct bob {
    int x,z;
    int y;
};
class test {
    int data;
    Temp hold;
    hello world;

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