/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_texturing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moat <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 11:44:54 by moat              #+#    #+#             */
/*   Updated: 2025/05/16 18:45:09 by moat             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"
#include "../../include/parse.h"

int	set_checkers(char *line, t_texturing *texturing)
{
	int			i;
	t_checkers *ch;

	ch = malloc(sizeof(t_checkers));
	if (!ch)
		return (0);
	*ch = default_uv_checkers();
	texturing->type = CHECKERS;
	i = ft_skip_space(line, 0);
	texturing->obj_colouring = ch;
	if (!ft_isdigit(line[i]))
	{
		ft_fprintf(2, "proceeding with default checkers colour\n");
		return (-1);
	}
	i = set_color(line, i, &(ch->color_a));
	if (!i)
		return (0);
	i = set_color(line, i, &(ch->color_b));
	if (!i)
		return (0);
	return (i);
}

int	set_texturing(char *line, t_texturing *texturing)
{
	int	i;
	int	type;
	int	j;

	i = ft_skip_space(line, 0);
	while (line[i] != '\0')
	{
		type = ft_set_texture_flag(&i, line);
		if (!type)
			break ;
		if (type == BUMP)
			j = set_bumpmap(&(line[i]), &(texturing->bumpmap));
		else if (type == CHECKERS)
			j = set_checkers(&line[i], texturing);
		else if (type == TEXTURE)
		{
			ft_fprintf(2, "Feature not yet available\nyou requested textures with te\n");
			j = 0;
		}
		if (!j)
			return (0);
		else if (line[i + (j && j > 0)] == '\n')
			return (1);
		i = ft_skip_space(line, i + j);
	}
	if ((line[i] == '\n') || line[i] == '\0')
		return (1);
	ft_parseerror("expected end of line here", line, i);
	return (0);
}
