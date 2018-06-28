#include "Analyzer.h"

void CreateGrammar(Gramm *gr, char * start, int numNotTerm, int numTerm, char * notTerm, char* term)
{
    gr->FirstCur = NULL;
    gr->FirstHead = NULL;
    gr->FollowCur = NULL;
    gr->FollowHead = NULL;
    gr->RuleCur = NULL;
    gr->RuleHead = NULL;
    gr->Start = (char*)malloc(sizeof(char));
    gr->Start = start;
    gr->NumNotTerm = numNotTerm;
    gr->NumTerm = numTerm;
    gr->NotTerm = (char*)malloc(sizeof(char)*numNotTerm);
    gr->Term = (char*)malloc(sizeof(char)*numTerm);
    for (int i = 0; i < gr->NumTerm; i++)
        gr->Term[i] = term[i];
    for (int i = 0; i < gr->NumNotTerm; i++)
        gr->NotTerm[i] = notTerm[i];


    gr->Table = (char***)calloc(gr->NumNotTerm, sizeof(char**) /* gr->NumNotTerm*/);
    for (int i = 0; i < gr->NumNotTerm; i++)
    {
        gr->Table[i] = (char**)calloc(gr->NumTerm, sizeof(char*)/*gr->NumTerm*/);
        for (int j = 0; j < gr->NumTerm; j++)
        {
            gr->Table[i][j] = (char*)malloc(sizeof(char) * 3);
            gr->Table[i][j] = "";
        }
    }
}
void AddRule(Gramm * gr, char * key, char * data)
{
    push(&(gr->RuleHead), key, data);
}
bool IsTerm(char* s)
{
    if (!IsRule(s))
    {
        if (atoi(s) == 0 && strchr(s, '0') == 0)
            return 0;
        else
            return 1;
    }
    else
        return 0;
}
bool IsNotTerm(char* s)
{
    if (!IsRule(s))
    {
        if (!IsTerm(s))
            return 1;
    }
    else return 0;

}
bool IsRule(char* s)
{
    if (strlen(s) > 1)
        return 1;
    else return 0;
}

bool isTerm(Gramm *gr, char c)
{
    for (int i = 0; i < gr->NumTerm; i++)
        if (c == gr->Term[i])
            return true;
    return false;
}
bool isNotTerm(Gramm *gr, char c)
{
    for (int i = 0; i < gr->NumNotTerm; i++)
        if (c == gr->NotTerm[i])
            return true;
    return false;
}


