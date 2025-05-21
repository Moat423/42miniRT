/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_png_texture.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:06:48 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/21 11:45:17 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parse.h"

static int	can_assign_texture(t_texturing *texturing)
{
	if (texturing->type != 0)
	{
		ft_fprintf(2, "ERROR\nalready has a type: %d", texturing->type);
		return (0);
	}
	texturing->type = TEXTURE;
	return (1);
}

int	set_png_texture(char *obj_line, t_texturing *texturing)
{
	char	*filename;
	int		position;

	if (!can_assign_texture(texturing))
		return (0);
	position = 0;
	while (ft_isspace(obj_line[position]))
		++position;
	filename = ft_subword(obj_line);
	if (!filename)
	{
		ft_fprintf(2, "ERROR\nfailed creating subword from line for filename");
		return (0);
	}
	texturing->texture = (mlx_load_png(filename));
	free(filename);
	if (!texturing->texture)
		return (0);
	while (obj_line[position] && (!ft_isspace(obj_line[position])
			|| obj_line[position] == '\n'))
		++position;
	return (position);
}
