/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:14:02 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/03/18 15:10:07 by kwurster         ###   ########.fr       */
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
	exit_code = render_loop(&scene);
	mlx_delete_image(scene.mlx, scene.image);
	scene.image = NULL;
	mlx_terminate(scene.mlx);
	scene.mlx = NULL;
	scene_destroy(&scene);
	return (0);
}
