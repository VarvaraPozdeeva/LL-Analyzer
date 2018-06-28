#include "List.h"
#include <malloc.h>
#include <string.h>

void push(Link **Head, char* key, char* data)
{
    Link *tmp = (Link*)malloc(sizeof(Link));

    tmp->Data = (char*)malloc(sizeof(char)*strlen(data));
    strcpy_s(tmp->Data, sizeof(data), data);
        
    tmp->Key = (char*)malloc(sizeof(char)*strlen(key));
    strcpy_s(tmp->Key, sizeof(key), key);
    tmp->flag = false;

    if ((*Head) != NULL)
    {
        tmp->NextLink = (*Head)->NextLink;
        (*Head)->NextLink = tmp;
    }
    else
    {
        tmp->NextLink = NULL;
        (*Head) = tmp;
    }
}

void DelList(Link **Head)
{
    Link *tmp;
    while ((*Head))
    {
        tmp = (*Head);
        (*Head) = (*Head)->NextLink;
        free(tmp);
    }
}

void Show(Link **Head)
{
    Link  **pptr;
    pptr = &(*Head);
    while (*pptr)
    {
        printf("%s -> %s \n", (*pptr)->Key, (*pptr)->Data);
        pptr = &(*pptr)->NextLink;
    }
}
char* ShowData(Link **Current, char* key)
{
    Link  **pptr;
    char* data;

    pptr = &(*Current);
    if (*pptr != NULL)
    {
        while (*pptr)
        {
            if (strcmp((*pptr)->Key, key) == 0)
            {
                data = (*pptr)->Data;
                (*Current) = (*pptr)->NextLink;
                return data;
            }
            pptr = &(*pptr)->NextLink;
        }
    }
    return "";
}
void DelLink(Link **Head, char* k)
{
    Link  **pptr, *prev, *link;
    pptr = &(*Head);
    prev = NULL;

    if (*pptr != NULL)
    {
        while (*pptr)
        {
            if (strcmp((*pptr)->Key, k) == 0)
            {
                link = *pptr;
                if (pptr == &(*Head))
                    (*Head) = link->NextLink;
                else
                    prev->NextLink = link->NextLink;
                //free(link->Data);
                //free(link->Key);
                free(link);
                break;
            }
            prev = pptr;
            pptr = &(*pptr)->NextLink;
        }
    }
}
int Find(Link **Head, char k)
{
    Link  **pptr;
    pptr = &(*Head);


    if (*pptr != NULL)
    {
        while (*pptr)
        {
            if ((*pptr)->Key[0] == k)
            {
                return 1;
            }
            pptr = &(*pptr)->NextLink;
        }
    }
    return 0;
}
char* FindRec(Link **Head, char* k)
{
    Link  **pptr;
    pptr = &(*Head);


    if (*pptr != NULL)
    {
        while (*pptr)
        {
            if ((*pptr)->Key[0] == k)
            {
                return (*pptr)->Data;
            }
            pptr = &(*pptr)->NextLink;
        }
    }
    return "";
}

void Reset(Link **Head, Link **Current)
{
    (*Current) = (*Head);
}
int IsEnded(Link **Head, Link **Current)
{
    return (*Current) == NULL;
}
void GoNext(Link **Head, Link **Current)
{
    if (!IsEnded(Head, Current))
    {
        (*Current) = (*Current)->NextLink;
    }
}