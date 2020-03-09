#include <stdio.h>

#define FILE_PATH "./example.txt"
#define MAX_LINE_CHARS  100
#define MAX_LINE_OPERS  10

char line[MAX_LINE_CHARS];

char ops[MAX_LINE_OPERS];
int opsPtr = -1;
char expression[MAX_LINE_CHARS] = {0};
int expPtr = -1;

int YXItems[10] = {-1};
int YXs[10] = {0};

enum _tokens {
    NONE = 0,
    RET,
    ID,
    OP,
};

void line_analysis(char *l) {
    char op = '\0';
    int tk = NONE;
    int idx_ret = 0;
    int idx_id = 0;
    int top = -1;

    while ((*l) != '\n') {
        if (((*l) >= 'a' && (*l) <= 'z') || ((*l) >= 'A' && (*l) <= 'Z')) {
            if (tk == NONE)
                tk = RET;
            else 
                tk = ID;
            l++;
        } else if ((*l) >= '0' && (*l) <= '9') {
            int a = (*l) - '0';
            l++;
            int b = (*l) - '0';
            if (tk == RET) 
                idx_ret = a * 10 + b;
            else if (tk == ID) {
                idx_id = a * 10 + b;
                expression[++expPtr] = YXs[idx_id] ? 1 : 0;
            }
        } else if ((*l) == '&') {
            if (tk == op) 
                l++;
            else {
                tk = op;
                l++;
                ops[++opsPtr] = '&';
            }
        } else if ((*l) == '|') {
            if (tk == op) 
                l++;
            else {
                tk = op;
                l++;
                ops[++opsPtr] = '|';
            }
        } else if ((*l) == '(') {
            l++;
            ops[++opsPtr] = '(';
        } else if ((*l) == ')') {
            l++;
            while (ops[opsPtr] != '(') 
                expression[++expPtr] = ops[opsPtr--];
            opsPtr--;
        }
    }
}

int main(void) {
    FILE *f = fopen(FILE_PATH, "r");
    
    while (fgets(line, MAX_LINE_CHARS, f)) {
        
    }

    return 0;
}