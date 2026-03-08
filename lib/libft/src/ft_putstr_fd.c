/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaula-n <lpaula-n@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 12:29:03 by lpaula-n          #+#    #+#             */
/*   Updated: 2026/03/07 20:51:22 by lpaula-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	while (*s)
	{
		ft_putchar_fd(*s, fd);
		s++;
	}
}

int	ft_printf_putstr_fd(char *s, int fd)
{
	int	count;
	int	i;

	if (!s)
		return (ft_printf_putstr_fd("(null)", fd));
	count = 0;
	i = 0;
	while (s[i])
	{
		count += ft_printf_putchar_fd(s[i], fd);
		i++;
	}
	return (count);
}
