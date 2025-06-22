#include "../inc/minishell.h"

/* Global variables */
static int	g_in_cmd_execution = 0;
extern int	g_exit_status;

/*
 * Handle Ctrl+C (SIGINT) signal
 * Different behavior for parent shell and child processes
 */
void	handel_sigint(int sig)
{
	(void)sig;
	if (!g_in_cmd_execution)
	{
		/* In parent shell (prompt) */
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_exit_status = 130;
	}
	else
		/* In child process or during command execution */
		write(STDOUT_FILENO, "\n", 1);
}

/*
 * Handle Ctrl+\ (SIGQUIT) signal
 * Ignored in parent shell, shows message in child processes
 */
void	handel_sigquit(int sig)
{
	(void)sig;
	if (g_in_cmd_execution)
	{
		write(STDOUT_FILENO, "Quit (core dumped)\n", 18);
		g_exit_status = 131;
	}
	else
	{
		/* In parent shell, ignore Ctrl+\ */
		rl_on_new_line();
		rl_redisplay();
	}
}

/*
 * Set up signal handlers for parent shell
 * Handles Ctrl+C, ignores Ctrl+\ and Ctrl+Z
 */
void	setup_parent_signals(void)
{
	g_in_cmd_execution = 0;
	signal(SIGINT, handel_sigint);	/* Handle Ctrl+C */
	signal(SIGQUIT, SIG_IGN);		/* Ignore Ctrl+\ */
	signal(SIGTSTP, SIG_IGN);		/* Ignore Ctrl+Z */
}

/*
 * Set up signal handlers for child processes
 * Restores default behavior for all signals
 */
void	setup_child_signals(void)
{
	g_in_cmd_execution = 1;
	signal(SIGINT, SIG_DFL);		/* Default Ctrl+C behavior */
	signal(SIGQUIT, SIG_DFL);		/* Default Ctrl+\ behavior */
	signal(SIGTSTP, SIG_DFL);		/* Default Ctrl+Z behavior */
}

/*
 * Initialize signal handling for the shell
 * Sets up parent shell signal handlers
 */
void	active_signal(void)
{
	setup_parent_signals();
}
