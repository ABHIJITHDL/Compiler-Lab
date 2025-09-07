#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Global variables
int n; // Number of productions
char prods[20][20]; // Productions

// Function to add a character to a set
void addToSet(char set[], char c) {
    int i;
    for (i = 0; set[i] != '\0'; i++) {
        if (set[i] == c) return;
    }
    set[i] = c;
    set[i + 1] = '\0';
}

// Function to find the FIRST set of a non-terminal
void findFirst(char c, char first[]) {
    int i, j;
    
    // If c is a terminal, its FIRST is itself
    if (!isupper(c)) {
        addToSet(first, c);
        return;
    }

    // Find productions starting with c
    for (i = 0; i < n; i++) {
        if (prods[i][0] == c) {
            // If the production is of the form c -> #
            if (prods[i][2] == '#') {
                addToSet(first, '#');
            } 
            else {
                // Process each symbol in the production body
                for (j = 2; prods[i][j] != '\0'; j++) {
                    char tempFirst[20] = "";
                    findFirst(prods[i][j], tempFirst);
                    
                    // Add all symbols from tempFirst except #
                    for (int k = 0; tempFirst[k] != '\0'; k++) {
                        if (tempFirst[k] != '#') {
                            addToSet(first, tempFirst[k]);
                        }
                    }
                    
                    // If # is not in FIRST of current symbol, break
                    if (strchr(tempFirst, '#') == NULL) {
                        break;
                    }
                    
                    // If we've reached the last symbol and it derives #
                    if (prods[i][j + 1] == '\0') {
                        addToSet(first, '#');
                    }
                }
            }
        }
    }
}

// Function to find the FOLLOW set of a non-terminal
void findFollow(char c, char follow[]) {
    int i, j;
    
    // Rule 1: For the start symbol, add '$' to its FOLLOW set
    if (prods[0][0] == c) {
        addToSet(follow, '$');
    }

    // Iterate through all productions
    for (i = 0; i < n; i++) {
        for (j = 2; prods[i][j] != '\0'; j++) {
            // Find the non-terminal 'c' in the production body
            if (prods[i][j] == c) {
                // Rule 2: If A -> aBb, then FOLLOW(B) contains FIRST(b)
                if (prods[i][j + 1] != '\0') {
                    int k = j + 1;
                    int allEpsilon = 1;
                    
                    // Process all symbols after c
                    while (prods[i][k] != '\0' && allEpsilon) {
                        char tempFirst[20] = "";
                        findFirst(prods[i][k], tempFirst);
                        
                        // Add all non-epsilon symbols to FOLLOW(c)
                        for (int l = 0; tempFirst[l] != '\0'; l++) {
                            if (tempFirst[l] != '#') {
                                addToSet(follow, tempFirst[l]);
                            }
                        }
                        
                        // If current symbol doesn't derive epsilon, stop
                        if (strchr(tempFirst, '#') == NULL) {
                            allEpsilon = 0;
                        }
                        k++;
                    }
                    
                    // If all symbols after c derive epsilon, add FOLLOW(A)
                    if (allEpsilon && prods[i][0] != c) {
                        char tempFollow[20] = "";
                        findFollow(prods[i][0], tempFollow);
                        for (int l = 0; tempFollow[l] != '\0'; l++) {
                            addToSet(follow, tempFollow[l]);
                        }
                    }
                }
                // Rule 3: If A -> aB, then FOLLOW(B) contains FOLLOW(A)
                else if (prods[i][0] != c) {
                    char tempFollow[20] = "";
                    findFollow(prods[i][0], tempFollow);
                    for (int l = 0; tempFollow[l] != '\0'; l++) {
                        addToSet(follow, tempFollow[l]);
                    }
                }
            }
        }
    }
}

int main() {
    int i;
    char c;
    FILE *inputFile, *outputFile;

    // Read grammar from input file
    inputFile = fopen("input.txt", "r");
    if (inputFile == NULL) {
        printf("Error opening input.txt\n");
        return 1;
    }
    fscanf(inputFile, "%d", &n);
    for (i = 0; i < n; i++) {
        fscanf(inputFile, "%s", prods[i]);
    }
    fclose(inputFile);

    outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        printf("Error opening output.txt\n");
        return 1;
    }

    // Find and print FIRST sets
    fprintf(outputFile, "FIRST Sets:\n");
    for (i = 0; i < n; i++) {
        c = prods[i][0];
        // Check if we have already computed FIRST for this non-terminal
        int found = 0;
        for(int k = 0; k < i; k++) {
            if(prods[k][0] == c) {
                found = 1;
                break;
            }
        }
        if(found) continue;

        char first[20] = "";
        findFirst(c, first);
        fprintf(outputFile, "FIRST(%c) = { ", c);
        for(int j = 0; first[j] != '\0'; j++) {
            fprintf(outputFile, "%c", first[j]);
            if(first[j+1] != '\0') fprintf(outputFile, " ");
        }
        fprintf(outputFile, " }\n");
    }
    
    fprintf(outputFile, "\n");

    // Find and print FOLLOW sets
    fprintf(outputFile, "FOLLOW Sets:\n");
    for (i = 0; i < n; i++) {
        c = prods[i][0];
        // Check if we have already computed FOLLOW for this non-terminal
        int found = 0;
        for(int k = 0; k < i; k++) {
            if(prods[k][0] == c) {
                found = 1;
                break;
            }
        }
        if(found) continue;

        char follow[20] = "";
        findFollow(c, follow);
        fprintf(outputFile, "FOLLOW(%c) = { ", c);
        for(int j = 0; follow[j] != '\0'; j++) {
            fprintf(outputFile, "%c", follow[j]);
            if(follow[j+1] != '\0') fprintf(outputFile, " ");
        }
        fprintf(outputFile, " }\n");
    }

    fclose(outputFile);
    printf("FIRST and FOLLOW sets have been written to output.txt\n");

    return 0;
}
