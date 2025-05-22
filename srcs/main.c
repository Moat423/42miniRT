/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:14:02 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/22 13:43:58 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"
#include "../include/parse.h"

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
