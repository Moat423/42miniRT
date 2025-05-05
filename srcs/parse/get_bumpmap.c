/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_bumpmap.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:09:04 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/05 16:47:34 by lmeubrin         ###   ########.fr       */
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
static t_bumpmap	*allocate_bumpmap(int fd)
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
	get_next_line(-1);
	bump = ft_malloc_bumpmap(width, height);
	return (bump);
}

int	set_bumpmap(char *obj_line, t_sphere *sphere)
{
	int			fd;
	char		*line;
	t_bumpmap	*bump;

	fd = open_bumpmap_file(obj_line);
	if (fd == -1)
		return (0);
	bump = allocate_bumpmap(fd);
	if (!bump)
		return (0);
	sphere->bumpmap = bump;
	return (1);
}
