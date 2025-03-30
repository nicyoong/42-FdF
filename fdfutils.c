/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdfutils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyoong <nyoong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 01:40:15 by nyoong            #+#    #+#             */
/*   Updated: 2025/03/30 22:10:14 by nyoong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	exit_error(char *msg)
{
	ft_printf("Error: %s\n", msg);
	exit(1);
}

void	free_split(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

int	count_words(const char *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

int	ft_atoi_base(const char *str, int base)
{
	int		result;
	char	*ptr;

	result = 0;
	ptr = (char *)str;
	if (base == 16 && ft_strncmp(ptr, "0x", 2) == 0)
		ptr += 2;
	while (*ptr)
	{
		if (*ptr >= '0' && *ptr <= '9')
			result = result * base + (*ptr - '0');
		else if (base > 10 && *ptr >= 'a' && *ptr <= 'f')
			result = result * base + (*ptr - 'a' + 10);
		else if (base > 10 && *ptr >= 'A' && *ptr <= 'F')
			result = result * base + (*ptr - 'A' + 10);
		else
			break ;
		ptr++;
	}
	return (result);
}

void	handle_node_allocation_failure(void *line)
{
	free(line);
	exit_error("Failed to allocate list node");
}
