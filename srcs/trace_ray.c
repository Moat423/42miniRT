#include "../include/miniRT.h"

t_color	trace_ray(t_ray ray, t_scene *scene)
{
	t_intersection	intersection;

	//intersection = find_closest_intersection(ray, scene);
	//if (intersection.object != NULL)
	//	return (calculate_lighting(intersection, scene));
	//else
	return (color_new(0, 0, 0));
}
