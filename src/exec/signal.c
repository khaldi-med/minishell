#include "../../include/minishell.h"

void	ft_signal_handler(int sig)
{
	g_signal = sig;
	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", 1);
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

void	ft_handle_sigint(void)
{
	if (g_signal == SIGINT)
		g_signal = 0;
}

void	ft_handle_sigquit(void)
{
	signal(SIGQUIT, SIG_IGN);
}
