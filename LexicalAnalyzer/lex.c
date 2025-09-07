#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token types
typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_STRING,
    TOKEN_CHAR,
    TOKEN_PREPROCESSOR,
    TOKEN_HEADER_FILE,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char value[100];
    int line;
    int column;
} Token;

// Keywords
char* keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while",
    "main", "printf", "scanf", "malloc", "free", "NULL", NULL
};

// Global variables
char* input;
int pos = 0;
int line = 1;
int column = 1;

// Function prototypes
int isKeyword(char* str);
void skipWhitespace();
void skipComment();
Token getNextToken();
void printToken(Token token);
void printTokenToFile(Token token, FILE* outputFile);

// Check if string is a keyword
int isKeyword(char* str) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Skip whitespace characters
void skipWhitespace() {
    while (input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\n') {
        if (input[pos] == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
        pos++;
    }
}

// Skip single line (//) and multi-line (/* */) comments
void skipComment() {
    if (input[pos] == '/' && input[pos + 1] == '/') {
        // Single line comment
        pos += 2;
        column += 2;
        while (input[pos] != '\n' && input[pos] != '\0') {
            pos++;
            column++;
        }
    } else if (input[pos] == '/' && input[pos + 1] == '*') {
        // Multi-line comment
        pos += 2;
        column += 2;
        while (input[pos] != '\0') {
            if (input[pos] == '*' && input[pos + 1] == '/') {
                pos += 2;
                column += 2;
                break;
            }
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
    }
}

// Get next token from input
Token getNextToken() {
    Token token;
    token.line = line;
    token.column = column;
    
    // Skip whitespace and comments
    while (1) {
        skipWhitespace();
        if (input[pos] == '/' && (input[pos + 1] == '/' || input[pos + 1] == '*')) {
            skipComment();
        } else {
            break;
        }
    }
    
    // End of file
    if (input[pos] == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    // Preprocessor directives
    if (input[pos] == '#') {
        token.type = TOKEN_PREPROCESSOR;
        int i = 0;
        
        // Read the entire preprocessor line
        while (input[pos] != '\n' && input[pos] != '\0') {
            token.value[i++] = input[pos++];
            column++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    // Header files (angle brackets)
    if (input[pos] == '<') {
        // Check if this looks like a header file
        int tempPos = pos + 1;
        int isHeaderFile = 0;
        
        while (input[tempPos] != '>' && input[tempPos] != '\0' && input[tempPos] != '\n') {
            if (input[tempPos] == '.' && input[tempPos + 1] == 'h') {
                isHeaderFile = 1;
                break;
            }
            tempPos++;
        }
        
        if (isHeaderFile && input[tempPos + 2] == '>') {
            token.type = TOKEN_HEADER_FILE;
            int i = 0;
            
            while (input[pos] != '>' && input[pos] != '\0') {
                token.value[i++] = input[pos++];
                column++;
            }
            if (input[pos] == '>') {
                token.value[i++] = input[pos++];
                column++;
            }
            token.value[i] = '\0';
            return token;
        }
    }
    
    // String literals
    if (input[pos] == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        token.value[i++] = input[pos++];
        column++;
        
        while (input[pos] != '"' && input[pos] != '\0' && input[pos] != '\n') {
            if (input[pos] == '\\') {
                token.value[i++] = input[pos++];
                column++;
                if (input[pos] != '\0') {
                    token.value[i++] = input[pos++];
                    column++;
                }
            } else {
                token.value[i++] = input[pos++];
                column++;
            }
        }
        
        if (input[pos] == '"') {
            token.value[i++] = input[pos++];
            column++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    // Character literals
    if (input[pos] == '\'') {
        token.type = TOKEN_CHAR;
        int i = 0;
        token.value[i++] = input[pos++];
        column++;
        
        while (input[pos] != '\'' && input[pos] != '\0' && input[pos] != '\n') {
            if (input[pos] == '\\') {
                token.value[i++] = input[pos++];
                column++;
                if (input[pos] != '\0') {
                    token.value[i++] = input[pos++];
                    column++;
                }
            } else {
                token.value[i++] = input[pos++];
                column++;
            }
        }
        
        if (input[pos] == '\'') {
            token.value[i++] = input[pos++];
            column++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    // Numbers (including hex, octal, float)
    if (isdigit(input[pos]) || (input[pos] == '0' && (input[pos + 1] == 'x' || input[pos + 1] == 'X'))) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        
        // Hexadecimal
        if (input[pos] == '0' && (input[pos + 1] == 'x' || input[pos + 1] == 'X')) {
            token.value[i++] = input[pos++];
            token.value[i++] = input[pos++];
            column += 2;
            
            while (isxdigit(input[pos])) {
                token.value[i++] = input[pos++];
                column++;
            }
        } else {
            // Decimal or octal
            while (isdigit(input[pos]) || input[pos] == '.') {
                token.value[i++] = input[pos++];
                column++;
            }
            
            // Scientific notation
            if (input[pos] == 'e' || input[pos] == 'E') {
                token.value[i++] = input[pos++];
                column++;
                if (input[pos] == '+' || input[pos] == '-') {
                    token.value[i++] = input[pos++];
                    column++;
                }
                while (isdigit(input[pos])) {
                    token.value[i++] = input[pos++];
                    column++;
                }
            }
        }
        
        // Float/double suffixes
        if (input[pos] == 'f' || input[pos] == 'F' || input[pos] == 'l' || input[pos] == 'L') {
            token.value[i++] = input[pos++];
            column++;
        }
        
        token.value[i] = '\0';
        return token;
    }
    
    // Identifiers and keywords
    if (isalpha(input[pos]) || input[pos] == '_') {
        int i = 0;
        
        while (isalnum(input[pos]) || input[pos] == '_') {
            token.value[i++] = input[pos++];
            column++;
        }
        token.value[i] = '\0';
        
        if (isKeyword(token.value)) {
            token.type = TOKEN_KEYWORD;
        } else {
            token.type = TOKEN_IDENTIFIER;
        }
        return token;
    }
    
    // Operators and delimiters
    char ch = input[pos];
    token.value[0] = ch;
    token.value[1] = '\0';
    pos++;
    column++;
    
    // Multi-character operators
    if ((ch == '=' && input[pos] == '=') ||
        (ch == '!' && input[pos] == '=') ||
        (ch == '<' && input[pos] == '=') ||
        (ch == '>' && input[pos] == '=') ||
        (ch == '&' && input[pos] == '&') ||
        (ch == '|' && input[pos] == '|') ||
        (ch == '+' && input[pos] == '+') ||
        (ch == '-' && input[pos] == '-') ||
        (ch == '<' && input[pos] == '<') ||
        (ch == '>' && input[pos] == '>') ||
        (ch == '+' && input[pos] == '=') ||
        (ch == '-' && input[pos] == '=') ||
        (ch == '*' && input[pos] == '=') ||
        (ch == '/' && input[pos] == '=') ||
        (ch == '%' && input[pos] == '=') ||
        (ch == '-' && input[pos] == '>')) {
        token.value[1] = input[pos++];
        token.value[2] = '\0';
        column++;
    }
    
    // Classify as operator or delimiter
    if (strchr("+-*/%=!<>&|~^", ch)) {
        token.type = TOKEN_OPERATOR;
    } else if (strchr("(){}[];,.", ch)) {
        token.type = TOKEN_DELIMITER;
    } else {
        token.type = TOKEN_UNKNOWN;
    }
    
    return token;
}

// Print token information
void printToken(Token token) {
    char* typeNames[] = {
        "KEYWORD", "IDENTIFIER", "NUMBER", "OPERATOR", 
        "DELIMITER", "STRING", "CHAR", "PREPROCESSOR", 
        "HEADER_FILE", "EOF", "UNKNOWN"
    };
    
    printf("Line %d, Col %d: %-12s '%s'\n", 
           token.line, token.column, typeNames[token.type], token.value);
}

// Print token information to file
void printTokenToFile(Token token, FILE* outputFile) {
    char* typeNames[] = {
        "KEYWORD", "IDENTIFIER", "NUMBER", "OPERATOR", 
        "DELIMITER", "STRING", "CHAR", "PREPROCESSOR", 
        "HEADER_FILE", "EOF", "UNKNOWN"
    };
    
    if (token.type != TOKEN_EOF) {
        fprintf(outputFile, "<%s,%s,%d>\n", 
                token.value, typeNames[token.type], token.line);
    }
}

int main() {
    FILE* inputFile = fopen("input.txt", "r");
    FILE* outputFile = fopen("output.txt", "w");
    
    if (inputFile == NULL) {
        printf("Error: Cannot open input.txt file\n");
        return 1;
    }
    
    if (outputFile == NULL) {
        printf("Error: Cannot create output.txt file\n");
        fclose(inputFile);
        return 1;
    }
    
    // Read entire file into buffer
    char buffer[10000];
    buffer[0] = '\0';
    
    char line[500];
    while (fgets(line, sizeof(line), inputFile)) {
        strcat(buffer, line);
    }
    
    fclose(inputFile);
    input = buffer;
    
    printf("Lexical Analysis Complete!\n");
    printf("Input read from: input.txt\n");
    printf("Output written to: output.txt\n");
    
    fprintf(outputFile, "Tokens from Lexical Analysis:\n");
    fprintf(outputFile, "Format: <token,type,line_number>\n");
    fprintf(outputFile, "--------------------------------\n");
    
    Token token;
    do {
        token = getNextToken();
        printTokenToFile(token, outputFile);
    } while (token.type != TOKEN_EOF);
    
    fclose(outputFile);
    printf("Tokenization completed successfully!\n");
    
    return 0;
}
