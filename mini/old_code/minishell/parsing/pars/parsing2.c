#include "../../inc/minishell.h"

int	check_redirection(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if ((str[i] == '<') || (str[i] == '>') ||
			(str[i] == '<' && str[i + 1] == '<') ||
			(str[i] == '>' && str[i + 1] == '>'))
			return (1);
		i++;
	}
	return (0);
}

int	check_envirment(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

void	check_red_env(t_bash *bash)
{
	int	i;

	i = 0;
	while (bash->s_cmd[i])
	{
		bash->s_cmd[i]->check_red = check_redirection(bash->s_cmd[i]->command);
		bash->s_cmd[i]->check_env = check_envirment(bash->s_cmd[i]->command);
		// Expand environment variables in arguments
		expand_command_variables(bash, bash->s_cmd[i]);
		// Remove redirection tokens from arguments after redirection parsing
		if (bash->s_cmd[i]->check_red && bash->s_cmd[i]->s_red)
		{
			// Debug: print arguments before removal
			// printf("Before removal: ");
			// int k = 0;
			// while (bash->s_cmd[i]->arguments[k]) {
			// 	printf("'%s' ", bash->s_cmd[i]->arguments[k]);
			// 	k++;
			// }
			// printf("\n");
			
			remove_redirection_from_args(bash->s_cmd[i]);
			
			// Debug: print arguments after removal
			// printf("After removal: ");
			// k = 0;
			// while (bash->s_cmd[i]->arguments[k]) {
			// 	printf("'%s' ", bash->s_cmd[i]->arguments[k]);
			// 	k++;
			// }
			// printf("\n");
		}
		i++;
	}
}
