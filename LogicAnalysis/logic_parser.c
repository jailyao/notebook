#include <stdio.h>
#include <stdlib.h>

#define FILE_PATH "./config.txt"
#define MAX_YX_POINTS   100
#define MAX_YX_ITEMS    8
#define MAX_LINE_CHARS  100
#define MAX_STACK_SIZE  32

enum _tokens {
    NONE = 0,
    RET,
    ID,
    OTS,
};

/***************************************************
* @brief stack structure and functions
****************************************************/
typedef int EleType;
typedef int StkPtr;
typedef struct _Stack {
    EleType data[MAX_STACK_SIZE];
    StkPtr  top;
}*Stack;

Stack InitStack(Stack S) {
    Stack s = (Stack)malloc(sizeof(struct _Stack));
    s->top = -1;
    return s;
}

void Push(Stack S, EleType E) {
    S->data[++S->top] = E;
}

EleType Pop(Stack S) {
    EleType e = S->data[S->top--];
    return e;
}

EleType Retrieve(Stack S) {
    return S->data[S->top];
}

EleType GetEle(Stack S, StkPtr P) {
    return S->data[P];
}

StkPtr GetPtr(Stack S) {
    return S->top;
}

char IsEmpty(Stack S) {
    return (S->top == -1);
}

char IsFull(Stack S) {
    return (S->top == MAX_STACK_SIZE - 1);
}

/***************************************************
* @brief calculate the expression
* @param expr --expression created from the text
* @param exprPtr --pointer to the expression stack
* @param yx --yx status
* @return result of the expression
****************************************************/
char CalcExpr(Stack expr, char *yx) {
    struct _Stack calc;
    calc.top = -1;

    EleType exprPtr = GetPtr(expr);
    for (int i = 0; i <= exprPtr; i++) {
        EleType element = GetEle(expr, i);
        if (element >= 128) {
            Push(&calc, (yx[element - 128]));
        } else if (element == '&') {
            EleType a = Pop(&calc);
            EleType b = Pop(&calc);
            Push(&calc, (a && b));
        } else if (element == '|') {
            EleType a = Pop(&calc);
            EleType b = Pop(&calc);
            Push(&calc, (a || b));
        } else if (element == '!') {
            EleType a = Pop(&calc);
            Push(&calc, (!a));
        }
    }
    return GetEle(&calc, 0);
}

/***************************************************
* @brief parse the text to extract an expression
* @param expr --stack to keep the expression
* @param l --a line of config file
* @return index --index of the YXItem
****************************************************/
int ParseText(Stack expr, char *l) {
    struct _Stack oper;
    oper.top = -1;
    int tk = NONE;
    int index = 0;

    while ((*l) != ';') {
        if (((*l) >= 'a' && (*l) <= 'z') || ((*l) >= 'A' && (*l) <= 'Z')) {
            if (tk == NONE || tk == RET)
                tk = RET;
            else
                tk = ID;
            l++;
        } else if ((*l) >= '0' && (*l) <= '9') {
            int a = (*l++) - '0';
            while ((*l) >= '0' && (*l) <= '9') {
                a = a * 10 + (*l++) - '0';
            }
            
            if (tk == RET) 
                index = a;
            else if (tk == ID)
                Push(expr, (a + 128));
        } else if ((*l) == '&') {
            if (!IsEmpty(&oper) && Retrieve(&oper) != '(') 
                Push(expr, Pop(&oper));
            Push(&oper, '&');
            l++;
        } else if ((*l) == '|') {
            if (!IsEmpty(&oper) && Retrieve(&oper) != '(') 
                Push(expr, Pop(&oper));
            Push(&oper, '|');
            l++;
        } else if ((*l) == '!') {
            Push(&oper, '!');
            l++;
        } else if ((*l) == '(') {
            Push(&oper, '(');
            l++;
        } else if ((*l) == ')') {
            while (Retrieve(&oper) != '(') 
                Push(expr, Pop(&oper));
            l++;
        } else {
            tk = OTS;
            l++;
        }
    }
    /** pop all operators remain in the oper[] stack */
    while (!IsEmpty(&oper))
        Push(expr, Pop(&oper));

    return index;
}

int main(void) {
    char YXItem[MAX_YX_ITEMS] = {-1};
    char YX[MAX_YX_POINTS] = {0, 1, 0, 1, 0};
    char line[MAX_LINE_CHARS];
    Stack exprStack[MAX_YX_ITEMS];

    int lines = 0;
    int index = 0;

    FILE *f = fopen(FILE_PATH, "r");
    while (fgets(line, MAX_LINE_CHARS, f)) {
        printf("%s", line);
        Stack expr = InitStack(NULL);
        index = ParseText(expr, line);
        exprStack[index] = expr;
        lines++;
    }

    printf("\n");
    for (int i = 1; i <= lines; i++) {
        YXItem[i] = CalcExpr(exprStack[i], YX);
        printf("%d\n", YXItem[i]);
    }

    fclose(f);

    return 0;
}