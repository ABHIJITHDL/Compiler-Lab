#!/bin/bash

echo "Compiling all programs..."

# Compile q1 to q4
for i in 1 2 3 4; do
    echo "Compiling q$i..."
    lex q$i.l && gcc lex.yy.c -o q$i -ll
done

# Compile q5
echo "Compiling q5..."
bison -d q5.y && lex q5.l && gcc q5.tab.c lex.yy.c -o q5 -ll

# Compile q6
echo "Compiling q6..."
bison -d q6.y && lex q6.l && gcc q6.tab.c lex.yy.c -o q6 -ll

# Compile q7
echo "Compiling q7..."
bison -d q7.y && lex q7.l && gcc q7.tab.c lex.yy.c -o q7 -ll

# Compile lexer
echo "Compiling lexer..."
lex lex.l && gcc lex.yy.c -o lexer -ll

echo "Compilation done."
echo

echo "Running Question 1:"
./q1 < input.txt
echo

echo "Running Question 2:"
./q2 < input.txt
echo

echo "Running Question 3:"
./q3 < input.txt
echo

echo "Running Question 4:"
./q4 < input.txt
echo

echo "Running Question 5:"
echo "1+2*3" | ./q5
echo

echo "Running Question 6:"
echo "abc123" | ./q6
echo

echo "Running Question 7:"
echo -e "1+2\n3*4\n5" | ./q7
echo

echo "Running lexer (from lex.l):"
./lexer