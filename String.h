//
// Created by ki11errabbit on 9/10/22.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>




class String {
    char *str = NULL;
    size_t size;
    size_t maxSize;

    String String(char* str) {
        String newString;
        newString.str = strdup(str);
        newString.size = sizeof(str);
        newString.maxSize = sizeof(str);
        return newString;
    }
    String* newString(char* str) {
        String *newString;
        newString->str = strdup(str);
        newString->size = sizeof(str);
        newString->maxSize = sizeof(str);
        return newString;
    }

    void _String(String* this) {
        free(this->str);
    }
    void _newString(String* this) {
        free(this->str);
        free(this);
        this = NULL;
    }

    size_t getSize() {
        return this->size;
    }

    size_t length() {
        return this->size;
    }
    size_t capacity() {
        return this->maxSize;
    }
    int empty() {
        if (this->size == 0) {
            return 1;
        }
        else {
            return 0;
        }
    }
    void clear() {
        for (size_t i = 0; i < this->size; i++) {
            this->str[i] = '\0';
        }
    }
    void resize(size_t n) {
        for (size_t i = 0; i < this->maxSize; i++) {
            this->str[i] = '\0';
        }
        this->size = n;
    }
    void resizeWithChar(size_t n, char c) {
        for (size_t i = 0; i < this->maxSize; i++) {
            if (i <= n) {
                this->str[i] = c;
            }
            else {
                this->str[i] = '\0';
            }
        }
        this->size = n;
    }
    void shrink_to_fit() {
        if (this->size < this->maxSize) {
            this->str = (char*) realloc(this->str, this->size);
        }
    }

    char at(size_t pos) {
        if (pos > this->size) {
            fprintf(stderr,"%s", "String access out of bounds.");
            exit(1);
        }
        else {
            return this->str[pos];
        }
    }
    char back() {
        if (this->size == 0) {
            fprintf(stderr, "%s", "String too small");
            exit(1);
        }
        return this->str[this->size - 1];
    }
    char front() {
        return this->str[0];
    }

    String appendString(String *str) {
        size_t totalSize = this->size + str->size;
        if (totalSize > this->maxSize) {
            this->str = (char*) realloc(this->str, totalSize);
            this->maxSize = totalSize;
        }
        strcat(this->str,str->str);
        this->size = totalSize;

        return *this;
    }
    String appendSubstring(String *str, size_t subpos, size_t sublen) {
        if (sublen > str->size) {
            fprintf(stderr, "%s", "Error. Out of bounds access of substring");
            exit(1);
        }
        size_t totalSize = this->size + sublen - subpos;
        if (totalSize > this->maxSize) {
            this->str = (char*) realloc(this->str, totalSize);
            this->maxSize = totalSize;
        }
        for (size_t i = this->size; i < this->maxSize && subpos < sublen; i++) {
            this->str[i] = str->str[subpos];
            subpos++;
        }
        return *this;
    }
    String appendStr(char* s) {
        size_t strlength = strlen(s);
        size_t totalSize = this->size + strlength;
        if (totalSize > this->maxSize) {
            this->str = (char*) realloc(this->str, totalSize);
            this->maxSize = totalSize;
        }
        strcat(this->str,s);
        this->size = strlen(this->str);
        return *this;
    }
    String appendBuffer(char *buffer, size_t n) {
        size_t totalSize = this->size + n;
        if (totalSize > this->maxSize) {
            this->str = (char*)realloc(this->str,totalSize);
            this->maxSize = totalSize;
        }
        strncpy(this->str,buffer,n);
        this->size = strlen(this->str);
        return *this;
    }
    String appendFill(size_t n, char c) {
        size_t totalSize = this->size + n;
        if (totalSize > this->maxSize) {
            this->str = (char*)realloc(this->str, totalSize);
            this->maxSize = totalSize;
        }
        for (size_t i = this->size; this->maxSize - i < n; i++) {
            this->str[i] = c;
        }
        return *this;
    }

