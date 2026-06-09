# Parse-Based Calculator

Parse-Based Calculator is a command-line arithmetic calculator written in C++. It reads mathematical expressions from an interactive prompt, converts the input into tokens, builds an abstract syntax tree using a recursive-descent parser, and evaluates the tree to produce the final result.

The project demonstrates the core stages of expression evaluation: lexical analysis, parsing, AST construction, and recursive evaluation. It supports decimal numbers, constants, unary operators, functions, parentheses, and arithmetic operators while respecting standard operator precedence.

## Features

- Interactive calculator prompt with input history
- **Caret Syntax Error Highlighting:** Points out the exact character location of lexical and parsing errors in the CLI.
- **Unit & Currency Conversion Mode:** Built-in engine to convert between different units (mass, length, time, volume) and live currencies (retrieved from Frankfurter API using `libcurl`).
- Tokenizer for numbers, identifiers, operators, and parentheses
- Recursive-descent parser for arithmetic expressions
- Abstract syntax tree representation
- Evaluation through AST node operations
- Constants: `pi`, `e`, and `phi`
- Functions: `sin`, `cos`, `tan`, `sqrt`, `log`, and `ln`
- Operators: `+`, `-`, `*`, `/`, unary `+`, unary `-`, and `^`
- Makefile-based build process
- Dockerfile for containerized builds and execution

## Prerequisites

To build locally, make sure you have standard development tools installed along with the following libraries:
- `libreadline-dev` (for interactive history and readline support)
- `libcurl4-openssl-dev` (for fetching live exchange rates)

On Debian/Ubuntu:
```sh
sudo apt-get install g++ make libreadline-dev libcurl4-openssl-dev
```

On macOS (using Homebrew):
```sh
brew install readline curl
```

## Build And Run

```sh
make
./calculator
```

### Command-line Options

- **Token Inspection:** Show the tokens produced by the lexer before evaluation:
  ```sh
  ./calculator --tokens
  ```
- **AST Inspection:** Show the abstract syntax tree structure before evaluation:
  ```sh
  ./calculator --ast
  ```
- **Conversion Mode:** Enable unit and currency conversion mode:
  ```sh
  ./calculator --convert
  ```

### Conversion Mode Usage Examples

In conversion mode, you can perform direct math in different units:
```sh
convert > 10 mi to km
= 16.0934 km

convert > 5 kg + 500 g to lbs
= 12.12541 lbs

convert > 100 USD to EUR
= 92.45 EUR  (live rate dependent)
```

### Docker Support

Build the Docker image:
```sh
docker build -t calculator .
```

Run the container:
```sh
docker run -it calculator
```

To exit the calculator at any time, type:
```sh
exit
```
