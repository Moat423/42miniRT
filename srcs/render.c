/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 14:35:51 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/18 15:16:00 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

void	window_resize(int32_t width, int32_t height, void* param)
{
	t_scene	*scene;

	scene = param;
	if (scene->image && scene->mlx)
	{
		scene->image_width = width;
		scene->image_height = height;
		mlx_delete_image(scene->mlx, scene->image);
		scene->image = mlx_new_image(scene->mlx, scene->image_width, scene->image_height);
		// TODO render the scene here
		printf("Resized window to %dx%d\n", width, height);
	}
	else
		printf("Tried to resize window with image or mlx == null.\n");
}

void	window_close(void *param)
{
	t_scene	*scene;

	scene = param;
	(void)scene;
	printf("Window close hook called\n");

	// need this? or is it already called?
	//mlx_close_window(scene->mlx);
	//scene_destroy(scene);
}

/// @brief Render loop
/// @param scene The scene to render
/// @return Exit code. EXIT_SUCCESS on success, EXIT_FAILURE on failure
int	render_loop(t_scene *scene)
{
	scene->mlx = mlx_init(scene->image_width, scene->image_height, "miniRT", true);
	if (!scene->mlx)
	{
		printf("Failed to initialize MLX: %s\n", mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	scene->image = mlx_new_image(scene->mlx, scene->image_width, scene->image_height);
	if (!scene->image)
	{
		mlx_close_window(scene->mlx);
		printf("Failed to create image: %s\n", mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	if (mlx_image_to_window(scene->mlx, scene->image, 0, 0) == -1)
	{
		mlx_close_window(scene->mlx);
		printf("Failed to draw image to window: %s\n", mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}

	//mlx_loop_hook(mlx, ft_randomize, scene);
	//mlx_loop_hook(mlx, ft_hook, scene);
	mlx_resize_hook(scene->mlx, window_resize, scene);
	mlx_close_hook(scene->mlx, window_close, scene);
	mlx_loop(scene->mlx);
	return (EXIT_SUCCESS);
}