    void push_back(char c) {
        size_t totalSize = this->size + 1;
        if (totalSize > this->maxSize) {
            this->str = (char*) realloc(this->str, totalSize);
            this->maxSize = totalSize;
        }
        this->str[this->maxSize - 1] = c;
    }
    void pop_back() {
        if (this->size == 0) {
            fprintf(stderr,"%s", "Error. String is too small");
            exit(1);
        }
        this->str[this->size -1] = '\0';
        this->size = this->size -1;
    }
    String assignString(String *str) {
        free(this->str);
        this->str = strdup(str->str);
        this->size = strlen(this->str);
        this->maxSize = strlen(this->str);

        return *this;
    }
    String assignSubstring(String *str,size_t subpos, size_t sublen) {
        if (sublen > str->length()) {
            sublen = 0;
        }
        if (subpos > str->size) {
            fprintf(stderr, "%s", "Error: out of bounds");
            exit(1);
        }
        free(this->str);
        strncpy(this->str,str->str+subpos,sublen);

        return *this;
    }
    String assignStr(char *str) {
        free(this->str);
        this->str = strdup(str);
        this->size = strlen(this->str);
        this->maxSize = strlen(this->str);

        return *this;
    }
    String assignBuffer(char* buffer, size_t n) {
        free(this->str);
        strncpy(this->str,buffer,n);

        return *this;
    }
    String assignFill(size_t n, char c) {
        free(this->str);
        char *fillStr = (char*)malloc(sizeof(char) * n);
        for (size_t i = 0; i < n; i++) {
            fillStr[i] = c;
        }
        this->str = fillStr;

        return *this;
    }

    String insertString(size_t pos, String *str) {
        if (pos > this->size) {
            fprintf(stderr, "%s", "Error: out of bounds");
            exit(1);
        }
        size_t totalSize = this->size + str->size;
        if (totalSize > this->maxSize) {
            this->str = (char*) realloc(this->str,totalSize);
            this->maxSize = totalSize;
        }
        char* tempStr = strdup(this->str+pos);
        this->str[pos] = '\0';
        strcat(this->str,str->str);
        strcat(this->str,tempStr);
        free(tempStr);
        this->size = strlen(this->str);

        return *this;
    }
    String insertSubstring(size_t pos, String *str, size_t subpos, size_t sublen) {
        if (pos > this->size) {
            fprintf(stderr, "%s", "Error: out of bounds");
            exit(1);
        }
        if (sublen > str->size) {
            sublen = str->size;
        }
        size_t totalSize = this->size + str->size;
        if (totalSize > this->maxSize) {
            this->str = (char*) realloc(this->str,totalSize);
            this->maxSize = totalSize;
        }
        char* tempStr = strdup(this->str+pos);
        this->str[pos] = '\0';
        strncat(this->str,str->str+subpos,sublen);
        strcat(this->str,tempStr);
        free(tempStr);
        this->size = strlen(this->str);

        return *this;
    }
    String insertStr(size_t pos, char *s) {
        if (pos > this->size) {
            fprintf(stderr, "%s", "Error: out of bounds");
            exit(1);
        }
        size_t totalSize = this->size + strlen(s);
        if (totalSize > this->maxSize) {
            this->str = (char*) realloc(this->str,totalSize);
            this->maxSize = totalSize;
        }
        char* tempStr = strdup(this->str+pos);
        this->str[pos] = '\0';
        strcat(this->str,s);
        strcat(this->str,tempStr);
        free(tempStr);
        this->size = strlen(this->str);

        return *this;
    }
    String insertStr(size_t pos, char *buffer, size_t n) {
        if (pos > this->size) {
            fprintf(stderr, "%s", "Error: out of bounds");
            exit(1);
        }
        size_t totalSize = this->size + n;
        if (totalSize > this->maxSize) {
            this->str = (char*) realloc(this->str,totalSize);
            this->maxSize = totalSize;
        }
        char* tempStr = strdup(this->str+pos);
        this->str[pos] = '\0';
        strncat(this->str,buffer,n);
        strcat(this->str,tempStr);
        free(tempStr);
        this->size = strlen(this->str);

        return *this;
    }
    String insertFill(size_t pos, size_t n,char c) {
        if (pos > this->size) {
            fprintf(stderr, "%s", "Error: out of bounds");
            exit(1);
        }
        size_t totalSize = this->size + n;
        if (totalSize > this->maxSize) {
            this->str = (char*) realloc(this->str,totalSize);
            this->maxSize = totalSize;
        }
        char *fillStr = (char*)malloc(sizeof(char) * n);
        for (size_t i = 0; i < n; i++) {
            fillStr[i] = c;
        }
        char* tempStr = strdup(this->str+pos);
        this->str[pos] = '\0';
        strcat(this->str,fillStr);
        strcat(this->str,tempStr);
        free(tempStr);
        free(fillStr);
        this->size = strlen(this->str);

        return *this;
    }
    String insertChar(size_t pos, char c) {
        if (pos > this->size) {
            fprintf(stderr, "%s", "Error: out of bounds");
            exit(1);
        }
        size_t totalSize = this->size + 1;
        if (totalSize > this->maxSize) {
            this->str = (char*) realloc(this->str,totalSize);
            this->maxSize = totalSize;
        }

        char* tempStr = strdup(this->str+pos);
        this->str[pos] = '\0';
        strcat(this->str,&c);
        strcat(this->str,tempStr);
        free(tempStr);
        this->size = strlen(this->str);

        return *this;
    }

