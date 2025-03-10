# Minishell - 42 Warsaw

## Introduction

Welcome to the `minishell` project! This project is a simple implementation of a Unix shell, created as a part of the 42 Warsaw school curriculum. The goal of this project is to understand how a shell works, including command execution, signal handling, and process management.

## Table of Contents

1. [Description](#description)
2. [Features](#features)
3. [Installation](#installation)
4. [Usage](#usage)
6. [License](#license)

## Description

The Minishell project is a minimalistic shell that provides basic functionalities like executing commands, handling signals, and managing processes. The shell supports various built-in commands and can execute external programs. This project helps to grasp the underlying mechanisms of a shell and improves understanding of system programming in C.

### Features
- **Command Execution**: Execute external commands and programs.
- **Built-in Commands**: Support for essential built-in commands.
- **Signal Handling**: Proper handling of signals like `SIGINT` and `SIGQUIT`.
- **Process Management**: Manage foreground and background processes.
- **Error Handling**: Basic error handling for invalid commands and inputs.
- **Piping**: Support for command piping allowing the output of one command to be used as the input to another.
- **Input Parsing**: Robust parsing of user input to handle commands, arguments, and special characters.
- **Redirection**: Implement input and output redirection to/from files using `>`, `<`, and `>>`.

## Prerequisites

Before you can build and run this project, you need to have the following installed on your machine:
- **GCC**: GNU Compiler Collection for compiling C code.
- **Make**: Build automation tool.
- **Git**: Version control system to clone the repository.

## Installation

To install and run the Minishell, follow these steps:

1. Clone the repository:
    ```sh
    git clone https://github.com/fjalowiecki/42-minishell.git
    cd 42-minishell
    ```

2. Build the project:
    ```sh
    make
    ```

## Usage

After building the project, you can start the Minishell by running the following command:
```sh
./minishell
```
