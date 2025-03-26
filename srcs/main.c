/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:14:02 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/03/26 13:13:47 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"
#include "../include/parse.h"

//TODO: valgrind!!
//TODO: implement specular reflection
//TODO: implement cone shape
//TODO: implement more options for dynamic camera movement or also object movement/rotation
// for example pgup/pgdown to move the camera up and down and mouse for rotation
//TODO: lazy\deferred rendering can be optimized to
// not wait x milliseconds after last render request but rather request that
// the rendering must be deferred to only the next frame if
// there isnt another key/mouse input on the next frame before rendering
//TODO: checkerboard pattern
//TODO: optimization for light calculation.
// We currently try to check for every point if it is in shadow of
// any/all lights no matter how far away the light is.
// Quadratic slowdown per light on the scene.
//TODO: bvh tree
//TODO: shadow not get closest, but get any intersect
//TODO: for bvh where we have arrays of pointers to generic objects, we want to store their specific intersection function inside the generic object struct

int	main(int argc, char **argv)
{
	t_minirt	minirt;
	t_scene		*scene;
	int	exit_code;

	minirt = (t_minirt){0};
	minirt.last_render_request_time = INFINITY;
	scene = &minirt.scene;
	(void)argc;
	(void)argv;
	if (input_check(argc) == 0)
		return (1);
	if (parse_scene(argv[1], scene))
		return (1);
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
