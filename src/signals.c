/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammed                                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-05                              #+#    #+#             */
/*   Updated: 2025-05-05                             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Signal handler for SIGINT (Ctrl+C)
 * Displays a new prompt on a new line
 */
void	handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
 * Signal handler for SIGQUIT (Ctrl+\)
 * Ignores the signal in interactive mode
 */
void	handle_sigquit(int sig)
{
	(void)sig;
	/* Do nothing for Ctrl+\ in interactive mode */
}

/**
 * Sets up signal handlers for the shell
 */
void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);   /* Handle Ctrl+C */
	signal(SIGQUIT, handle_sigquit); /* Handle Ctrl+\ */
}

