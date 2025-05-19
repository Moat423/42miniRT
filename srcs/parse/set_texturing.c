/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_texturing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moat <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 11:44:54 by moat              #+#    #+#             */
/*   Updated: 2025/05/19 18:46:29 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"
#include "../../include/parse.h"

int	set_checkers(char *line, t_texturing *texturing,
					const float width, const float height)
{
	int			i;
	t_checkers	*ch;

	ch = malloc(sizeof(t_checkers));
	if (!ch)
		return (0);
	*ch = uv_checkers(width, height, (t_vec3){1, 1, 1}, (t_vec3){0, 0, 0});
	texturing->type = CHECKERS;
	i = ft_skip_space(line, 0);
	texturing->checkers = ch;
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

// return 0 on error in parse
// returns -1 if good line ending has been discovered
// returns i+ j if line ending has not been discovered
// uses default values for the SPHERE!
int	set_type_texture(char *line, t_texturing *texturing, const int i,
						const t_texture type)
{
	int	j;

	j = 0;
	if (type == BUMP)
		j = set_bumpmap(&(line[i]), &(texturing->bumpmap));
	else if (type == CHECKERS)
		j = set_checkers(&line[i], texturing, SP_CH_WIDTH, SP_CH_HEIGHT);
	else if (type == TEXTURE)
	{
		ft_fprintf(2, "Feature not yet available:\nte is for textures\n");
		j = 0;
	}
	if (!j)
		return (0);
	else if (line[i + (j && j > 0)] == '\n')
		return (-1);
	return (i + j);
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
		j = set_type_texture(line, texturing, i, type);
		if (j > 0)
			i = ft_skip_space(line, j);
		else
			return (1 && j);
	}
	if ((line[i] == '\n') || line[i] == '\0')
		return (1);
	ft_parseerror("expected end of line here", line, i);
	return (0);
}

// return 0 on error in parse
// returns -1 if good line ending has been discovered
// returns i+ j if line ending has not been discovered
// uses default values for the PLANE
int	set_plane_type_texture(char *line, t_texturing *texturing, const int i,
						const t_texture type)
{
	int	j;

	j = 0;
	if (type == BUMP)
		j = set_bumpmap(&(line[i]), &(texturing->bumpmap));
	else if (type == CHECKERS)
		j = set_checkers(&line[i], texturing, PL_CH_WIDTH, PL_CH_HEIGHT);
	else if (type == TEXTURE)
	{
		ft_fprintf(2, "Feature not yet available:\nte is for textures\n");
		j = 0;
	}
	if (!j)
		return (0);
	else if (line[i + (j && j > 0)] == '\n')
		return (-1);
	return (i + j);
}

// grabs texturing and sets default values for checkers for plane
int	set_plane_texturing(char *line, t_texturing *texturing)
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
		j = set_plane_type_texture(line, texturing, i, type);
		if (j > 0)
			i = ft_skip_space(line, j);
		else
			return (1 && j);
	}
	if ((line[i] == '\n') || line[i] == '\0')
		return (1);
	ft_parseerror("expected end of line here", line, i);
	return (0);
}
