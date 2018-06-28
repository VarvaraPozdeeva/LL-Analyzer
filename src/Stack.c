#include "Stack.h"

void CreateStack(struct Stack *st)
{
    st->CountData = 0;
    st->data = (char*)malloc(sizeof(char)*MaxSize);
    for (int i = 0; i < MaxSize; i++)
        st->data[i] = "";
}
void Push(struct Stack *st, char c)
{
    if (st->CountData < MaxSize)
        st->data[st->CountData++] = c;
}
char Pop(struct Stack *st)
{
    if (st->CountData != 0)
        return st->data[st->CountData--];
    return "";
}
char Top(struct Stack *st)
{
    if (st->CountData != 0)
        return st->data[st->CountData-1];
    return "";
}
bool IsEmpty(struct Stack *st)
{
    return st->CountData == 0;
}