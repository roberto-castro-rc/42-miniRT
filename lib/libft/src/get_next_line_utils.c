/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaula-n <lpaula-n@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 03:49:23 by lpaula-n          #+#    #+#             */
/*   Updated: 2026/03/07 20:51:22 by lpaula-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*handle_read_error(char *storage)
{
	free(storage);
	return (NULL);
}

char	*init_storage(void)
{
	char	*storage;

	storage = ft_strdup("");
	if (!storage)
		return (NULL);
	return (storage);
}

char	*append_buffer(char *storage, char *buffer)
{
	char	*temp;

	temp = storage;
	storage = ft_strjoin(temp, buffer);
	free(temp);
	return (storage);
}
