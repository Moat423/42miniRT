/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:14:02 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/10 15:48:49 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"
#include "../include/parse.h"

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

// BONUS PART (fork of mandatory)
//TODO: checkerboard pattern
//TODO: bump map textures
//TODO: parallelize the raytracing (bonus only)
//TODO: shadow not get closest, but instead get any intersect (bool)

/// Returns the distance² after which there is no light contribution from this light
float max_light_distance_sq(t_light light) {
	// Light contributions below 1/512 are considered negligible
    float threshold = 0.0019;

    // Get maximum brightness component from light color
    float max_color_component = fmaxf(light.color.x, fmaxf(light.color.y, light.color.z));

    // If your light has color components, use the maximum component for brightness

    // Calculate effective brightness (intensity × brightest color component)
    float effective_brightness = light.brightness * max_color_component;

    // Calculate the squared distance where attenuation falls below threshold
    // Based on formula: attenuation = light.brightness / (distance² / LIGHT_DIST)
    float max_distance_squared = effective_brightness * LIGHT_DIST / threshold;
    return max_distance_squared;
}

void calc_max_light_distances(t_scene *scene)
{
	size_t	i;

	i = 0;
	while (i < scene->light_count)
	{
		scene->lights[i].max_dist = sqrtf(max_light_distance_sq(scene->lights[i]));
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_minirt	minirt;
	t_scene		*scene;
	int			exit_code;

	minirt = (t_minirt){0};
	scene = &minirt.scene;
	if (!(input_check(argc) && !parse_scene(argv[1], scene)))
	{
		scene_destroy(scene);
		return (1);
	}
	calc_max_light_distances(scene);
	print_scene(scene);
	scene->image_width = 800;
	scene->image_height = 600;
	exit_code = render_loop(&minirt);
	printf("exit code: %d\nCleaning up...", exit_code);
	mlx_delete_image(minirt.mlx, minirt.image);
	minirt.image = NULL;
	mlx_terminate(minirt.mlx);
	minirt.mlx = NULL;
	scene_destroy(scene);
	return (0);
}
