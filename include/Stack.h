#include <stdbool.h>
#include <stdio.h>

#define MaxSize 15

struct Stack
{
    char *data;
    int CountData;
};

void CreateStack(struct Stack *st);
void Push(struct Stack *st, char c);
char Pop(struct Stack *st);
char Top(struct Stack *st);
bool IsEmpty(struct Stack *st);