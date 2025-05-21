/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_texture.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 19:35:47 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/07 14:24:30 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"
#include "../../include/parse.h"

static char	*skip_hash_lines(char *line, int fd)
{
	line = get_next_line(fd);
	while (line && (line[0] == '#'))
	{
		free(line);
		line = get_next_line(fd);
	}
	if (!line)
	{
		get_next_line(-1);
		ft_fprintf(2, "Error skipping comments");
	}
	return (line);
}

//checks for P3 in first line and skipps following comments
char	*parse_file_head(int fd)
{
	char	*line;

	line = get_next_line(fd);
	if (!(line[0] == 'P' && line[1] == '3' && line[2] == '\n'))
	{
		ft_fprintf(2, "Error\nline: %s", line);
		return (rperror_get_next_line(line,
				"file is not in correct format\nExpected .ppm\n"));
	}
	free(line);
	line = skip_hash_lines(line, fd);
	if (!line)
	{
		return (rperror_get_next_line(line,
				"Error in get_next_line\n"));
	}
	return (line);
}

int	get_val(int fd)
{
	char	*line;
	char	*endptr;
	int		num;

	line = get_next_line(fd);
	if (!line)
		return (-1);
	num = ft_strtoimax(line, &endptr, 10);
	if (!num && (*endptr) != '\n')
	{
		ft_fprintf(2, "Error\ninvalid number in file\n");
		free(line);
		return (-1);
	}
	free(line);
	return (num);
}
