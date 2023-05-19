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
		perror("(getline)");
		exit(EXIT_FAILURE);
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
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		perror(argv);
		exit(EXIT_FAILURE);
	} else if (pid == 0)
	{
		execve(command, arguments, NULL);
		perror(argv);
		exit(EXIT_FAILURE);
	}
	else
		wait(NULL);
}
