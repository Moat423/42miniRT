/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_textures.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:49:03 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/05 14:55:54 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

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
	new = ft_substr(line, 0, i + 1);
	return (new);
}

// if this returns -1, somethig went wrong and an errormessage has been printed
int	open_bumpmap_file(char *line)
{
	int		i;
	int		fd;
	char	*filename;

	i = 0;
	i = ft_skip_space(line, i);
	filename = ft_subword(&line[i]);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_fprintf(2, "Error\ninvalid file: %s\n", strerror(errno));
		ft_fprintf(2, "filename: %s\n", &filename[i]);
		free(filename);
		return (-1);
	}
	free(filename);
	return (fd);
}

char	*skip_hash_lines(char *line, int fd)
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

// return NULL, print string, clean up get_next_line
void	*rperror_get_next_line(char *line, char *errormessage)
{
	ft_fprintf(2, errormessage);
	free(line);
	get_next_line(-1);
	return (NULL);
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

	line = get_next_line(fd);
	if (!(line[0] == 'P' && line[1] == '3' && line[3] == '\n'))
		return (rperror_get_next_line(line,
				"Error\n file is not in correct format\nExpected .ppm"));
	free(line);
	line = skip_hash_lines(line, fd);
	if (!line)
		return (NULL);
	width = ft_strtoimax(line, &line_after_number, 10);
	if (width == 0 && line == line_after_number)
		return (rperror_get_next_line(line, "Error in ft_strtoimax"));
	while (ft_isspace(*line_after_number))
		line_after_number++;
	height = ft_strtoimax(line_after_number, NULL, 10);
	bump = ft_malloc_bumpmap(width, height);
	if (!bump)
		perror("malloc");
	return (bump);
}

int	set_bumpmap(char *obj_line, t_sphere *sphere)
{
	int			fd;
	char		*line;
	t_bumpmap	*bump;

	fd = open_bumpmap_file(obj_line);
	bump = allocate_bumpmap(fd);
	if (!bump)
		return (0);
	sphere->bumpmap = bump;
	// bumpmap_destroy(bump);
	get_next_line(-1);
	return (1);
}
