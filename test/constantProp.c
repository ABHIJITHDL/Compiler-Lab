#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 100

struct three_address_code {
    char op[10];
    char arg1[10];
    char arg2[10];
    char result[10];
};


int is_number(char *s) {
    if (s[0] == '\0') {
        return 0;
    }
    for (int i = 0; s[i] != '\0'; i++) {
        if (!isdigit(s[i])) {
            return 0;
        }
    }
    return 1;
}

int main() {
    struct three_address_code code[MAX_SIZE];
    int n, i, j;

    printf("Enter the number of 3AC statements: ");
    scanf("%d", &n);

    printf("Enter the 3AC code (op arg1 arg2 result):\n");
    for (i = 0; i < n; i++) {
        scanf("%s %s %s %s", code[i].op, code[i].arg1, code[i].arg2, code[i].result);
    }

    for (i = 0; i < n; i++) {

        if (strcmp(code[i].op, "=") == 0 && is_number(code[i].arg1)) {
            for (j = i + 1; j < n; j++) {
                if (strcmp(code[j].arg1, code[i].result) == 0) {
                    strcpy(code[j].arg1, code[i].arg1);
                }
                if (strcmp(code[j].arg2, code[i].result) == 0) {
                    strcpy(code[j].arg2, code[i].arg1);
                }
            }
        }
    }

    printf("\nResult:\n");
    for (i = 0; i < n; i++) {
        printf("%s %s %s %s\n", code[i].op, code[i].arg1, code[i].arg2, code[i].result);
    }

    return 0;
}
