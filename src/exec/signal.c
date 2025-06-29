#include "../../include/minishell.h"

/* Signal handler for SIGINT (Ctrl+C) */
void	ft_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = SIGINT;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	ft_setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = ft_signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	ft_reset_signal(void)
{
	g_signal = 0;
}
