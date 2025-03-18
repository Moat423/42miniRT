/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 14:35:51 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/18 17:22:29 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

// TODO allow pressing esc to exit
// TODO dont render on every resize call.
// resize should only queue a render for ~250ms after the last resize event
// store an accumulation of time deltas from the mlx struct in the scene struct
// to determine when to render. Store a timestamp of when the last resize event was called.
// if the time delta is greater than 250ms, render the scene and clear the resize event.

static int32_t	rgb_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

// for now, fill all pixels with random RGB colors with full opacity
void	render(void *param)
{
	t_scene	*scene;
	uint32_t	x;
	uint32_t	y;
	uint32_t	color;
	uint32_t	*pixels;

	scene = param;
	pixels = (uint32_t*)scene->image->pixels;
	y = 0;
	while (y < scene->image_height)
	{
		x = 0;
		while (x < scene->image_width)
		{
			color = rgb_pixel(rand() % 256, rand() % 256, rand() % 256, 255);
			pixels[y * scene->image_width + x] = color;
			x++;
		}
		y++;
	}
}

void	window_resize(int32_t width, int32_t height, void* param)
{
	t_scene	*scene;

	scene = param;
	if (scene->image && scene->mlx)
	{
		scene->image_width = width;
		scene->image_height = height;
		// TODO error handling like in render_loop
		mlx_delete_image(scene->mlx, scene->image);
		scene->image = mlx_new_image(scene->mlx, scene->image_width, scene->image_height);
		mlx_image_to_window(scene->mlx, scene->image, 0, 0);
		render((void*)scene);
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
	render((void*)scene);
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
