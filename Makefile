CC      = gcc
CFLAGS  = -Wall -Wextra -w -g
TARGET  = tasklang

SRCS    = src/lex.yy.c src/y.tab.c src/ast.c src/semantic.c src/executor.c src/main.c

all: src/lex.yy.c src/y.tab.c
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) -lm
	@echo "Build successful! Run with: ./$(TARGET) <file.task>"

src/lex.yy.c: src/lexer.l src/y.tab.h
	@echo "Running Flex on lexer.l..."
	flex -o src/lex.yy.c src/lexer.l

src/y.tab.c src/y.tab.h: src/parser.y
	@echo "Running Bison on parser.y..."
	bison -d -o src/y.tab.c src/parser.y

clean:
	@echo "Cleaning build artifacts..."
	rm -f $(TARGET)
	rm -f src/lex.yy.c
	rm -f src/y.tab.c
	rm -f src/y.tab.h
	rm -f src/parser.output
	@echo "Clean complete."

test: all
	@echo ""
	@echo "=== TEST 1: simple_daily.task ==="
	./$(TARGET) examples/simple_daily.task
	@echo ""
	@echo "=== TEST 2: workflow.task ==="
	./$(TARGET) examples/workflow.task
	@echo ""
	@echo "=== TEST 3: conditions.task ==="
	./$(TARGET) examples/conditions.task
	@echo ""
	@echo "=== TEST 4: complex.task ==="
	./$(TARGET) examples/complex.task

test-errors: all
	@echo ""
	@echo "=== ERROR TEST 1: invalid_syntax.task ==="
	-./$(TARGET) examples/error_examples/invalid_syntax.task
	@echo ""
	@echo "=== ERROR TEST 2: duplicate_task.task ==="
	-./$(TARGET) examples/error_examples/duplicate_task.task
	@echo ""
	@echo "=== ERROR TEST 3: undefined_task.task ==="
	-./$(TARGET) examples/error_examples/undefined_task.task
	@echo ""
	@echo "=== ERROR TEST 4: circular_deps.task ==="
	-./$(TARGET) examples/error_examples/circular_deps.task
	@echo ""
	@echo "=== ERROR TEST 5: type_error.task ==="
	-./$(TARGET) examples/error_examples/type_error.task

.PHONY: all clean test test-errors
