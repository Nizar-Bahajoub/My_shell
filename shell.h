#ifndef _SHELL_H
#define _SHELL_H

#define MAX_COMMANDS 100

void read_input(char **buffer, size_t *n);
void perse_input(char *input, char **commands, int *num_commands);
void execute_command(char *command, char **arguments, char *argv);
char **get_command(char *input);

#endif /* _SHELL_H */
