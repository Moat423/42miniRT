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
