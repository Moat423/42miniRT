/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_bumpmap.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:09:04 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/21 14:33:40 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"
#include "../../include/parse.h"

static int	fill_bump_greyscale(t_bumpmap *bump, int fd)
{
	int		r;
	int		g;
	int		b;
	int		max_pixels;
	int		i;

	i = 0;
	max_pixels = bump->height * bump->width;
	while (i <= max_pixels)
	{
		r = get_val(fd);
		if (r == -1)
			break ;
		g = get_val(fd);
		if (g == -1)
			return (0);
		b = get_val(fd);
		if (b == -1)
			return (0);
		bump->elevation[i] = ((float)r + (float)g + (float)b) / (3.0f * 255.0f);
		++i;
	}
	return (1);
}

// function to call to fill bumpmap object in sphere
int	set_bumpmap(char *obj_line, t_bumpmap **bumpmap)
{
	int			fd;
	t_bumpmap	*bump;
	int			position;

	position = 0;
	while (ft_isspace(obj_line[position]))
		++position;
	fd = open_bumpmap_file(&obj_line[position]);
	if (fd == -1)
		return (0);
	bump = allocate_bumpmap(fd);
	if (!bump)
		return (0);
	if (!fill_bump_greyscale(bump, fd))
		return (0);
	*bumpmap = bump;
	while (obj_line[position] && (!ft_isspace(obj_line[position])
			|| obj_line[position] == '\n'))
		++position;
	return (position);
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
	bump = ft_malloc_bumpmap(width, height);
	return (bump);
}
