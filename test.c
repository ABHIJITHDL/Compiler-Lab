#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

int rfind(const char *s, const char *p) {
    int n = strlen(s), m = strlen(p);
    for (int i = n - m; i >= 0; --i) {
        if (strncmp(s + i, p, m) == 0) return i;
    }
    return -1;
}

int reduce(char *stk) {
    int pos;
    pos = rfind(stk, "(E)");
    if (pos != -1) {
        stk[pos] = 'F';
        stk[pos+1] = '\0';
        strcat(stk, stk + pos + 4);
        printf("REDUCE by F -> (E)\n");
        return 1;
    }
    pos = rfind(stk, "i");
    if (pos != -1) {
        stk[pos] = 'F';
        printf("REDUCE by F -> id\n");
        return 1;
    }
    pos = rfind(stk, "T*F");
    if (pos != -1) {
        stk[pos] = 'T';
        stk[pos+1] = '\0';
        strcat(stk, stk + pos + 3);
        printf("REDUCE by T -> T*F\n");
        return 1;
    }
    pos = rfind(stk, "E+T");
    if (pos != -1) {
        stk[pos] = 'E';
        stk[pos+1] = '\0';
        strcat(stk, stk + pos + 3);
        printf("REDUCE by E -> E+T\n");
        return 1;
    }
    pos = rfind(stk, "F");
    if (pos != -1) {
        stk[pos] = 'T';
        printf("REDUCE by T -> F\n");
        return 1;
    }
    pos = rfind(stk, "T");
    if (pos != -1) {
        stk[pos] = 'E';
        printf("REDUCE by E -> T\n");
        return 1;
    }
    return 0;
}

int main() {
    char in[MAX], buf[MAX*2];
    char stk[MAX*2] = "";
    printf("Enter input string (use 'i' for id):\n");
    if (!fgets(in, sizeof(in), stdin)) return 0;
    in[strcspn(in, "\n")] = 0;
    buf[0] = '\0';
    for (int i = 0; in[i]; ) {
        if (in[i] == 'i' && in[i+1] == 'd') { strcat(buf, "i"); i += 2; }
        else {
            char t[2] = { in[i], '\0' };
            strcat(buf, t);
            i++;
        }
    }
    if (strlen(buf) == 0) strcpy(buf, in);
    if (buf[strlen(buf)-1] != '$') strcat(buf, "$");
    printf("Stack\t\tInput\t\tAction\n");
    printf("-----\t\t-----\t\t------\n");
    int ip = 0;
    while (1) {
        printf("%-16s%-16s", stk, buf + ip);
        if (buf[ip] == '\0') { printf("ERROR\n"); break; }
        if (buf[ip] == '$') {
            int chg = 1;
            while (chg) chg = reduce(stk);
            if (strcmp(stk, "E") == 0) {
                printf("ACCEPT\n");
            } else {
                printf("REJECT\n");
            }
            break;
        }
        char tk[3] = {0};
        tk[0] = buf[ip];
        tk[1] = '\0';
        int slen = strlen(stk);
        stk[slen] = tk[0];
        stk[slen+1] = '\0';
        ip++;
        printf("SHIFT '%c'\n", tk[0]);
        int r = 1;
        while (r) {
            r = reduce(stk);
        }
    }
    return 0;
}
