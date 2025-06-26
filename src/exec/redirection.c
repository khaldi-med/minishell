#include "../../include/minishell.h"

static int	ft_handle_inp_red(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		ft_print_file_error(file);
		return (-1);
	}
	dup2(fd, 0);
	close(fd);
	return (0);
}

static int	ft_handle_outp_red(char *file, int flags)
{
	int	fd;

	fd = open(file, flags, 0644);
	if (fd < 0)
	{
		ft_print_file_error(file);
		return (-1);
	}
	dup2(fd, 1);
	close(fd);
	return (0);
}

int	ft_setup_redirections(t_redir *redirs)
{
	t_redir	*current;

	current = redirs;
	while (current)
	{
		if (current->type == TOKEN_REDIR_IN)
		{
			if (ft_handle_inp_red(current->file) == -1)
				return (-1);
		}
		else if (current->type == TOKEN_REDIR_OUT)
		{
			if (ft_handle_outp_red(current->file,
					O_WRONLY | O_CREAT | O_TRUNC) == -1)
				return (-1);
		}
		else if (current->type == TOKEN_REDIR_APPEND)
		{
			if (ft_handle_outp_red(current->file,
					O_WRONLY | O_CREAT | O_APPEND) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}
