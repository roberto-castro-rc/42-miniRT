/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaula-n <lpaula-n@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 17:08:32 by lpaula-n          #+#    #+#             */
/*   Updated: 2026/03/07 17:08:33 by lpaula-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	error_exit(char *message)
{
	write(2, "Error\n", 6);
	if (message)
	{
		write(2, message, ft_strlen(message));
		write(2, "\n", 1);
	}
}

int	check_file_extension(char *filename, char *ext)
{
	int	len;
	int	ext_len;

	if (!filename || !ext)
		return (0);
	len = ft_strlen(filename);
	ext_len = ft_strlen(ext);
	if (len < ext_len)
		return (0);
	return (ft_strncmp(filename + len - ext_len, ext, ext_len) == 0);
}
