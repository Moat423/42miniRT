/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:14:02 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/03/21 17:13:24 by lmeubrin         ###   ########.fr       */
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
