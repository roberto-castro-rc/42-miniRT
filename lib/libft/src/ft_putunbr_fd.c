/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putunbr_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaula-n <lpaula-n@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 09:04:45 by lpaula-n          #+#    #+#             */
/*   Updated: 2026/03/07 20:51:22 by lpaula-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putunbr_fd(unsigned int n, int fd)
{
	if (n >= 10)
		ft_putunbr_fd(n / 10, fd);
	ft_putchar_fd((n % 10) + '0', fd);
}

int	ft_printf_putunbr_fd(unsigned int n, int fd)
{
	int	count;

	count = 0;
	if (n >= 10)
		count += ft_printf_putunbr_fd(n / 10, fd);
	count += ft_printf_putchar_fd((n % 10) + '0', fd);
	return (count);
}