    String replaceString(size_t pos, size_t len, String* str) {
        if (len > this->size) {
            len = this->size;
        }
        if (pos > this->size) {
            fprintf(stderr, "%s", "Error: out of bounds");
            exit(1);
        }
        for(size_t i = pos; i < len; i++) {
            this->str[i] = '\0';
        }
        size_t totalSize = this->size + str->size;
        if (totalSize > this->maxSize) {
            this->str = (char*) realloc(this->str,totalSize);
            this->maxSize = totalSize;
        }
        if (len != this->size) {
            char *endChar = strdup(this->str+pos);
            (this->str+pos)[0] = '\0';

            strcat(this->str,str->str);
            strcat(this->str, endChar);
        }
        else {
            strcat(this->str,str->str);
        }
        this->size = strlen(this->str);

        return *this;
    }
    String replaceSubstring(size_t pos, size_t len, String* str, size_t subpos, size_t sublen) {
        if (len > this->size) {
            len = this->size;
        }
        if (sublen > str->size) {
            len = str->size;
        }
        if (pos > this->size || subpos > str->size) {
            fprintf(stderr, "%s", "Error: out of bounds");
            exit(1);
        }
        for(size_t i = pos; i < len; i++) {
            this->str[i] = '\0';
        }
        size_t totalSize = this->size + sublen - subpos;
        if (totalSize > this->maxSize) {
            this->str = (char*) realloc(this->str,totalSize);
            this->maxSize = totalSize;
        }
        if (len != this->size) {
            char *endChar = strdup(this->str+pos);
            (this->str+pos)[0] = '\0';

            strncat(this->str,str->str + subpos,sublen);
            strcat(this->str, endChar);
        }
        else {
            strncat(this->str,str->str + subpos,sublen);
        }
        this->size = strlen(this->str);

        return *this;
    }
    String replaceStr(size_t pos, size_t len, char* s) {
        if (len > this->size) {
            len = this->size;
        }
        if (pos > this->size) {
            fprintf(stderr, "%s", "Error: out of bounds");
            exit(1);
        }
        for(size_t i = pos; i < len; i++) {
            this->str[i] = '\0';
        }
        size_t totalSize = this->size + strlen(s);
        if (totalSize > this->maxSize) {
            this->str = (char*) realloc(this->str,totalSize);
            this->maxSize = totalSize;
        }
        if (len != this->size) {
            char *endChar = strdup(this->str+pos);
            (this->str+pos)[0] = '\0';

            strcat(this->str,s);
            strcat(this->str, endChar);
        }
        else {
            strcat(this->str,s);
        }
        this->size = strlen(this->str);

        return *this;
    }
    String replaceBuffer(size_t pos, size_t len, char* buffer, size_t n) {
        if (len > this->size) {
            len = this->size;
        }
        if (pos > this->size) {
            fprintf(stderr, "%s", "Error: out of bounds");
            exit(1);
        }
        for(size_t i = pos; i < len; i++) {
            this->str[i] = '\0';
        }
        size_t totalSize = this->size + n;
        if (totalSize > this->maxSize) {
            this->str = (char*) realloc(this->str,totalSize);
            this->maxSize = totalSize;
        }
        if (len != this->size) {
            char *endChar = strdup(this->str+pos);
            (this->str+pos)[0] = '\0';

            strncat(this->str,buffer,n);
            strcat(this->str, endChar);
        }
        else {
            strncat(this->str,buffer,n);
        }
        this->size = strlen(this->str);

        return *this;
    }
    String replaceFill(size_t pos, size_t len, size_t n, char c) {
        if (len > this->size) {
            len = this->size;
        }
        if (pos > this->size) {
            fprintf(stderr, "%s", "Error: out of bounds");
            exit(1);
        }
        for(size_t i = pos; i < len; i++) {
            this->str[i] = '\0';
        }
        size_t totalSize = this->size + n;
        if (totalSize > this->maxSize) {
            this->str = (char*) realloc(this->str,totalSize);
            this->maxSize = totalSize;
        }
        char* fillStr = (char*) malloc(sizeof(char) * n);
        if (len != this->size) {
            char *endChar = strdup(this->str+pos);
            (this->str+pos)[0] = '\0';

            strcat(this->str,fillStr);
            strcat(this->str, endChar);
        }
        else {
            strcat(this->str,fillStr);
        }
        this->size = strlen(this->str);
        free(fillStr);

        return *this;
    }

