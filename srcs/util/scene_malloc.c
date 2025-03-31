#include "../../include/miniRT_types.h"
#include "../../include/parse.h"

// returns 0 on failure and 
// 1 if it was sucessful or didn't alloc annything
int	ft_malloc_array(const size_t count, const size_t size, void **arr)
{
	if (count)
	{
		*arr = malloc(count * size);
		if (!*arr)
			return (0);
	}
	return (1);
}

//return 0 on faulure also checks if at least one light in scene
int	ft_malloc_scene_arrays(t_scene *scene)
{
	if (scene->light_count == 0)
		return (ft_parseerror("Need Lights! Give at least one L", NULL));
	if (!ft_malloc_array(scene->light_count, sizeof(t_light), 
			(void **)&(scene->lights)))
		return (0);
	if (!ft_malloc_array(scene->sphere_count, sizeof(t_sphere), 
			(void **)&(scene->spheres)))
		return (0);
	if (!ft_malloc_array(scene->plane_count, sizeof(t_plane), 
			(void **)&(scene->planes)))
		return (0);
	if (!ft_malloc_array(scene->cylinder_count, sizeof(t_cylinder), 
			(void **)&(scene->cylinders)))
		return (0);
	if (!ft_malloc_array(scene->cone_count, sizeof(t_cone), 
			(void **)&(scene->cones)))
		return (0);
	return (1);
}
