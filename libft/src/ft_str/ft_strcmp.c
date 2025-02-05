/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shoaib <shoaib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:17:46 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/01/31 14:30:31 by shoaib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	const unsigned char	*_s1;
	const unsigned char	*_s2;
	size_t				i;

	i = 0;
	_s1 = (const unsigned char *)s1;
	_s2 = (const unsigned char *)s2;
	while (_s1[i] == _s2[i] && _s1[i])
		i++;
	return (_s1[i] - _s2[i]);
}