#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char prods[20][100];
int n;

void processGrammar() {
    printf("\nProcessing grammar for left recursion...\n\n");
    
    char processed[20] = {0}; // Track which non-terminals are processed
    
    for (int i = 0; i < n; i++) {
        char non_terminal = prods[i][0];
        
        // Skip if already processed
        if (processed[non_terminal - 'A']) continue;
        
        char beta[50][50], alpha[50][50];
        int beta_count = 0, alpha_count = 0;
        
        // Find all productions for this non-terminal
        for (int j = 0; j < n; j++) {
            if (prods[j][0] == non_terminal) {
                char *curr_rhs = strstr(prods[j], "->") + 2;
                char temp[100];
                strcpy(temp, curr_rhs);
                
                char *token = strtok(temp, "|");
                while (token != NULL) {
                    // Remove leading spaces
                    while (*token == ' ') token++;
                    
                    if (token[0] == non_terminal) {
                        strcpy(alpha[alpha_count], token + 1);
                        alpha_count++;
                    } else {
                        strcpy(beta[beta_count], token);
                        beta_count++;
                    }
                    token = strtok(NULL, "|");
                }
            }
        }
        
        // Mark this non-terminal as processed
        processed[non_terminal - 'A'] = 1;
        
        // If left recursion found, print modified rules
        if (alpha_count > 0) {
            printf("%c -> ", non_terminal);
            for (int k = 0; k < beta_count; k++) {
                printf("%s%c'", beta[k], non_terminal);
                if (k < beta_count - 1) printf(" | ");
            }
            printf("\n");
            
            printf("%c' -> ", non_terminal);
            for (int k = 0; k < alpha_count; k++) {
                printf("%s%c' | ", alpha[k], non_terminal);
            }
            printf("#\n");
        } else {
            // No left recursion, print original
            for (int j = 0; j < n; j++) {
                if (prods[j][0] == non_terminal) {
                    printf("%s\n", prods[j]);
                }
            }
        }
    }
}

int main() {
    printf("Enter number of productions: ");
    scanf("%d", &n);
    getchar(); // consume newline
    
    printf("Enter productions (e.g., E->E+T|T):\n");
    for (int i = 0; i < n; i++) {
        printf("Production %d: ", i + 1);
        fgets(prods[i], sizeof(prods[i]), stdin);
        prods[i][strcspn(prods[i], "\n")] = 0; // remove newline
    }
    
    printf("\nOriginal Grammar:\n");
    for (int i = 0; i < n; i++) {
        printf("%s\n", prods[i]);
    }
    
    processGrammar();
    
    return 0;
}