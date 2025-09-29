#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LEN 100

typedef enum {
    KEYWORD, IDENTIFIER, NUMBER, OPERATOR, DELIMITER, STRING, UNKNOWN, END
} TokenType;

const char* keywords[] = {"int", "float", "double", "char", "if", "else", "while", "for", "return", "void", "main", NULL};
const char* operators[] = {"+", "-", "*", "/", "%", "=", "==", "!=", "<", ">", "<=", ">=", NULL};
const char* delimiters[] = {"(", ")", "{", "}", "[", "]", ";", ",", ".", NULL};

int is_keyword(const char* str) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) return 1;
    }
    return 0;
}

int is_operator(const char* str) {
    for (int i = 0; operators[i] != NULL; i++) {
        if (strcmp(str, operators[i]) == 0) return 1;
    }
    return 0;
}

int is_delimiter(char ch) {
    for (int i = 0; delimiters[i] != NULL; i++) {
        if (delimiters[i][0] == ch) return 1;
    }
    return 0;
}

TokenType get_next_token(FILE* fp, char* token) {
    int ch;
    int i = 0;
    
    while ((ch = fgetc(fp)) != EOF) {
        if (isspace(ch)) continue;
        
        if (isalpha(ch) || ch == '_') {
            token[i++] = ch;
            while ((ch = fgetc(fp)) != EOF && (isalnum(ch) || ch == '_')) {
                token[i++] = ch;
            }
            ungetc(ch, fp);
            token[i] = '\0';
            return is_keyword(token) ? KEYWORD : IDENTIFIER;
        }
        
        if (isdigit(ch) || ch == '.') {
            token[i++] = ch;
            int has_dot = (ch == '.');
            while ((ch = fgetc(fp)) != EOF && (isdigit(ch) || (ch == '.' && !has_dot))) {
                token[i++] = ch;
                if (ch == '.') has_dot = 1;
            }
            ungetc(ch, fp);
            token[i] = '\0';
            return NUMBER;
        }
        
        if (ch == '"') {
            token[i++] = ch;
            while ((ch = fgetc(fp)) != EOF && ch != '"') {
                token[i++] = ch;
            }
            if (ch == '"') token[i++] = ch;
            token[i] = '\0';
            return STRING;
        }
        
        // Check for two-character operators
        char next_ch = fgetc(fp);
        char temp[3] = {ch, next_ch, '\0'};
        if (is_operator(temp)) {
            strcpy(token, temp);
            return OPERATOR;
        }
        ungetc(next_ch, fp);
        
        // Single character
        token[0] = ch;
        token[1] = '\0';
        if (is_operator(token)) return OPERATOR;
        if (is_delimiter(ch)) return DELIMITER;
        return UNKNOWN;
    }
    return END;
}

int main() {
    FILE* fp = stdin; // Read from standard input
    char token[MAX_TOKEN_LEN];
    TokenType type;
    
    printf("Lexical Analyzer Output:\n");
    printf("========================\n");
    
    while ((type = get_next_token(fp, token)) != END) {
        switch (type) {
            case KEYWORD: printf("KEYWORD: %s\n", token); break;
            case IDENTIFIER: printf("IDENTIFIER: %s\n", token); break;
            case NUMBER: printf("NUMBER: %s\n", token); break;
            case OPERATOR: printf("OPERATOR: %s\n", token); break;
            case DELIMITER: printf("DELIMITER: %s\n", token); break;
            case STRING: printf("STRING: %s\n", token); break;
            default: printf("UNKNOWN: %s\n", token); break;
        }
    }
    
    return 0;
}
