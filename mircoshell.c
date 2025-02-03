#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>

#define STDIN_FILENO 1

void	ft_putstr_error(char *str, char *arg)
{
	while (*str)
		write(2, str++, 1);
	if (arg)
		while (*arg)
			write(2, arg++, 1);
	write(2, "\n", 1);
}

void	ft_execute(char **argv, int i, int tmp_fd, char **env)
{
	
}

int main(int argc, char **argv, char **env)
{
	int i = -1;
	int fd[2]; // for pipe
	int tmp_fd; // for semicolon

	while (++i < argc)
	{
		while (argv[i] && strcmp(argv[i], ";") && strcmp(argv[i], "|"))
			i++;
		if (strcmp(argv[1], "cd") == 0)
		{
			if (i != 3)
				ft_putstr_error("error: cd: bad arguments", NULL);
			else if (chdir(argv[2]) != 0)
				ft_putstr_error("error: cd: cannot change directory to ", argv[2]);
		}
		else if (i != 1 && (argv[i] == NULL || strcmp(argv[i], ";") == 0))
		{
			if (fork() == 0)
				ft_execute(argv, i, tmp_fd, env);
			else
			{
				close(tmp_fd);
				while (waitpid(-1, NULL, WUNTRACED) != -1)
					;
				tmp_fd = dup(STDIN_FILENO);
			}
		}
	}
	return (0);
}
