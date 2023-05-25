#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "shell.h"

/**
 * main - shell loop
 * @argc: number of arguments
 * @argv: array of arguments
 *
 * Return: nothing
 */

int main(int argc, char *argv[])
{
	char *input = NULL;
	size_t bufsize = 0;
	char *commands[MAX_COMMANDS];
	int num_commands, from_pipe = 0;
	(void)argc;

	while (1 && !from_pipe)
	{
		/**
		 * cheks if data is from program or terminal
		 */
		if (isatty(STDIN_FILENO) == 0)
			from_pipe = 1;

		read_input(&input, &bufsize);
		perse_input(input, commands, &num_commands);

		if (num_commands > 0)
		{
			if (strcmp(commands[0], "exit") == 0)
			{
				free(input);
				exit(EXIT_SUCCESS);
			}


			char **command_args = get_command(input);

			execute_command(commands[0], commands, argv[0]);
			for (int i = 0; command_args[i] != NULL; i++)
			{
				free(command_args[i]);
			}
			free(command_args);
		}
		free(input);
		input = NULL;
	}
	return (0);
}