    String erase(size_t pos) {
        if (pos > this->size) {
            fprintf(stderr, "%s", "Error: out of bounds");
            exit(1);
        }
        this->str[pos] = '\0';
        strcat(this->str,this->str+pos);
        this->size = this->size -1;

        return *this;
    }
    String eraseSubstring(size_t pos, size_t len) {
        if (pos > this->size || len > this->size) {
            fprintf(stderr, "%s", "Error: out of bounds");
            exit(1);
        }
        for (size_t i = pos; i <= len; i++) {
            this->str[i] = '\0';
        }
        this->size = this->size - (len - pos);
        strcat(this->str, this->str+pos);

        return *this;
    }
    String eraseChar(char c) {
        for (size_t i = 0; i < this->size; i++) {
            if (this->str[i] == c) {
                this->erase(this,i);
                break;
            }
        }

        return *this;
    }
    String swap(String *str) {
        char* tempStr = NULL;
        size_t tempSize, tempMaxSize;

        tempStr = this->str;
        tempSize = this->size;
        tempMaxSize = this->maxSize;

        this->str = str->str;
        this->size = str->size;
        this->maxSize = str->maxSize;

        str->str = tempStr;
        str->size = tempSize;
        str->maxSize = tempMaxSize;

        return *this;
    }

    const char* c_str() {
        return strdup(this->str);
    }
    size_t copy(char* s, size_t len, size_t pos) {
        if (pos > this->size) {
            fprintf(stderr, "%s", "Error: out of bounds");
            exit(1);
        }
        if (len > this->size)
            len = this->size;
        if (pos != 0) {
            strncat(s,this->str+pos,len);
        }
        else {
            strncat(s,this->str,len);
        }
        return len - pos;
    }
    size_t findString(String *str) {
        char *start = strstr(this->str,str->str);
        return (size_t)(start - this->str);
    }
    size_t findStr(char* s) {
        char *start = strstr(this->str,s);
        return (size_t)(start - this->str);
    }

    String substr(size_t pos, size_t len) {
        if (pos > this->size) {
            fprintf(stderr, "%s", "Error: out of bounds");
            exit(1);
        }
        if (pos == this->size) {
            return String__String("");
        }
        if (len > this->size)
            len = this->size;

        char *tempChar = strndup(this->str+pos,len-pos);
        String newString = String__String(tempChar);
        free(tempChar);
        return newString;
    }
    int compareString(String *str) {
        return strcmp(this->str,str->str);
    }
    int compareStr(char *s) {
        return strcmp(this->str,s);
    }
};