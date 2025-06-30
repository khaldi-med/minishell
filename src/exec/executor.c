#include "../../include/minishell.h"

void	ft_exec_cmds(t_shell *shell, t_cmd *cmds)
{
	if (!cmds)
		return ;
	// Remove redundant preprocessing since it's already done in main
	if (!cmds->next)
		ft_exec_sing_cmd(shell, cmds);
	else
		ft_exec_pipe(shell, cmds);
}
