#include "shell.h"

/**
 * shell_no_interactive - UNIX command line interpreter.
 * Usage: simple_shell
 * @ac: number of arguments passed (non-interactive)
 * @av: double pointer with arguments passed (non-interactive)
 * Return: On success always 0
 */

int shell_no_interactive(int ac, char **av)
{
	char *line = NULL, **tokens = NULL, *path = NULL, *name_file = av[ac - 1];
	int (*funct)(), num_process = 0;

	while (1)
	{	signal(SIGINT, handle_sigint);
		line = prompt_no_interactive(name_file);
		if (line == NULL)
			return (0);
		tokens = tokenizer(line, " \n");
		if (tokens == NULL)
		{	free(line);
			continue; }
		funct = get_builtin(tokens[0]);
		if (funct != NULL)
		{	free(tokens);
			free(line);
			if (funct() == 1)
				exit(0);
			continue; }
		if (access(tokens[0], F_OK) != 0)
		{	path = find_path(tokens);
			if (path == NULL)
			{	free(path);
				num_process++;
				path_error(num_process, tokens, name_file);
				free_all(tokens);
				continue; }}
		else
			path = _strdup(tokens[0]);
		if (fork_process(path, tokens, environ) == 1)
		{	num_process++;
			permission_error(num_process, tokens, name_file);
			free(tokens);
			free(path);
			free(line);
			return (1); }
		free(line);
		num_process++; }
	return (0);
}