char* First(Gramm *gr, char *X)
{
    if (IsTerm(X))
    {
        push(&(gr->FirstHead), X, X);
        return X;
    }
    else if (IsNotTerm(X))
    {
        if (X == gr->Start)
            push(&(gr->FirstHead), "&", "&");

        Link *Cur = gr->RuleHead;
        Link  *CurF = gr->FirstHead;

        while (1) //обработать все правила и в них найти first() и добавить к множеству
        {
            char* rule = ShowData(&Cur, X);

            if (rule == "")
                break;
            else if (rule == "e")
                push(&(gr->FirstHead), X, rule);
            else if (IsTerm(rule))
                push(&(gr->FirstHead), X, First(gr, rule));
            else
            {
                First(gr, rule);
                while (1)
                {
                    char* term = ShowData(&CurF, rule);
                    if (term == "")    break;
                    push(&(gr->FirstHead), X, term);
                }
            }
        }

        if (X == gr->Start)
        {
            char* str = (char*)malloc(sizeof(char));
            DelLink(&(gr->FirstHead), "&");
            for (int i = 0; i < gr->NumTerm; i++)
            {
                str[0] = gr->Term[i];
                str[1] = '\0';
                if (Find(&(gr->FirstHead), gr->Term[i]) == 0)
                    First(gr, str);
            }
            for (int i = 0; i < gr->NumNotTerm; i++)
            {
                str[0] = gr->NotTerm[i];
                str[1] = '\0';
                if (Find(&(gr->FirstHead), gr->NotTerm[i]) == 0)
                    First(gr, str);
            }
            return "";
            free(str);
        }
    }
    else if (IsRule(X)) //найти first() для первого символа
    {
        char* first = (char*)malloc(sizeof(char*) * 4);
        Link  *CurF = gr->FirstHead;

        strncpy_s(first, 4, X, 1);
        First(gr, first);

        while (1)
        {
            char* tmp = ShowData(&CurF, first);
            if (tmp == "")    break;
            push(&(gr->FirstHead), X, tmp);
        }
        free(first);
    }
}
char* Follow(Gramm *gr)
{
    int i = 0;
    char* X;
    for (Reset(&(gr->RuleHead), &(gr->RuleCur)); !IsEnded(&(gr->RuleHead), &(gr->RuleCur)); GoNext(&(gr->RuleHead), &(gr->RuleCur)))
    {
        X = gr->RuleCur->Key;
        if (strcmp(X, gr->Start) == 0)
            push(&(gr->FollowHead), X, "$");

        char * rule = (char*)malloc(sizeof(char)*strlen(gr->RuleCur->Data));
        rule = gr->RuleCur->Data;
        int lenRul = strlen(rule);

        for (int i = 0; i < lenRul - 1; i++)
        {
            if (isNotTerm(gr, rule[i]))
            {
                char* str = (char*)malloc(sizeof(char));
                if (rule[i + 1] != '\0')
                {
                    str[0] = rule[i];
                    str[1] = '\0';
                    char* tmp = FindRec(&(gr->FirstHead), rule[i + 1]);
                    for (int i = 0; i < strlen(tmp); i++)
                        if (tmp[i] == 'e')
                            for (int j = i; j<strlen(tmp) - 1; j++)
                                tmp[j] = tmp[j + 1];
                    push(&(gr->FollowHead), str, tmp);
                }
                else
                {
                    for (Reset(&(gr->FollowHead), &(gr->FollowCur)); !IsEnded(&(gr->FollowHead), &(gr->FollowCur)); GoNext(&(gr->FollowHead), &(gr->FollowCur)))
                        if (gr->FollowCur->Key == X)
                            push(&(gr->FollowHead), rule[i], gr->FollowCur->Data);
                }
            }
        } 
    }
}
void TableAnalysis(Gramm *gr)
{
    for (Reset(&(gr->RuleHead), &(gr->RuleCur)); !IsEnded(&(gr->RuleHead), &(gr->RuleCur)); GoNext(&(gr->RuleHead), &(gr->RuleCur)))
        for (int i = 0; i < gr->NumNotTerm; i++)
            if (gr->NotTerm[i] == gr->RuleCur->Key[0])
                for (Reset(&(gr->FirstHead), &(gr->FirstCur)); !IsEnded((gr->FirstHead), &(gr->FirstCur)); GoNext((gr->FirstHead), &(gr->FirstCur)))
                    if (strcmp(gr->FirstCur->Key, gr->RuleCur->Data) == 0)
                        for (int j = 0; j < gr->NumTerm; j++)
                            if (gr->Term[j] == gr->FirstCur->Data[0])
                            {
                                char* first = gr->FirstCur->Data;
                                int k = 0;
                                int len = strlen(first);
                                while (k < len)
                                    if (first != 'e')
                                        k++;
                                    else
                                    {
                                        char* str = (char*)malloc(sizeof(char) * 2);
                                        str[0] = gr->NotTerm[i];
                                        str[1] = '\0';
                                        char* term = FindRec(&(gr->FollowHead), str);
                                        for (k = 0; k < gr->NumTerm; k++)
                                            if (gr->Term[k] == term[0])
                                                gr->Table[i][k] = gr->FirstCur->Key;
                                        free(str);
                                    }
                                gr->Table[i][j] = gr->FirstCur->Key;
                            }
    for (int i = 0; i < gr->NumNotTerm; i++)
        for (int j = 0; j < gr->NumTerm; j++)
            if (gr->Table[i][j] == "")
                gr->Table[i][j] = "error";

}
void ShowTable(Gramm *gr)
{
    printf("\n+----+");
    for (int i = 0; i < gr->NumTerm; i++)
        printf("-----------+");
    printf("\n");
    printf("|    |");
    for (int j = 0; j < gr->NumTerm; j++)
        printf("   %c       |", gr->Term[j]);

    printf("\n+----+");
    for (int i = 0; i < gr->NumTerm; i++)
        printf("-----------+");
    printf("\n");
    for (int i = 0; i < gr->NumNotTerm; i++)
    {
        printf("|%c   |", gr->NotTerm[i]);
        for (int j = 0; j < gr->NumTerm; j++)
        {
           printf("%c -> %s", gr->NotTerm[i], gr->Table[i][j]);
           for (int k = 6 - strlen(gr->Table[i][j]); k > 0; k--)
               printf(" ");
           printf("|");
        }

        printf("\n+----+");
        for (int i = 0; i < gr->NumTerm; i++)
            printf("-----------+");
        printf("\n");
    }     
}
char* FindTableRec(Gramm *gr, char X, char a)
{
    for (int i = 0; i < gr->NumNotTerm; i++)
    {
        if (gr->NotTerm[i] == X)
            for (int j = 0; j < gr->NumTerm; j++)
                if (gr->Term[j] == a)
                    return gr->Table[i][j];
    }
    return "error";
}
bool CheckRule(Gramm *gr, char X, char* str)
{
    for (Reset(&(gr->RuleHead), &(gr->RuleCur)); !IsEnded(&(gr->RuleHead), &(gr->RuleCur)); GoNext(&(gr->RuleHead), &(gr->RuleCur)))
    {
        if (gr->RuleCur->Key[0] == X)
            if (strcmp(gr->RuleCur->Data, str) == 0)
                return true;
    }
    return false;
}
char* Analysis(Gramm *gr, char* string)
{
    struct Stack St;
    CreateStack(&St);
    Push(&St, '$'); Push(&St, gr->Start[0]);
    int currPos = 0;
    char a = string[currPos];
    char X = Top(&St);
    while (!IsEmpty(&St))
        if (isTerm(gr, X) || X == '$')
            if (X == a)
            {
                Pop(&St);
                currPos++;
                a = string[currPos];
                if (X == a == '$')
                    break;
                X = Top(&St);
            }
            else
                return "Incorrectly";
        else if (CheckRule(gr, X, FindTableRec(gr, X, a)))
        {
            Pop(&St);
            char* rule = (char*)malloc(sizeof(char)*strlen(FindTableRec(gr, X, a)));
            rule = FindTableRec(gr, X, a);
            int i = strlen(rule);
            for (i; i > 0; i--)
                Push(&St, rule[i - 1]);
            X = Top(&St);
        }
        else return "Incorrectly";
    return "Correctly";
}

