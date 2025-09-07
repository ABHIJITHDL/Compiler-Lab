#include <stdio.h>
#include <string.h>

int n;
char prods[20][20];
char stack[50];
char input[50];

void print_step(const char* action) {
    printf("$%-20s\t%20s$\t%s\n", stack, input, action);
}

int main() {
    FILE *fp;
    int i, j, k, slen, rlen;
    char *rhs;

    fp = fopen("input.txt", "r");
    if (fp == NULL) {
        printf("Error: Cannot open input.txt\n");
        return 1;
    }
    fscanf(fp, "%d", &n);
    for (i = 0; i < n; i++) {
        fscanf(fp, "%s", prods[i]);
    }
    fscanf(fp, "%s", input);
    fclose(fp);

    printf("STACK\t\t\tINPUT\t\t\tACTION\n");
    stack[0] = '\0';
    print_step("INITIALIZE");

    while (input[0] != '\0') {
        slen = strlen(stack);
        stack[slen] = input[0];
        stack[slen + 1] = '\0';
        for (j = 0; input[j] != '\0'; j++) {
            input[j] = input[j + 1];
        }
        print_step("SHIFT");

        int r;
        do {
            r = 0;
            for (k = 0; k < n; k++) {
                rhs = &prods[k][2];
                rlen = strlen(rhs);
                slen = strlen(stack);

                if (slen >= rlen && strcmp(&stack[slen - rlen], rhs) == 0) {
                    stack[slen - rlen] = prods[k][0];
                    stack[slen - rlen + 1] = '\0';
                    
                    char act[50];
                    sprintf(act, "REDUCE %s", prods[k]);
                    print_step(act);
                    r = 1;
                    break;
                }
            }
        } while (r);
    }

    int r;
    do {
        r = 0;
        for (k = 0; k < n; k++) {
            rhs = &prods[k][2];
            rlen = strlen(rhs);
            slen = strlen(stack);

            if (slen >= rlen && strcmp(&stack[slen - rlen], rhs) == 0) {
                stack[slen - rlen] = prods[k][0];
                stack[slen - rlen + 1] = '\0';
                
                char act[50];
                sprintf(act, "REDUCE %s", prods[k]);
                print_step(act);
                r = 1;
                break;
            }
        }
    } while (r);

    if (strlen(stack) == 1 && stack[0] == prods[0][0]) {
        printf("\n--- STRING ACCEPTED ---\n");
    } else {
        printf("\n--- STRING REJECTED ---\n");
    }

    return 0;
}