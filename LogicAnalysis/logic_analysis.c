#include <stdio.h>

#define FILE_PATH "./example.txt"
#define MAX_LINE_CHARS  100
#define MAX_LINE_OPERS  100

char YXItems[10] = {-1};
char YXs[10] = {0};

enum _tokens {
    NONE = 0,
    RET,
    ID,
    OTS,
};

/***************************************************
* @brief calculate the expression
* @param expression --expression created from the text
* @param expPtr --pointer to the expression stack
@ @return result of the expression
****************************************************/
char calc_expression(char *expression, int expPtr) {
    char result[MAX_LINE_CHARS] = {0};
    int retPtr = -1;

    for (int i = 0; i <= expPtr; i++) {
        if (expression[i] < 2) {
            result[++retPtr] = expression[i];
        } else if (expression[i] == '&') {
            int a = result[retPtr--];
            int b = result[retPtr];
            result[retPtr] = a && b;
        } else if (expression[i] == '|') {
            int a = result[retPtr--];
            int b = result[retPtr];
            result[retPtr] = a || b;
        } else if (expression[i] == '!') {
            int a = !result[retPtr];
            result[retPtr] = a;
        }
    }
    return result[0];
}

/***************************************************
* @brief analysis the text to create an expression
* @param l --a line of config file
* @param yxArr --yx status array
* @param expression --stack to keep the expression
* @param expPtr --pointer to the top of the stack
@ @return index --index of the YXItems
****************************************************/
int line_analysis(char *l, char *yxArr, char *expression, int *expPtr) {
    char ops[MAX_LINE_OPERS];
    int opsPtr = -1;
    int ptr = -1;
    int index = 0;
    int tk = NONE;

    while ((*l) != '\n' && (*l) != '\0') {
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
                expression[++ptr] = yxArr[a] ? 1 : 0;
        } else if ((*l) == '&') {
            if (opsPtr != -1 && ops[opsPtr] != '(') 
                expression[++ptr] = ops[opsPtr--];
            ops[++opsPtr] = '&';
            l++;
        } else if ((*l) == '|') {
            if (opsPtr != -1 && ops[opsPtr] != '(')
                expression[++ptr] = ops[opsPtr--];
            ops[++opsPtr] = '|';
            l++;
        } else if ((*l) == '!') {
            ops[++opsPtr] = '!';
            l++;
        } else if ((*l) == '(') {
            ops[++opsPtr] = '(';
            l++;
        } else if ((*l) == ')') {
            while (ops[opsPtr] != '(') 
                expression[++ptr] = ops[opsPtr--];
            l++;
        } else {
            tk = OTS;
            l++;
        }
    }
    /** pop all operators remain in the ops[] stack */
    while (opsPtr != -1)
        expression[++ptr] = ops[opsPtr--];
    *expPtr = ptr;

    return index;
}

int main(void) {
    FILE *f = fopen(FILE_PATH, "r");
    char line[MAX_LINE_CHARS];
    int lines = 0;
    char expression[MAX_LINE_CHARS] = {0};
    int expPtr = -1;
    int index = 0;

    YXs[0] = 1;
    YXs[1] = 0;
    YXs[2] = 1;
    YXs[3] = 0;
    YXs[4] = 1;
    YXs[5] = 0;

    while (fgets(line, MAX_LINE_CHARS, f)) {
        printf("%s\n", line);
        index = line_analysis(line, YXs, expression, &expPtr);
        YXItems[index] = calc_expression(expression, expPtr);
        lines++;
    }

    for (int i = 1; i <= lines; i++) 
        printf("%d\n", YXItems[i]);

    fclose(f);

    return 0;
}