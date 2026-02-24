#include "minishell.h"

void	exit_free(t_data *data, int status)
{
	free_token(&data->tokens);
	free(data);
	exit(status);
}
