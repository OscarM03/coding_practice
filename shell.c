#include "shell.h"

/**
 * custom_strstr - Custom implementation of strstr
 * @haystack: The string to be searched
 * @needle: The substring to search for
 * Return: Pointer
 */
char *custom_strstr(const char *haystack, const char *needle)
{
	while (*haystack)
	{
		const char *h = haystack;
		const char *n = needle;

		while (*h && *n && (*h == *n))
		{
			h++;
			n++;
		}

		if (!*n)
		{
			return ((char *)haystack);
		}

		haystack++;
	}

	return (NULL);
}

/**
 * process_input - Process the input command
 * @input: The input command string
 */
void process_input(char *input)
{

	if (comp_str(input, "env\n") == 1)
	{
		display_env();
	}

	handle_exit(input);


	if (custom_strstr(input, ";") != NULL)
	{
		handle_semicolon(input);
	}
	else if (custom_strstr(input, "&&") != NULL)
	{
		handle_and(input);
	}
	else if (custom_strstr(input, "||") != NULL)
	{
		handle_or(input);
	}
	else
	{
		exec_cmd(input);
	}
}

/**
  * printchar - prints a character
  * @c: Takes a parameter c, which is a char
  * Return: number of bytes/characters written
  */
int printchar(char c)
{
	return (write(1, &c, 1));
}

/**
  * prompt_print - print the prompt
  */
void prompt_print(void)
{
	int i;
	char prompt[] = "codec$ ";

	for (i = 0; prompt[i] != '\0'; i++)
	{
		printchar(prompt[i]);
	}
}

/**
 * get_last_status - Retrieves the last recorded status of a command
 *
 * This function returns the value of the last recorded status of a command.
 *
 * Return: The last status code
 */
int get_last_status(void)
{
	int last_status = 0;

	return (last_status);
}

/**
 * handle_semicolon - Function to handle semicolon operator
 * @command: The input command string
 */
void handle_semicolon(char *command)
{
	char *cmd = strtok(command, ";");

	while (cmd != NULL)
	{
		exec_cmd(cmd);
		cmd = strtok(NULL, ";");
	}
}

/**
 * handle_and - Function to handle and operator
 * @command: The input command string
 */
void handle_and(char *command)
{
	char *cmd = strtok(command, "&&");

	while (cmd != NULL)
	{
		exec_cmd(cmd);
		if (WEXITSTATUS(get_last_status()) != 0)
		{
			break;
		}
		cmd = strtok(NULL, "&&");
	}
}

/**
 * handle_or - Function to handle or operator
 * @command: The input command string
 */
void handle_or(char *command)
{
	char *cmd = strtok(command, "||");

	while (cmd != NULL)
	{
		exec_cmd(cmd);
		cmd = strtok(NULL, "||");
	}
}

/**
 * strtok_manual - Tokenize a string manually.
 *
 * This function tokenizes a string based on space (' ') and newline ('\n')
 * characters. It splits the input string into tokens and stores them in the
 * data_tokens array. The tokens are separated by space or newline characters.
 * Lines starting with '#' are ignored, and tokens within them are not stored.
 *
 * @data: The string to be tokenized.
 * @data_tokens: An array to store the resulting tokens.
 */
void strtok_manual(char *data, char *data_tokens[])
{
	int i = 0;
	char *start = data;
	int hash = 0;

	while (*data != '\0')
	{
		if (*data == ' ' || *data == '\n')
		{
			if (data != start && !hash)
			{
				*data = '\0';
				data_tokens[i++] = start;
			}
			start = data + 1;
		}
		else if (*data == '#')
		{
			hash = 1;
		}
		data++;
	}

	if (data != start && !hash)
	{
		data_tokens[i++] = start;
	}

	data_tokens[i] = NULL;
}
/**
  * cmd_err_msg - print the error message
  * Return void
  */
