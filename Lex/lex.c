#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
    
// Function to check if a given string is a C keyword
int isKeyword(char buffer[]) {
    const char* keywords[] = {
        "auto", "break", "case", "char", "const", "continue", "default",
        "do", "double", "else", "enum", "extern", "float", "for", "goto",
        "if", "int", "long", "register", "return", "short", "signed",
        "sizeof", "static", "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while"
    };
    int numKeywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < numKeywords; ++i) {
        if (strcmp(keywords[i], buffer) == 0) {
            return 1; // It is a keyword
        }
    }
    return 0; // It is not a keyword
}

int main() {
    FILE *inputFile, *outputFile;
    char c, next_c;
    char buffer[1000];
    int buffer_index = 0;

    // --- Open files and handle potential errors ---
    inputFile = fopen("input.c", "r");
    if (inputFile == NULL) {
        printf("Error: Cannot open input.c\n");
        return 1;
    }

    outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        printf("Error: Cannot open output.txt for writing.\n");
        fclose(inputFile);
        return 1;
    }

    // --- Main loop to read characters from the input file ---
    while ((c = fgetc(inputFile)) != EOF) {

        // --- 1. Handle Comments ---
        if (c == '/') {
            next_c = fgetc(inputFile);
            if (next_c == '/') { // Single-line comment
                while ((c = fgetc(inputFile)) != '\n' && c != EOF); // Skip to end of line
                continue;
            } else if (next_c == '*') { // Multi-line comment
                while (1) {
                    c = fgetc(inputFile);
                    if (c == EOF) break;
                    if (c == '*') {
                        next_c = fgetc(inputFile);
                        if (next_c == '/') break;
                        ungetc(next_c, inputFile); // Put back the character if not end of comment
                    }
                }
                continue;
            } else {
                // It's a division operator, not a comment
                fprintf(outputFile, "<%c, OPERATOR>\n", c);
                ungetc(next_c, inputFile); // Put back the character we peeked at
            }
            continue;
        }

        // --- 2. Handle Preprocessor Directives ---
        if (c == '#') {
            buffer_index = 0;
            buffer[buffer_index++] = c;
            while ((c = fgetc(inputFile)) != '\n' && c != EOF) {
                buffer[buffer_index++] = c;
            }
            buffer[buffer_index] = '\0';
            fprintf(outputFile, "<%s, PREPROCESSOR_DIRECTIVE>\n", buffer);
            continue;
        }

        // --- 3. Handle String Literals ---
        if (c == '"') {
            buffer_index = 0;
            buffer[buffer_index++] = c;
            while ((c = fgetc(inputFile)) != '"' && c != EOF) {
                // Handle escaped quotes
                if (c == '\\') {
                    buffer[buffer_index++] = c;
                    c = fgetc(inputFile);
                }
                buffer[buffer_index++] = c;
            }
            buffer[buffer_index++] = '"';
            buffer[buffer_index] = '\0';
            fprintf(outputFile, "<%s, STRING_LITERAL>\n", buffer);
            continue;
        }
        
        // --- 3.5. Handle Character Literals ---
        if (c == '\'') {
            buffer_index = 0;
            buffer[buffer_index++] = c;
            while ((c = fgetc(inputFile)) != '\'' && c != EOF) {
                // Handle escaped single quotes
                if (c == '\\') {
                    buffer[buffer_index++] = c;
                    c = fgetc(inputFile);
                }
                buffer[buffer_index++] = c;
            }
            buffer[buffer_index++] = '\'';
            buffer[buffer_index] = '\0';
            fprintf(outputFile, "<%s, CHARACTER_LITERAL>\n", buffer);
            continue;
        }


        // --- 4. Handle Operators (including multi-character ones) ---
        if (strchr("=<>!+-&|", c)) {
            buffer_index = 0;
            buffer[buffer_index++] = c;
            next_c = fgetc(inputFile);
            // Check for two-character operators like ==, !=, <=, >=, &&, ||, ++, --
            if ((c == '=' && next_c == '=') || (c == '!' && next_c == '=') ||
                (c == '<' && next_c == '=') || (c == '>' && next_c == '=') ||
                (c == '&' && next_c == '&') || (c == '|' && next_c == '|') ||
                (c == '+' && next_c == '+') || (c == '-' && next_c == '-')) {
                buffer[buffer_index++] = next_c;
            } else {
                ungetc(next_c, inputFile); // Not a two-character operator, put char back
            }
            buffer[buffer_index] = '\0';
            fprintf(outputFile, "<%s, OPERATOR>\n", buffer);
            continue;
        }
        
        // Handle other single-character operators
        if (strchr("*/%", c)) {
             fprintf(outputFile, "<%c, OPERATOR>\n", c);
             continue;
        }


        // --- 5. Handle Identifiers and Keywords ---
        if (isalpha(c) || c == '_') {
            buffer_index = 0;
            buffer[buffer_index++] = c;
            while (isalnum(c = fgetc(inputFile)) || c == '_') {
                buffer[buffer_index++] = c;
            }
            buffer[buffer_index] = '\0';
            ungetc(c, inputFile); // Put back the non-alphanumeric character

            if (isKeyword(buffer)) {
                fprintf(outputFile, "<%s, KEYWORD>\n", buffer);
            } else {
                fprintf(outputFile, "<%s, IDENTIFIER>\n", buffer);
            }
            continue;
        }

        // --- 6. Handle Numbers ---
        if (isdigit(c)) {
            buffer_index = 0;
            buffer[buffer_index++] = c;
            while (isdigit(c = fgetc(inputFile))) {
                buffer[buffer_index++] = c;
            }
            buffer[buffer_index] = '\0';
            ungetc(c, inputFile); // Put back the non-digit character
            fprintf(outputFile, "<%s, NUMBER>\n", buffer);
            continue;
        }

        // --- 7. Handle Special Symbols ---
        if (strchr(";,(){}[]", c)) {
            fprintf(outputFile, "<%c, SPECIAL_SYMBOL>\n", c);
            continue;
        }

        // --- 8. Ignore Whitespace ---
        if (isspace(c)) {
            continue;
        }
    }

    // --- Clean up ---
    fclose(inputFile);
    fclose(outputFile);

    printf("Lexical analysis complete. Output written to output.txt\n");

    return 0;
}
