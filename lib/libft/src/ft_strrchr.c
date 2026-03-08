/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaula-n <lpaula-n@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 10:34:42 by lpaula-n          #+#    #+#             */
/*   Updated: 2026/03/07 20:51:22 by lpaula-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*last;
	char	ch;
	size_t	i;

	ch = (char)c;
	last = NULL;
	i = 0;
	while (s[i])
	{
		if (s[i] == ch)
			last = (char *)&s[i];
		i++;
	}
	if (ch == '\0')
		return ((char *)&s[i]);
	return (last);
}
