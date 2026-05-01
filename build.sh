#!/bin/bash
set -e

TARGET="tasklang"

echo "=================================================="
echo "   Building TaskLang++ Compiler"
echo "=================================================="

echo ""
echo "Step 1: Generating parser (Bison)..."
bison -d -o src/y.tab.c src/parser.y
if [ $? -ne 0 ]; then
    echo "ERROR: Bison parser generation failed!"
    exit 1
fi
echo "   Generated: src/y.tab.c and src/y.tab.h"

echo ""
echo "Step 2: Generating lexer (Flex)..."
flex -o src/lex.yy.c src/lexer.l
if [ $? -ne 0 ]; then
    echo "ERROR: Flex lexer generation failed!"
    exit 1
fi
echo "   Generated: src/lex.yy.c"

echo ""
echo "Step 3: Compiling all source files (GCC)..."
gcc -Wall -Wextra -w -g \
    -o $TARGET \
    src/lex.yy.c \
    src/y.tab.c \
    src/ast.c \
    src/semantic.c \
    src/executor.c \
    src/main.c \
    -lm

if [ $? -ne 0 ]; then
    echo "ERROR: Compilation failed!"
    exit 1
fi

echo "   Compiled: $TARGET"
echo ""
echo "=================================================="
echo "   Build Successful!"
echo "   Run: ./$TARGET <file.task>"
echo "=================================================="