void cmd_err_msg(void)
{
	int i;
	char error_display[] = "No such file or directory\n";

	for (i = 0; error_display[i] != '\0'; i++)
	{
		printchar(error_display[i]);
	}
}

/**
 * cmd_execution - execute the command
 * @exec: pointer
 * @exec_args: This is a pointer to a constant pointer
 * @path: pointer to a char array that contains list of directories
 */
void cmd_execution(char *exec, char * const *exec_args, char *path)
{
	char path_buf[256], *path_tk = path, *path_st = path_tk;
	char *cur_dir, *path_buf_ptr, *exec_ptr;
	int exec_stat;

	while (*path_tk != '\0')
	{
		if (*path_tk == ':')
		{
			cur_dir = path_st;
			path_buf_ptr = path_buf;

			while (cur_dir != path_tk)
				*path_buf_ptr++ = *cur_dir++;
			*path_buf_ptr++ = '/';
			exec_ptr = exec;

			while (*exec_ptr != '\0')
				*path_buf_ptr++ = *exec_ptr++;
			*path_buf_ptr = '\0';

			exec_stat = execve(path_buf, exec_args, NULL);

			if (exec_stat == -1 && errno != ENOENT)
			{
				perror("execve");
				error_output();
				exit(EXIT_FAILURE);
			}
			path_st = path_tk + 1;
		}
		path_tk++;
	}

	cmd_err_msg();
}

/**
 * exec_helper - execute the command
 * @exec: pointer
 * @exec_args: This is a pointer to a constant pointer
 */
void exec_helper(char *exec, char * const *exec_args)
{
	int exec_stat = execve(exec, exec_args, NULL);

	if (exec_stat == -1)
	{
		perror("execve");
		exit(EXIT_FAILURE);
	}
}

/**
 * comp_str - Compare two strings for equality, It compares two strings,
 *  to check whether they're equal or not.
 *
 * This function compares two strings character by character and returns 1
 * if they are equal and 0 if they are not.
 *
 * @str1: The first string to compare.
 * @str2: The second string to compare.
 *
 * Return: 1 if the strings are equal, 0 otherwise.
 */
int comp_str(char *str1, char *str2)
{
	int i = 0;

	/* Continue comparing characters until */
	/* a null terminator is reached in both strings */
	while (str1[i] != '\0' || str2[i] != '\0')
	{
		/* If the characters at the current position are not equal, return 0 */
		if (str1[i] != str2[i])
		{
			return (0);
		}
		i++;
	}

	/* If the loop completes, it means the strings are equal, so return 1 */
	return (1);
}

/**
 * display_env - Display environment variables.
 *
 * This function prints the current envirnment variable to the standard output.
 * It iterates through the envirnment variable array and prints each variables
 * name and value.
 * Return void
 */
void display_env(void)
{
	char **env = __environ;
	char *temp;

	while (*env != NULL)
	{
		temp = *env;

		while (*temp != '\0')
		{
			printchar(*temp);
			temp++;
		}
		printchar('\n');
		env++;
	}
}

/**
 * error_output - Display an error message.
 *
 * This function is responsible for displaying an error message indicating
 * that a command execution has failed.
 * Return void
 */
void error_output(void)
{
	int k;
	char error_display[] = "Error executing command.\n";

	/* Iterate through the error message string and print each character */
	for (k = 0; error_display[k] != '\0'; k++)
	{
		/* Print the current character from the error message */
		printchar(error_display[k]);
	}
}

/**
 * exec_cmd - Execute a command.
 * @input: The input command as a string.
 */
void exec_cmd(char *input)
{
	pid_t small_pid;
	char *tokens[256];
	char *executable;
	char *const *args;
	char *path_env;
	int status;

	strtok_manual(input, tokens);

	small_pid = fork();

	if (small_pid < 0)
	{
		perror("fork");
		exit(1);
	}
	if (small_pid == 0)
	{
		executable = tokens[0];
		args = tokens;

		path_env = getenv_manual("PATH");

		if (executable[0] == '/')
		{
			exec_helper(executable, args);
		}
		else
		{
			cmd_execution(executable, args, path_env);
		}
	}
	else
	{
		/* Wait for the child process to complete */
		waitpid(small_pid, &status, 0);
	}
}

