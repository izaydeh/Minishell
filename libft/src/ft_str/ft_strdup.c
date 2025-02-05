/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sal-kawa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:23:52 by sal-kawa          #+#    #+#             */
/*   Updated: 2024/09/05 15:23:57 by sal-kawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char		*dest;
	size_t		length;

	length = ft_strlen(s);
	dest = malloc(length + 1);
	if (dest == NULL)
		return (NULL);
	ft_strlcpy(dest, s, length + 1);
	return (dest);
}
/*int main(void)
{
    const char *original = "Hello, World!";
    char *duplicate;
    duplicate = ft_strdup(original);
    if (duplicate == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    printf("Original: %s\n", original);
    printf("Duplicate: %s\n", duplicate);
    free(duplicate);

    return 0;
}*/
