#include <stdbool.h>
#include <stdio.h>

struct Link
{
    char* Key;
    char* Data;    
    struct Link* NextLink;
    bool flag ;
};
typedef struct Link Link;
void push(Link **Head, char *key, char *data);

void DelList(Link **Head);
void Show(Link **Head);
char* ShowData(Link **Current, char* key);
void DelLink(Link **Head, char* k);
char *FindRec(Link **Head, char* k);

void Reset(Link **Head, Link **Current);
int IsEnded(Link **Head, Link **Current);
void GoNext(Link **Head, Link **Current);

#pragma once
