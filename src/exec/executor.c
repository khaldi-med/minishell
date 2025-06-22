#include "../../include/minishell.h"

void	ft_exec_cmds(t_shell *shell, t_cmd *cmds)
{
	if (!cmds || !cmds->args)
		return ;
	ft_preprocess_heredocs(cmds);
	if (!cmds->next)
		ft_exec_sing_cmd(shell, cmds);
	else
		ft_exec_pipe(shell, cmds);
}
