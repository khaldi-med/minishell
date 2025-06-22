#include "../../inc/minishell.h"

/**
 * Free memory allocated for a null-terminated array of strings
 */
void free_array(char **arr)
{
    int i;

    if (!arr)
        return;
    
    i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

/**
 * Free memory allocated for an array of redirection structures
 */
void free_redirection(t_red **red)
{
    int i;

    if (!red)
        return;
    
    i = 0;
    while (red[i])
    {
        if (red[i]->file)
            free(red[i]->file);
        free(red[i]);
        i++;
    }
    free(red);
}

