#include <stdio.h>
#include <string.h>
#include <ctype.h>

char stack[50];
int top = -1;
int temp_count = 0;

void push(char c) {
    stack[++top] = c;
}

char pop() {
    return stack[top--];
}

int precedence(char op) {
    switch(op) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        case '^': return 3;
        default: return 0;
    }
}

int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

void generateCode(char *input) {
    char *assignment_var = NULL;
    char *expression = input;
    
    // Check if it's an assignment statement
    char *equals_pos = strchr(input, '=');
    if (equals_pos != NULL) {
        *equals_pos = '\0'; // Split the string
        assignment_var = input;
        expression = equals_pos + 1;
        printf("Assignment variable: %s\n", assignment_var);
    }
    
    char postfix[100];
    int j = 0;
    
    printf("Intermediate Code Generation:\n");
    printf("============================\n");
    
    // Convert to postfix
    for (int i = 0; expression[i]; i++) {
        char c = expression[i];
        
        if (isalnum(c)) {
            postfix[j++] = c;
        }
        else if (c == '(') {
            push(c);
        }
        else if (c == ')') {
            while (top != -1 && stack[top] != '(') {
                postfix[j++] = pop();
            }
            pop();
        }
        else if (isOperator(c)) {
            while (top != -1 && precedence(stack[top]) >= precedence(c)) {
                postfix[j++] = pop();
            }
            push(c);
        }
    }
    
    while (top != -1) {
        postfix[j++] = pop();
    }
    postfix[j] = '\0';
    
    printf("Postfix: %s\n\n", postfix);
    printf("Three Address Code:\n");
    printf("-------------------\n");
    
    // Generate three-address code from postfix
    char operand_stack[50][10];
    int op_top = -1;
    
    for (int i = 0; postfix[i]; i++) {
        char c = postfix[i];
        
        if (isalnum(c)) {
            char temp[10];
            temp[0] = c;
            temp[1] = '\0';
            strcpy(operand_stack[++op_top], temp);
        }
        else if (isOperator(c)) {
            char op2[10], op1[10], temp_var[10];
            strcpy(op2, operand_stack[op_top--]);
            strcpy(op1, operand_stack[op_top--]);
            
            sprintf(temp_var, "t%d", temp_count++);
            printf("%s = %s %c %s\n", temp_var, op1, c, op2);
            
            strcpy(operand_stack[++op_top], temp_var);
        }
    }
    
    // Handle final assignment
    if (assignment_var != NULL) {
        printf("%s = %s\n", assignment_var, operand_stack[op_top]);
    } else {
        printf("\nResult stored in: %s\n", operand_stack[op_top]);
    }
}

int main() {
    char input[100];
    
    printf("Enter expression (e.g., a=b+c or b+c): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    
    printf("\nInput: %s\n", input);
    generateCode(input);
    
    return 0;
}
