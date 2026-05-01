# TaskLang++

> A Domain-Specific Language (DSL) for Task Scheduling and Automation

**Student:** IT24103532 — H.S.S Udayaratna  
**Course:** SE2052 — Programming Paradigms  
**Program:** BSc (Hons) in Computer Science — Year 2, Semester 2  
**Deadline:** 04-05-2026

---

## What is TaskLang++?

TaskLang++ is a mini programming language (compiler) that lets you define scheduled tasks using simple, readable syntax — instead of writing complex scripts or cron jobs.

You write a `.task` file like this:

```
TASK dailyReport {
  RUN "report.py";
  EVERY DAY AT 06:00;
}
```

And the compiler reads it, validates it, and prints the execution plan.

---

## How It Works

```
Your .task file
      │
      ▼
   Lexer (lexer.l)          — breaks text into tokens
      │
      ▼
   Parser (parser.y)        — checks grammar rules
      │
      ▼
   AST Builder (ast.c)      — builds a tree structure
      │
      ▼
   Semantic Validator        — checks for logic errors
   (semantic.c)
      │
      ▼
   Executor (executor.c)    — prints the output
```

---

## Project Structure

```
TaskLang++/
├── src/
│   ├── lexer.l         # Tokenizes input using Flex
│   ├── parser.y        # Grammar rules using Bison
│   ├── ast.h           # Task data structure definitions
│   ├── ast.c           # Task data structure implementation
│   ├── semantic.h      # Semantic checker header
│   ├── semantic.c      # Error detection logic
│   ├── executor.h      # Executor header
│   ├── executor.c      # Prints execution output
│   └── main.c          # Entry point
│
├── examples/
│   ├── simple_daily.task
│   ├── workflow.task
│   ├── conditions.task
│   ├── complex.task
│   └── error_examples/
│       ├── circular_deps.task
│       ├── duplicate_task.task
│       ├── invalid_syntax.task
│       ├── undefined_task.task
│       └── type_error.task
│
├── build.sh            # Linux/WSL build script
├── clean.sh            # Removes build artifacts
├── Makefile            # Alternative build method
├── REPORT.md           # Academic report
└── README.md           # This file
```

---

## Prerequisites

| Tool | Purpose |
|------|---------|
| GCC | C compiler |
| Flex | Lexer generator |
| Bison | Parser generator |

**Install on Linux/WSL (Ubuntu):**
```bash
sudo apt install -y gcc flex bison make
```

---

## Build & Run

**Step 1 — Build the compiler:**
```bash
./build.sh
```

**Step 2 — Run a task file:**
```bash
./tasklang examples/simple_daily.task
```

**Step 3 — Test error detection:**
```bash
./tasklang examples/error_examples/circular_deps.task
```

**Clean build artifacts:**
```bash
./clean.sh
```

---

## Language Syntax

### Task Definition
```
TASK taskName {
  RUN "script_or_command";
  scheduling_specification;
  [optional_condition];
}
```

### Scheduling Options

| Type | Syntax | Example |
|------|--------|---------|
| Daily | `EVERY DAY AT HH:MM;` | `EVERY DAY AT 06:00;` |
| Weekly | `EVERY WEEK ON DAY AT HH:MM;` | `EVERY WEEK ON SUNDAY AT 03:00;` |
| One-time | `AT HH:MM;` | `AT 15:30;` |
| After task | `AFTER taskName;` | `AFTER backupDB;` |

### Condition
```
IF success;
```
Makes the task run only if the dependency completed successfully.

### Days of the Week
`MONDAY` `TUESDAY` `WEDNESDAY` `THURSDAY` `FRIDAY` `SATURDAY` `SUNDAY`

---

## Examples

### 1. Simple Daily Task
```
TASK dailyReport {
  RUN "report.py";
  EVERY DAY AT 06:00;
}
```
**Output:**
```
Parsing TaskLang++ input...

--- EXECUTION START ---
Executing Task: dailyReport
  Script: "report.py"
  Schedule: EVERY DAY AT 06:00

--- EXECUTION COMPLETE ---
```

---

### 2. Workflow with Dependencies
```
TASK backupDB {
  RUN "backup.sh";
  EVERY DAY AT 02:00;
}

TASK sendReport {
  RUN "report.py";
  AFTER backupDB;
  IF success;
}

TASK cleanup {
  RUN "cleanup.sh";
  EVERY WEEK ON SUNDAY AT 03:00;
}
```
**Output:**
```
Parsing TaskLang++ input...

--- EXECUTION START ---
Executing Task: backupDB
  Script: "backup.sh"
  Schedule: EVERY DAY AT 02:00

Executing Task: sendReport
  Script: "report.py"
  Schedule:
  Depends on: backupDB
  Condition: success

Executing Task: cleanup
  Script: "cleanup.sh"
  Schedule: EVERY WEEK ON SUNDAY AT 03:00

--- EXECUTION COMPLETE ---
```

