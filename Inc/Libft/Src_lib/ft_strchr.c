/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andcarva <andcarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 15:15:34 by andcarva          #+#    #+#             */
/*   Updated: 2025/06/05 15:31:02 by andcarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strchr(const char *ptr, int c)
{
	if (!ptr)
		return (0);
	while (*ptr != (char)c && *ptr != '\0')
	{
		ptr++;
	}
	if (*ptr == (char)c)
		return ((char *)ptr);
	return (0);
}

/* int	main(void)
{
	char *p = "Hello World";
	int	c = 'e';
	
	printf("%s\n", ft_strchr(p, c)); 
} */
