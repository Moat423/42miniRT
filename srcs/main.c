/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:14:02 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/03/18 16:58:50 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

int	main(int argc, char **argv)
{
	t_scene	scene;
	int	exit_code;

	(void)argc;
	(void)argv;
	//what it should do:
	//
	// if (argc != 2)
	// 	return (0);
	// if (parse_scene(argv[1], &scene))
		// return (1);
	// render_scene(scene);
	scene = (t_scene){0};
	scene.image_width = 800;
	scene.image_height = 600;
	exit_code = render_loop(&scene);
	printf("exit code: %d\nCleaning up...", exit_code);
	mlx_delete_image(scene.mlx, scene.image);
	scene.image = NULL;
	mlx_terminate(scene.mlx);
	scene.mlx = NULL;
	scene_destroy(&scene);
	return (0);
}
