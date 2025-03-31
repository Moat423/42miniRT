#include "../../include/miniRT_types.h"
#include "../../include/parse.h"

// returns 0 on failure and 
// 1 if it was sucessful or didn't alloc annything
int	ft_malloc_array(const size_t count, const size_t size, void *arr)
{
	if (count)
	{
		arr = malloc(count * sizeof(t_sphere));
		if (!arr)
			return (0);
	}
	return (1);
}

//return 0 on faulure also checks if at least one light in scene
int	ft_malloc_scene_arrays(t_scene *scene)
{
	if (scene->light_count == 0)
		return (ft_parseerror("Need Lights! Give at least one L", NULL));
	if (!ft_malloc_array(scene->light_count, sizeof(t_light), &(scene->lights)))
		return (0);
	// scene->lights = malloc(scene->light_count * sizeof(t_light));
	// if (!scene->lights)
	// 	return (0);
	if (scene->sphere_count)
	{
		scene->spheres = malloc(scene->sphere_count * sizeof(t_sphere));
		if (!scene->spheres)
			return (0);
	}
	if (scene->plane_count)
	{
		scene->planes = malloc(scene->plane_count * sizeof(t_plane));
		if (!scene->planes)
			return (0);
	}
	if (scene->cylinder_count)
	{
		scene->cylinders = malloc(scene->cylinder_count * sizeof(t_cylinder));
		if (!scene->cylinders)
			return (0);
	}
	return (1);
}