---

### 3. Chained Conditions
```
TASK preprocess {
  RUN "preprocess.py";
  AT 08:00;
}

TASK analyze {
  RUN "analyze.py";
  AFTER preprocess;
  IF success;
}

TASK alert {
  RUN "send_alert.sh";
  AFTER analyze;
  IF success;
}
```

---

## Error Detection

The compiler catches these errors automatically:

### Syntax Error — missing semicolon
```
TASK test {
  RUN "script.sh"       ← missing ;
  EVERY DAY AT 06:00;
}
```
```
Line 2: syntax error
```

---

### Circular Dependency
```
TASK taskA { RUN "a.sh"; AFTER taskC; }
TASK taskB { RUN "b.sh"; AFTER taskA; }
TASK taskC { RUN "c.sh"; AFTER taskB; }
```
```
Semantic error: Circular dependency detected: taskA -> taskB -> taskC -> taskA
```

---

### Undefined Task Reference
```
TASK test {
  RUN "test.sh";
  AFTER nonexistent;    ← task does not exist
}
```
```
Semantic error: Task 'nonexistent' is not defined
```

---

### Duplicate Task Name
```
TASK backup { RUN "backup.sh"; EVERY DAY AT 02:00; }
TASK backup { RUN "backup2.sh"; EVERY DAY AT 03:00; }
```
```
Semantic error: Task 'backup' is already defined
```

---

### Type Error — number instead of string
```
TASK test {
  RUN 123;              ← must be a string
  EVERY DAY AT 06:00;
}
```
```
Semantic error: RUN argument must be a string literal, not a number
```

---

## Grammar (EBNF)

```
program         = { task_definition } ;
task_definition = "TASK" identifier "{" task_body "}" ;
task_body       = run_statement schedule_spec [ condition_spec ] ;
run_statement   = "RUN" string_literal ";" ;
schedule_spec   = daily_schedule | weekly_schedule
                | timed_schedule | after_schedule ;
daily_schedule  = "EVERY" "DAY" "AT" time ";" ;
weekly_schedule = "EVERY" "WEEK" "ON" weekday "AT" time ";" ;
timed_schedule  = "AT" time ";" ;
after_schedule  = "AFTER" identifier ";" ;
condition_spec  = "IF" "success" ";" ;
time            = digit digit ":" digit digit ;
weekday         = "MONDAY" | "TUESDAY" | "WEDNESDAY" | "THURSDAY"
                | "FRIDAY" | "SATURDAY" | "SUNDAY" ;
identifier      = letter { letter | digit | "_" } ;
string_literal  = '"' { character } '"' ;
```

---

## Tokens

| Token | Lexeme | Description |
|-------|--------|-------------|
| `TASK` | `TASK` | Begins a task definition |
| `RUN` | `RUN` | Specifies command to execute |
| `EVERY` | `EVERY` | Recurring schedule keyword |
| `DAY` | `DAY` | Used with EVERY for daily |
| `WEEK` | `WEEK` | Used with EVERY for weekly |
| `ON` | `ON` | Day specifier for weekly |
| `AT` | `AT` | Time specification |
| `AFTER` | `AFTER` | Dependency specification |
| `IF` | `IF` | Conditional execution |
| `SUCCESS` | `success` | Condition keyword |
| `IDENTIFIER` | `[a-zA-Z][a-zA-Z0-9_]*` | Task name |
| `STRING_LITERAL` | `"..."` | Script path |
| `TIME` | `HH:MM` | Time value |
| `WEEKDAY` | `MONDAY`..`SUNDAY` | Day of week |
| `LBRACE` | `{` | Block open |
| `RBRACE` | `}` | Block close |
| `SEMICOLON` | `;` | Statement end |

---

## Semantic Validation

| Check | Description |
|-------|-------------|
| Duplicate names | No two tasks can have the same name |
| Undefined references | AFTER must reference an existing task |
| Circular dependencies | Detected using depth-first search (DFS) |
| Type checking | RUN argument must be a string, not a number |

---

## Architecture Overview

| Component | File | Role |
|-----------|------|------|
| Lexer | `src/lexer.l` | Tokenizes input text |
| Parser | `src/parser.y` | Validates grammar, builds AST |
| AST | `src/ast.c/h` | Stores task data structures |
| Semantic | `src/semantic.c/h` | Checks logic errors |
| Executor | `src/executor.c/h` | Prints execution output |
| Entry point | `src/main.c` | Ties all phases together |

---

## Limitations

- No actual script execution — simulation only
- Only `success` condition supported
- No task parameters or environment variables
- No retry or timeout support

---

*Created by H.S.S Udayaratna — IT24103532 — SLIIT — 2026*