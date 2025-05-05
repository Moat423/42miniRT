/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_texture.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 19:35:47 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/05 19:38:00 by lmeubrin         ###   ########.fr       */
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
static	char	*parse_file_head(int fd)
{
	char	*line;

	line = get_next_line(fd);
	if (!(line[0] == 'P' && line[1] == '3' && line[2] == '\n'))
	{
		return (rperror_get_next_line(line,
				"Error\n file is not in correct format\nExpected .ppm\n"));
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

// starts parsing the file to get file id (only accepting P3 for .ppm)
// gets width and height to allocate struct
t_bumpmap	*allocate_bumpmap(int fd)
{
	char		*line;
	t_bumpmap	*bump;
	size_t		width;
	size_t		height;
	char		*line_after_number;

	line = parse_file_head(fd);
	if (!line)
		return (NULL);
	width = ft_strtoimax(line, &line_after_number, 10);
	if (width == 0 && line == line_after_number)
		return ((t_bumpmap *)rperror_get_next_line(line,
				"Error in ft_strtoimax\n"));
	while (ft_isspace(*line_after_number))
		line_after_number++;
	height = ft_strtoimax(line_after_number, NULL, 10);
	free(line);
	line = get_next_line(fd);
	if (ft_strncmp(line, "255\n", 4))
		return ((t_bumpmap *)rperror_get_next_line(line,
				"Error got different max colour value\n"));
	free(line);
	bump = ft_malloc_bumpmap(width, height);
	return (bump);
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

