/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:14:02 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/03/20 13:14:32 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"
#include "../include/parse.h"

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
	// render_scene(scene);
	// scene->camera = camera_new(vec3_new(0, 0, 0), vec3_new(0, 0, 1), 60);
	// scene->sphere_count = 2;
	// scene->spheres = malloc(sizeof(t_sphere) * scene->sphere_count);
	// scene->spheres[0] = (t_sphere){
	// 	.pos = vec3_new(0, 0, -3),
	// 	.color = color_new(1, 0, 0),
	// 	.radius = 1.3f
	// };
	// scene->spheres[1] = (t_sphere) {
	// 	.pos = vec3_new(0,-100.5,-1),
	// 	.color = color_new(0, 1, 0),
	// 	.radius = 100.0f,
	// };
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
