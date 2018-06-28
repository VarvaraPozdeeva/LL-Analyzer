#include "List.h"
#include "Stack.h"

typedef struct Grammatic
{
    char *Start;

    int NumTerm;
    int NumNotTerm;

    char *Term;
    char *NotTerm;
    Link *RuleHead;
    Link *RuleCur;
    Link *FirstHead;
    Link *FirstCur;
    Link *FollowHead;
    Link *FollowCur;

    char ***Table;
}Gramm;

void CreateGrammar(Gramm *gr, char *start, int numNotTerm, int numTerm, char *notTerm, char *term);
void AddRule(Gramm *gr, char* key, char* data);

bool IsTerm(char *s);
bool IsNotTerm(char *s);
bool IsRule(char *s);

bool isTerm(Gramm *gr, char c);
bool isNotTerm(Gramm *gr, char c);


char* First(Gramm *gr, char *X);
char* Follow(Gramm *gr);
void TableAnalysis(Gramm *gr);
void ShowTable(Gramm *gr);
char* FindTableRec(Gramm *gr, char X, char a);

bool CheckRule(Gramm *gr, char X, char *str);
char* Analysis(Gramm *gr, char *string);
void CreateLogFile(Gramm *gr, char* str, char*  res);
void Clear(Gramm * gr);