#define _CRT_SECURE_NO_WARNINGS
#include "Analyzer.h"

int main(int argc, char *argv[])
{
    char string[10] ;
    if (argc < 2)
    {
        printf("Enter string with '$' at the end ");
        scanf("%s", &string);
    }
    else
        *string = argv[1];
    while (1)
    {
        if (string[strlen(string) - 1] != '$')
        {
            printf("Enter string with '$' at the end ");
            scanf("%s", &string);
        }
        else break;
    }

    char* res;
    Gramm *gr =  (Gramm*)calloc(1, sizeof(Gramm));   

    CreateGrammar(gr, "S", 2, 3, "SB", "012");
    AddRule(gr,"S", "B2");
    AddRule(gr ,"B", "0");
    AddRule(gr, "B", "1");

    First(gr, gr->Start);
    Follow(gr);
    TableAnalysis(gr);      
    
    CreateLogFile(gr, string, Analysis(gr, string)); 

    Clear(gr);
    free(gr);
   
    return 0;
}