#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stddef.h>
#include "shell.h"
/**
 * read_input - reads the input from the command line
 * @buffer: the buffer where to store the user input
 * @n: pointer to the buffer size
 *
 * Return: nothing
 */

void read_input(char **buffer, size_t *n)
{
	char *prompt = " ($) ";
	ssize_t bytesRead;

	if (buffer == NULL || n == NULL)
		return;

	if (isatty(STDIN_FILENO) == 1)
		write(STDOUT_FILENO, prompt, 5);

	bytesRead = getline(buffer, n, stdin);

	if (bytesRead == -1)
	{
		if (feof(stdin))
		{
			printf("\n");
			exit(EXIT_SUCCESS);
		}
		else
		{
			perror("(getline)");
			exit(EXIT_FAILURE);
		}
	}

	if ((*buffer)[bytesRead - 1] == '\n')
		(*buffer)[bytesRead - 1] = '\0';
}


/**
 * perse_input - separate the input string to tokens
 * @input: the input from the command line
 * @commands: a table of strings where cammands will be stored
 * @num_commands: the size of the commands array
 *
 * Return: nothing
 */

void perse_input(char *input, char **commands, int *num_commands)
{
	char *token;
	int count = 0;

	token = strtok(input, " \t\n");
	while (token != NULL)
	{
		commands[count] = token;
		token = strtok(NULL, " \t\n");
		count++;
	}
	commands[count] = NULL;
	*num_commands = count;
}
/**
 * execute_command - execute command passed to the function
 * @command: the command to be executed
 * @arguments: the arguments of the command
 * @argv: my executable name
 *
 * Return: nothing
 */

void execute_command(char *command, char **arguments, char *argv)
{
	pid_t pid = fork();

	if (pid == -1)
	{
		perror(argv);
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		if (strchr(command, '/') != NULL)
			execvp(command, arguments);
		else
		{
			char *path = getenv("PATH");
			char *path_copy = strdup(path);
			char *token = strtok(path_copy, ":");

			while (token != NULL)
			{
				char command_path[100];

				snprintf(command_path, sizeof(command_path), "%s/%s", token, command);
				execvp(command_path, arguments);
				token = strtok(NULL, ":");
			}
			free(path_copy);
		}
		perror(command);
		exit(EXIT_FAILURE);
	}
	else
	{
		int status;

		waitpid(pid, &status, 0);
	}
}

/**
 * get_command - retrieves the command and arguments from the input
 * @input: the input from the command line
 *
 * Return: an array of strings with the command and arguments
 */
char **get_command(char *input)
{
	char **command_args = malloc((MAX_COMMANDS + 1) * sizeof(char *));

	if (command_args == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	int i = 0;
	char *token = strtok(input, " \t\n");

	while (token != NULL && i < MAX_COMMANDS)
	{
		command_args[i] = strdup(token);
		token = strtok(NULL, " \t\n");
		i++;
	}
	command_args[i] = NULL;

	return (command_args);
}
