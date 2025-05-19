/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_bumpmap_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:49:03 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/19 19:17:08 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/libft/libft_full.h"
#include "../../include/parse.h"

// creates a word while reading till 
// the null temrinator,
// or a newline
// or any isspace character
char	*ft_subword(char *line)
{
	char	*new;
	size_t	i;

	i = 0;
	while (line[i] && !ft_isspace(line[i]))
		++i;
	new = ft_substr(line, 0, i);
	return (new);
}

// if this returns -1, somethig went wrong and an errormessage has been printed
int	open_bumpmap_file(char *line)
{
	int		fd;
	char	*filename;

	while (*line == ' ')
		line++;
	filename = ft_subword(line);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_fprintf(2, "Error\ninvalid file: %s\n", strerror(errno));
		ft_fprintf(2, "filename: %s\n", filename);
		free(filename);
		return (-1);
	}
	free(filename);
	return (fd);
}

// return NULL, print string, clean up get_next_line
void	*rperror_get_next_line(char *line, char *errormessage)
{
	ft_fprintf(2, errormessage);
	free(line);
	get_next_line(-1);
	return (NULL);
}
