/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:14:02 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/03/19 12:30:38 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

int	main(int argc, char **argv)
{
	t_minirt	minirt;
	t_scene		*scene;
	int	exit_code;

	minirt = (t_minirt){0};
	scene = &minirt.scene;
	(void)argc;
	(void)argv;
	//what it should do:
	//
	// if (argc != 2)
	// 	return (0);
	// if (parse_scene(argv[1], &scene))
		// return (1);
	// render_scene(scene);
	scene->camera = camera_new(vec3_new(0, 0, 0), vec3_new(0, 0, 1), 60);
	scene->sphere_count = 1;
	scene->spheres = malloc(sizeof(t_sphere) * scene->sphere_count);
	scene->spheres[0] = (t_sphere){
		.pos = vec3_new(0, 0, -2),
		.color = color_new(1, 0, 0),
		.diameter = 1.0f
	};
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
