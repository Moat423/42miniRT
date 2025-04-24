/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:14:02 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/16 14:33:22 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"
#include "../include/parse.h"

// trace_ray_intersection_t_total
// intersect_t_avg_sphere
// intersect_t_avg_plane
// intersect_t_avg_cylinder
// intersect_t_avg_cone
// trace_ray_shade_t_total
// shade_light_contribution[light_count]
// // display percentage and totals
// shade_light_contribution[].lambert_skips
// // display percentage and totals
// shade_light_contribution[].shadow_test_culled_objects
// shade_light_contribution[].shadow_test_closest_intersect_tested_total
// shade_light_contribution[].shadow_test_t

// MANDATORY PART
//TODO: valgrind!!
//TODO: implement more options for dynamic camera movement for example
// pgup/pgdown to move the camera up and down and mouse for rotation
//TODO: compile time checks for things that should be disabled if not compiling bonus
// 1. single light
// 2. cones
//TODO: precalculate on scene parsing which lights might affect which objects
// this will optimize large scenes with many lights which are scattered loosely around the scene
// without this we will not be able to render large scenes with many lights sufficiently fast
// for this we need to create bounding boxes for each object (except for the plane) and then
// check if the bounding box is in range of a light source
// the objects which are in range will be stored in another small scene struct specific to the light
//TODO: sort objects by distance to origin:
// scene objects dont need to be sorted because we check all objects anyway
// but light objects in relation to light position would be beneficial

// BONUS PART (fork of mandatory)
//TODO: checkerboard pattern
//TODO: bump map textures
//TODO: parallelize the raytracing (bonus only)
//TODO: shadow not get closest, but instead get any intersect (bool)

void	minirt_exit(t_minirt *minirt, int status)
{
	mlx_close_window(minirt->mlx);
	if (minirt->image)
		mlx_delete_image(minirt->mlx, minirt->image);
	minirt->image = NULL;
	mlx_terminate(minirt->mlx);
	minirt->mlx = NULL;
	scene_destroy(&minirt->scene);
	exit(status);
}

void	calculate_derived_scene_values(t_scene *scene);
void	fix_unnormalized_scene_vectors(t_scene *scene);

int	main(int argc, char **argv)
{
	t_minirt	minirt;
	t_scene		*scene;

	minirt = (t_minirt){0};
	scene = &minirt.scene;
	if (!(input_check(argc) && !parse_scene(argv[1], scene)))
	{
		scene_destroy(scene);
		return (1);
	}
	printf("Scene loaded successfully\n");
	fix_unnormalized_scene_vectors(scene);
	calculate_derived_scene_values(scene);
	if (!fill_lights_for_objects(scene) || !malloc_light_objects(scene)
		|| !fill_objects_for_lights(scene))
	{
		scene_destroy(scene);
		return (1);
	}
	printf("Printing scene...\n");
	print_scene(scene);
	scene->image_width = 800;
	scene->image_height = 600;
	minirt_exit(&minirt, render_loop(&minirt));
}
