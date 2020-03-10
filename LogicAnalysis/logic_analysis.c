#include <stdio.h>

#define FILE_PATH "./config.txt"
#define MAX_YX_POINTS   100
#define MAX_YX_ITEMS    100
#define MAX_LINE_CHARS  100
#define MAX_LINE_OPERS  100
#define MAX_EXPR_SIZE   100

enum _tokens {
    NONE = 0,
    RET,
    ID,
    OTS,
};

/***************************************************
* @brief calculate the expression
* @param expr --expression created from the text
* @param exprPtr --pointer to the expression stack
* @param yx --yx status
* @return result of the expression
****************************************************/
char CalcExpr(int *expr, int exprPtr, char *yx) {
    char result[MAX_LINE_CHARS] = {0};
    int retPtr = -1;

    for (int i = 0; i <= exprPtr; i++) {
        if (expr[i] >= 128) {
            result[++retPtr] = yx[expr[i] - 128];
        } else if (expr[i] == '&') {
            int a = result[retPtr--];
            int b = result[retPtr];
            result[retPtr] = a && b;
        } else if (expr[i] == '|') {
            int a = result[retPtr--];
            int b = result[retPtr];
            result[retPtr] = a || b;
        } else if (expr[i] == '!') {
            int a = !result[retPtr];
            result[retPtr] = a;
        }
    }
    return result[0];
}

/***************************************************
* @brief parse the text to extract an expression
* @param l --a line of config file
* @param expr --stack to keep the expression
* @param exprPtr --pointer to the top of the stack
* @return index --index of the YXItem
****************************************************/
int ParseText(char *l, int *expr, int *exprPtr) {
    char oper[MAX_LINE_OPERS];
    int operPtr = -1;
    int ptr = -1;
    int index = 0;
    int tk = NONE;

    while ((*l) != ';') {
        if (((*l) >= 'a' && (*l) <= 'z') || ((*l) >= 'A' && (*l) <= 'Z')) {
            if (tk == NONE || tk == RET)
                tk = RET;
            else
                tk = ID;
            l++;
        } else if ((*l) >= '0' && (*l) <= '9') {
            int a = (*l++) - '0';
            a = a * 10 + (*l++) - '0';
            if (tk == RET) 
                index = a;
            else if (tk == ID)
                expr[++ptr] = a + 128;
        } else if ((*l) == '&') {
            if (operPtr != -1 && oper[operPtr] != '(') 
                expr[++ptr] = oper[operPtr--];
            oper[++operPtr] = '&';
            l++;
        } else if ((*l) == '|') {
            if (operPtr != -1 && oper[operPtr] != '(')
                expr[++ptr] = oper[operPtr--];
            oper[++operPtr] = '|';
            l++;
        } else if ((*l) == '!') {
            oper[++operPtr] = '!';
            l++;
        } else if ((*l) == '(') {
            oper[++operPtr] = '(';
            l++;
        } else if ((*l) == ')') {
            while (oper[operPtr] != '(') 
                expr[++ptr] = oper[operPtr--];
            l++;
        } else {
            tk = OTS;
            l++;
        }
    }
    /** pop all operators remain in the oper[] stack */
    while (operPtr != -1)
        expr[++ptr] = oper[operPtr--];
    *exprPtr = ptr;

    return index;
}

int main(void) {
    char YXItem[MAX_YX_ITEMS] = {-1};
    char YX[MAX_YX_POINTS] = {0, 1, 0, 1, 0};
    char line[MAX_LINE_CHARS];
    int expr[MAX_EXPR_SIZE] = {0};

    int lines = 0;
    int exprPtr = -1;
    int index = 0;

    FILE *f = fopen(FILE_PATH, "r");
    while (fgets(line, MAX_LINE_CHARS, f)) {
        printf("%s", line);
        index = ParseText(line, expr, &exprPtr);
        YXItem[index] = CalcExpr(expr, exprPtr, YX);
        lines++;
    }

    printf("\n");
    for (int i = 1; i <= lines; i++) 
        printf("%d\n", YXItem[i]);

    fclose(f);

    return 0;
}