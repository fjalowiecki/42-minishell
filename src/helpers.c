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