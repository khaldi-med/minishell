#include "../../include/minishell.h"

int		g_signal;
void	ft_init_shell(t_shell *shell, char **envp)
{
	shell->env = ft_copy_env(envp);
	shell->exit_status = 0;
	shell->cmds = NULL;
}

void	ft_shell_loop(t_shell *shell)
{
	char	*input;
	t_token	*tokens;

	while (1)
	{
		ft_setup_signals();
		if (shell->exit_status == MS_SYNTAX_ERROR)
			shell->exit_status = 0;
		input = readline("minishell$ ");
		if (!input)
		{
			ft_putendl_fd("exit", 1);
			break ;
		}
		if (*input && *input != '\n')
			add_history(input);
		if (!*input || *input == '\n')
		{
			free(input);
			continue ;
		}
		tokens = ft_tokenize(input);
		if (tokens)
		{
			shell->cmds = ft_parse_tokens(tokens, shell);
			if (shell->exit_status != MS_SYNTAX_ERROR && shell->cmds
				&& shell->cmds->args && shell->cmds->args[0])
				ft_exec_cmds(shell, shell->cmds);
			ft_free_tokens(tokens);
			ft_free_cmds(shell->cmds);
			shell->cmds = NULL;
		}
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
