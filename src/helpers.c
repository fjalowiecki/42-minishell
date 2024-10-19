// DELETE IN MAKEFILE AND IN HEADER

#include "minishell.h"

void ft_print_token_types(t_token *tokens)
{
    t_token *current = tokens;

	int i = 0;
    while (current)
    {
		printf("TOKEN NR. %d\n", i++);
        print_string_with_nulls(current->text);
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

void print_string_with_nulls(const char *str) 
{
	int length = ft_strlen(str) + 1;
    for (int i = 0; i < length; i++) {
        if (str[i] == '\0') {
            printf("\\0");  // Print "\0" for null characters
        } else {
            putchar(str[i]); // Print the character
        }
    }
}