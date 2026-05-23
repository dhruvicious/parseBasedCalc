# Parse-Based Calculator

Parse-Based Calculator is a command-line arithmetic calculator written in C++. It reads mathematical expressions from an interactive prompt, converts the input into tokens, builds an abstract syntax tree using a recursive-descent parser, and evaluates the tree to produce the final result.

The project demonstrates the core stages of expression evaluation: lexical analysis, parsing, AST construction, and recursive evaluation. It supports decimal numbers, constants, unary operators, functions, parentheses, and arithmetic operators while respecting standard operator precedence.

## Features

- Interactive calculator prompt with input history
- Tokenizer for numbers, identifiers, operators, and parentheses
- Recursive-descent parser for arithmetic expressions
- Abstract syntax tree representation
- Evaluation through AST node operations
- Constants: `pi`, `e`, and `phi`
- Functions: `sin`, `cos`, `tan`, `sqrt`, `log`, and `ln`
- Operators: `+`, `-`, `*`, `/`, unary `+`, unary `-`, and `^`
- Makefile-based build process
- Dockerfile for containerized builds and execution

## Build And Run

```sh
make
./calculator
```

Or use Docker:

Build the Docker image:

```sh
docker build -t calculator .
```

Run the container:

```sh
docker run -it calculator
```

To exit the calculator, type:

```sh
exit
```