void CreateLogFile(Gramm * gr, char* str, char* res)
{
    FILE *file;
    if (fopen_s( &file, "log.txt", "w") == NULL)
    {
        fprintf(file,"FIRST\n");
        fprintf(file, "---------------\n");
        for (int i = 0; i < gr->NumNotTerm; i++)
        {
            fprintf(file, "FIRST(%c) = {", gr->NotTerm[i]);
            for (Reset(&(gr->FirstHead), &(gr->FirstCur)); !IsEnded((gr->FirstHead), &(gr->FirstCur)); GoNext((gr->FirstHead), &(gr->FirstCur)))
            {
                if(gr->FirstCur->Key[0] == gr->NotTerm[i]&& gr->FirstCur->Key[1] == '\0')
                    fprintf(file, " %s", gr->FirstCur->Data);
            }
            fprintf(file, " }\n");
        }
        for (int i = 0; i < gr->NumTerm; i++)
        {
            fprintf(file, "FIRST(%c) = {", gr->Term[i]);
            for (Reset(&(gr->FirstHead), &(gr->FirstCur)); !IsEnded((gr->FirstHead), &(gr->FirstCur)); GoNext((gr->FirstHead), &(gr->FirstCur)))
            {
                if (gr->FirstCur->Key[0] == gr->Term[i])
                    fprintf(file, " %s", gr->FirstCur->Data);
            }
            fprintf(file, " }\n");
        }
        fprintf(file, "---------------\n");
        fprintf(file, "FOLLOW\n");
        fprintf(file, "---------------\n");
        for (int i = 0; i < gr->NumNotTerm; i++)
        {
            fprintf(file, "FOLLOW(%c) = {", gr->NotTerm[i]);
            for (Reset(&(gr->FollowHead), &(gr->FollowCur)); !IsEnded((gr->FollowHead), &(gr->FollowCur)); GoNext((gr->FollowHead), &(gr->FollowCur)))
            {
                if (gr->FollowCur->Key[0] == gr->NotTerm[i] && gr->FollowCur->Key[1] == '\0')
                    fprintf(file, " %s", gr->FollowCur->Data);
            }
            fprintf(file, " }\n");
        }
        
        fprintf(file, "---------------\n");
        fprintf(file, "ANALYSIS TABLE \n");
        fprintf(file, "---------------\n");



        fprintf(file, "\n+----+");
        for (int i = 0; i < gr->NumTerm; i++)
            fprintf(file, "-----------+");
        fprintf(file, "\n");
        fprintf(file, "|    |");
        for (int j = 0; j < gr->NumTerm; j++)
            fprintf(file, "   %c       |", gr->Term[j]);

        fprintf(file, "\n+----+");
        for (int i = 0; i < gr->NumTerm; i++)
            fprintf(file, "-----------+");
        fprintf(file, "\n");
        for (int i = 0; i < gr->NumNotTerm; i++)
        {
            fprintf(file, "|%c   |", gr->NotTerm[i]);

            for (int j = 0; j < gr->NumTerm; j++)
            {
                fprintf(file, "%c -> %s", gr->NotTerm[i], gr->Table[i][j]);
                for (int k = 6 - strlen(gr->Table[i][j]); k > 0; k--)
                    fprintf(file, " ");
                fprintf(file, "|");
            }

            fprintf(file, "\n+----+");
            for (int i = 0; i < gr->NumTerm; i++)
                fprintf(file, "-----------+");
            fprintf(file, "\n");
        }

        fprintf(file, "\nThis string (%s) was cheked and  reslat is \"%s \" ", str, res);

        fclose(file);
    }
    else printf("error file open");
}

void Clear(Gramm * gr)
{
    DelList(&(gr->FirstHead));
    DelList(&(gr->FollowHead));
    DelList(&(gr->RuleHead));
    free(gr->NotTerm);
    free(gr->Term);
    for (int i = 0; i < gr->NumNotTerm; i++)
       free(gr->Table[i]);
    free(gr->Table);
    gr->FirstCur = NULL;
    gr->FollowCur = NULL;
    gr->RuleCur = NULL;
}
