#include "mystring.h"
// Hi TA, I am Andy Lu
char *mystrchr(const char *s, int c){
    int idx = 0;
    while(*(s+idx)){
        if(c==*(s+idx))return (char *)s+idx;
        if(!c) return NULL;
        idx++;
    }
}

char *mystrrchr(const char *s, int c){

    int idx = 0;
    while(*(s+idx))idx++;
    for(;idx>=0;idx--)if(*(s+idx)==c)return (char *)s+idx;
    return NULL;

}

size_t mystrspn(const char *s, const char *accept){

    int idx=0;
    while(*(s+idx)){
        int j=0;
        while(*(accept+j)){
            if(*(accept+j) == *(s+idx)) break;
            j ++;
        }
        if(!*(accept+j))return idx;
        idx++;
    }
    return idx;

}

size_t mystrcspn(const char *s, const char *accept){

    int idx=0;
    while(*(s+idx)){
        int j=0;
        while(*(accept+j)){
            if(*(accept+j) == *(s+idx)) break;
            j ++;
        }
        if(*(accept+j))return idx;
        idx++;
    }
    return idx;

}

char *mystrpbrk(const char *s, const char *accept){
    int idx=0;
    while(*(s+idx)){
        int j=0;
        while(*(accept+j)){
            if(*(accept+j) == *(s+idx)) break;
            j ++;
        }
        if(*(accept+j))return (char *)s+idx;
        idx++;
    }
    return NULL;
}

char *mystrstr(const char *haystack , const char *needle){

    int idx=0;
    while(*(haystack+idx)){
        int i=0;
        while(*(needle+i)){
            if(*(haystack+idx+i)!=*(needle+i)||!*(haystack+idx+i))break;
            i++;
        }
        if(*(needle+i)){
            idx++;
            continue;
        }
        return (char *)haystack+idx;
    }
    return NULL;

}

char *mystrtok(char *str, const char *delim){

    static char *string;
    if(!str)str = string;
    else string = str;
    if(!string)return NULL;
    while(indelim(string,delim)){
        *string = 0;
        string ++;
    }
    static char *next , *old;
    old = string;
    next = string;
    while(!indelim(next,delim))next++;
    while(indelim(next,delim)){
        *next = 0;
        next++;
    }
    string = next;
    return old;
}

int indelim(char *str, const char *delim){

    int idx = 0;
    while(*(delim+idx)){
        if(*str==*(delim+idx))return 1;
        idx ++;
    }
    return 0;
}