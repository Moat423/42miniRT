/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_malloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 11:27:18 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/05 11:23:51 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT_types.h"
#include "../../include/parse.h"

// returns 0 on failure and
// 1 if it was sucessful or didn't alloc annything
int	ft_malloc_array(const size_t count, const size_t size, void **arr)
{
	if (count)
	{
		*arr = ft_calloc(count, size);
		if (!*arr)
			return (0);
	}
	return (1);
}

//return 0 on failure
int	ft_malloc_objects(t_objects *objects)
{
	if (!ft_malloc_array(objects->sphere_count, sizeof(t_sphere),
			(void **)&(objects->spheres)))
		return (0);
	if (!ft_malloc_array(objects->plane_count, sizeof(t_plane),
			(void **)&(objects->planes)))
		return (0);
	if (!ft_malloc_array(objects->cylinder_count, sizeof(t_cylinder),
			(void **)&(objects->cylinders)))
		return (0);
	if (!ft_malloc_array(objects->cone_count, sizeof(t_cone),
			(void **)&(objects->cones)))
		return (0);
	return (1);
}

//return 0 on failure also checks if at least one light in scene
int	ft_malloc_scene_arrays(t_scene *scene)
{
	if (scene->light_count == 0)
		return (ft_parseerror("Need Lights! Give at least one L", NULL, 0));
	if (!ft_malloc_array(scene->light_count, sizeof(t_light),
			(void **)&(scene->lights)))
		return (0);
	if (!ft_malloc_objects(&(scene->objs)))
		return (0);
	return (1);
}

t_bumpmap	*ft_malloc_bump_map(size_t width, size_t height)
{
	t_bumpmap	*bump;

	bump = ft_calloc(sizeof(*bump), 1);
	if (!bump)
		return (NULL);
	bump->height = height;
	bump->width = width;
	if (!ft_malloc_array(width, height, (void **)&(bump->elevation)))
	{
		free(bump);
		return (NULL);
	}
	return (bump);
}
