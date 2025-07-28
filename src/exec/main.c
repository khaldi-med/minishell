
#include "../../include/minishell.h"

void	ft_init_shell(t_shell *shell, char **envp)
{
	shell->env = ft_copy_env(envp);
	shell->exit_status = 0;
	shell->cmds = NULL;
}

static int	ft_handle_empty_input(char *input)
{
	if (!*input || *input == '\n')
	{
		free(input);
		return (1);
	}
	return (0);
}

static void	ft_process_input(t_shell *shell, char *input)
{
	t_token	*tokens;

	if (!ft_validate_input(shell, input))
		return ;
	tokens = ft_tokenize(input);
	if (tokens)
	{
		shell->cmds = ft_parse_tokens(tokens, shell);
		if (shell->exit_status != SYNTAX_ERROR && shell->cmds)
		{
			if (ft_preprocess_heredocs(shell->cmds) == 0)
				ft_exec_cmds(shell, shell->cmds);
		}
		ft_free_tokens(tokens);
		ft_free_cmds(shell->cmds);
		shell->cmds = NULL;
	}
}

void	ft_shell_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		ft_setup_signals();
		if (shell->exit_status == SYNTAX_ERROR)
			shell->exit_status = 0;
		input = readline("minishell$ ");
		if (!input)
		{
			ft_putendl_fd("exit", 1);
			break ;
		}
		if (*input && *input != '\n')
			add_history(input);
		if (ft_handle_empty_input(input))
			continue ;
		ft_process_input(shell, input);
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	ft_init_shell(&shell, envp);
	ft_shell_loop(&shell);
	ft_cleanup_shell(&shell);
	rl_clear_history();
	return (shell.exit_status);
}
