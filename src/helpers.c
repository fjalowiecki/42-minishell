// DELETE IN MAKEFILE AND IN HEADER

#include "minishell.h"

void ft_print_token_types(t_token *tokens)
{
    t_token *current = tokens;

    while (current)
    {
        printf("%d ", current->type);
        current = current->next;    
	}
    printf("\n");
}

void ft_print_commands(t_cmd *cmds)
{
	while(cmds)
	{
		if (cmds->cmd)
			ft_print_split(cmds->cmd);
		if (cmds->infile)
			printf("%s\n",cmds->infile);
		if (cmds->outfile)
			printf("%s\n",cmds->outfile);
		printf("Append:%d\n",cmds->append);
		printf("Redirectoin error:%d\n",cmds->redir_error);
		printf("Here_doc:%d\n",cmds->here_doc);
		cmds = cmds->next;	
	}
}

void ft_print_split(char **str)
{
	int i;

	i = 0;
	if(!str)
		return;
	while(str[i])
		printf("split args[%d]%s\n", i,str[i++]);
}