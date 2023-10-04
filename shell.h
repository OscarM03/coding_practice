#ifndef SHELL_H
#define SHELL_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>


int printchar(char c);
int comp_str(char *str1, char *str2);
void prompt_print(void);
char *getenv_manual(const char *name);
char *get_input();
void strtok_manual(char *data, char *data_tokens[]);
void error_output(void);
void cmd_err_msg(void);
void cmd_execution(char *exec, char *const *exec_args, char *path);
void exec_cmd(char *input);
void display_env(void);
void exec_helper(char *exec, char * const *exec_args);
void handle_exit(char *input);
int custom_atoi(const char *str);
int str_n_cmp(const char *s1, const char *s2, size_t n);
void handle_semicolon(char *command);
void handle_and(char *command);
void handle_or(char *command);
char *custom_strstr(const char *haystack, const char *needle);
void process_input(char *input);

#endif
