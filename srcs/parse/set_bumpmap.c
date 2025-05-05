/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_bumpmap.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:09:04 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/05 19:38:52 by lmeubrin         ###   ########.fr       */
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
	get_next_line(-1);
	return (1);
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
	if (!fill_bump_greyscale(bump, fd))
		return (0);
	sphere->bumpmap = bump;
	return (1);
}