/**
 * getenv_manual - Get the value of an environment variable.
 *
 * This function searches for an environment variable by name and returns its
 * value. If the variable is not found, it returns NULL.
 *
 * @name: The name of the environment variable to retrieve.
 * Return: A pointer to the value of the environment
 * variable, or NULL if not found.
 */
char *getenv_manual(const char *name)
{
	char *env_var;
	size_t name_length = 0;
	const char *name_pointer = name;
	char **environment;
	size_t index;

	/* Calculate the length of the variable name */
	while (*name_pointer != '\0')
	{
		name_length++;
		name_pointer++;
	}

	/* Iterate through the environment variables */
	for (environment = __environ; *environment != NULL; environment++)
	{
		env_var = *environment;

		/* Compare the variable name with the name parameter */
		for (index = 0; index < name_length; index++)
		{
			if (env_var[index] != name[index])
			{
				break;
			}
		}

		/* If a match is found, return the value of the variable */
		if (index == name_length && env_var[name_length] == '=')
		{
			return (&env_var[name_length + 1]);
		}
	}

	/* Variable not found, return NULL */
	return (NULL);
}

/**
  * get_input - get input from the user
  * Return: user's input
  */
char *get_input(void)
{
	FILE *ptr = stdin;
	size_t bufsize = 256;
	char *input = NULL;
	ssize_t chars_noted;

	chars_noted = getline(&input, &bufsize, ptr);

	if (chars_noted == -1)
	{
		if (feof(ptr))
		{
			free(input);
			exit(EXIT_SUCCESS);
		}
		else
		{
			perror("getline");
			free(input);
			exit(EXIT_FAILURE);
		}
	}
	return (input);
}

/**
 * handle_exit - Handles the 'exit' command in the shell.
 * @input: The user input.
 */
void handle_exit(char *input)
{
	int exit_code;

	if (comp_str(input, "exit\n") == 1)
	{
		free(input);
		exit(0);
	}
	else if (str_n_cmp(input, "exit ", 5) == 0)
	{
		exit_code = custom_atoi(input + 5);
		free(input);
		exit(exit_code);
	}
}

/**
 * custom_atoi - Converts a string to an integer
 * @str: Input string
 *
 * Return: The converted integer value
 */
int custom_atoi(const char *str)
{
	int result = 0;
	int sign = 1;
	int i = 0;

	while (str[i] == ' ')
	{
		i++;
	}

	if (str[i] == '-' || str[i] == '+')
	{
		sign = (str[i++] == '-') ? -1 : 1;
	}

	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i++] - '0');
	}

	return (sign * result);
}

/**
 * str_n_cmp - Compares at most n characters of two strings
 * @s1: First string
 * @s2: Second string
 * @n: Maximum number of characters to compare
 * Return: int
 */
int str_n_cmp(const char *s1, const char *s2, size_t n)
{
	while (n--)
	{
		if (*s1 != *s2)
			return ((unsigned char)*s1 - (unsigned char)*s2);

		if (*s1 == '\0')
			return (0);

		s1++;
		s2++;
	}

	return (0);
}
/**
 * main - Entry point of the program.
 * This function serves as the entry point for a simple command-line shell.
 * It repeatedly prompts the user for input, processes the input, and executes
 * commands until the user enters "exit" to exit the shell.
 * Return: Always returns 0 to indicate successful execution.
 */
int main(void)
{
	char *input;

	if (isatty(STDIN_FILENO))
	{
		while (1)
		{
			prompt_print();
			input = get_input();
			process_input(input);
			free(input);
		}
	}
	else
	{
		while (1)
		{
			input = get_input();
			process_input(input);
			free(input);
		}
	}

	return (0);
}
