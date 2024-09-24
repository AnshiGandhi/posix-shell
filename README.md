### AOS Assignment 2

# Author : Anshi Gandhi 2024201038
# Date : 14/08/24

This project is part of a college assignment focused on learning system calls in C++ and working of POSIX shell.

# Task
Implement a shell that supports a semi-colon separated list of commands. Use 'strtok' to tokenize
the command. Also, support '&' operator which lets a program run in the background after printing
the process id of the newly created process. Write this code in a modular fashion.

The goal of the assignment is to create a user-defined interactive shell program using
c/cpp that can create and manage new processes. The shell should be able to create
a process out of a system program like emacs, vi, or any user-defined executable.
Your shell can handle background and foreground processes and also input/output
redirections and pipes.

# File Structure
2024201038_Assignment2
├── README.md
├── makefile
└── Other .cpp and .o files

# Assumptions
 1. the folder from where the code is running is taken as the home directory and represented as ~ throughout the program
 2. A text file called history.txt is created to keep track of history

# Run the file
enter the directory where main.cpp is present
run the "make" command to link the files
run ./a.out
the program starts
now enter any command

# description
1. display requirement - a custom prompt that dynamically shows the current working directory.
2. cd.cpp - executes cd command using getcwd system call
3. echo.cpp - prints the input provided to echo command it is assumed that the input is always in "" and it will print the string as it is including ""
4. pwd.cpp - prints current working directory using getcwd()
5. ls.cpp - scandir() system call is used to print the directory contentes and stat is used to print it in long format (a and l flags are implemented)
6. fw_bw.cpp - foreground , background process are implemented using fork and wait system calls. if it is forground process, program will wait, else programm will not wait
7. pinfo.cpp - prints the process information of current shell and its child processes using fstream, open /proc directory and go to /proc/pid/stat tokenize it with " " and extract relevant information
8. search.cpp - search for a file in given directory and subdirectories.used dfs traversal to implement.
9. redirect.cpp - I/O redirection is implemented using dup2 to change the standard input and standard output
10. pipe.cpp - implemented using pipe() and dup2
11. history.cpp - using file operations, store the previous commands in history.txt
