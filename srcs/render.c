/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 14:35:51 by kwurster          #+#    #+#             */
/*   Updated: 2025/04/14 14:08:18 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static int32_t	rgb_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

void	render_raytraced(void *param)
{
	t_minirt	*minirt;
	uint32_t	x;
	uint32_t	y;
	t_ray		ray;
	size_t		i;

	minirt = param;
	y = 0;
	while (y < minirt->scene.image_height)
	{
		x = 0;
		while (x < minirt->scene.image_width)
		{
			i = (y * minirt->scene.image_width + x) * 4;
			ray = get_viewport_ray(&minirt->scene,
				(float)x / (float)minirt->scene.image_width,
				(float)y / (float)minirt->scene.image_height);
			color_to_rgb(trace_ray(&minirt->scene, ray),
				&minirt->image->pixels[i],
				&minirt->image->pixels[i + 1],
				&minirt->image->pixels[i + 2]);
			minirt->image->pixels[i + 3] = 255;
			x++;
		}
		y++;
	}
}

// for now, fill all pixels with random RGB colors with full opacity
void	render_random(void *param)
{
	t_minirt	*minirt;
	uint32_t	x;
	uint32_t	y;
	uint32_t	color;
	uint32_t	*pixels;

	minirt = param;
	pixels = (uint32_t*)minirt->image->pixels;
	y = 0;
	while (y < minirt->scene.image_height)
	{
		x = 0;
		while (x < minirt->scene.image_width)
		{
			color = rgb_pixel(rand() % 256, rand() % 256, rand() % 256, 255);
			pixels[y * minirt->scene.image_width + x] = color;
			x++;
		}
		y++;
	}
}

void	render(void *param)
{
	double		before_render;

	before_render = mlx_get_time();
	render_raytraced(param);
	printf("Rendered frame in %f seconds\n", mlx_get_time() - before_render);
}

void	window_resize(int32_t width, int32_t height, void* param)
{
	t_minirt	*minirt;

	minirt = param;
	if (minirt->image && minirt->mlx)
	{
		minirt->scene.image_width = width;
		minirt->scene.image_height = height;
		// TODO error handling like in render_loop
		mlx_delete_image(minirt->mlx, minirt->image);
		minirt->image = mlx_new_image(minirt->mlx, minirt->scene.image_width, minirt->scene.image_height);
		mlx_image_to_window(minirt->mlx, minirt->image, 0, 0);
		minirt->loop_state = RESIZING;
		minirt->last_resize_time = mlx_get_time();
		printf("Resized window to %dx%d\n", width, height);
	}
	else
		printf("Tried to resize window with image or mlx == null.\n");
}

void	window_close(void *param)
{
	t_minirt	*minirt;

	minirt = param;
	(void)minirt;
	printf("Window close hook called\n");
}

void	render_on_request(void *param)
{
	t_minirt	*minirt;

	minirt = param;
	if (minirt->loop_state == RENDER_NOW
		|| (minirt->loop_state == RESIZING
			&& mlx_get_time() - minirt->last_resize_time > 0.5))
	{
		render((void*)minirt);
		minirt->loop_state = NO_ACTION;
	}
}

/// @brief Render loop
/// @param scene The scene to render
/// @return Exit code. EXIT_SUCCESS on success, EXIT_FAILURE on failure
int	render_loop(t_minirt *minirt)
{
	t_scene	*scene;

	scene = &minirt->scene;
	minirt->mlx = mlx_init(scene->image_width, scene->image_height, "miniRT", true);
	if (!minirt->mlx)
	{
		printf("Failed to initialize MLX: %s\n", mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	minirt->image = mlx_new_image(minirt->mlx, scene->image_width, scene->image_height);
	if (!minirt->image)
	{
		mlx_close_window(minirt->mlx);
		printf("Failed to create image: %s\n", mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	render((void*)minirt);
	if (mlx_image_to_window(minirt->mlx, minirt->image, 0, 0) == -1)
	{
		mlx_close_window(minirt->mlx);
		printf("Failed to draw image to window: %s\n", mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	//mlx_loop_hook(mlx, ft_randomize, scene);
	//mlx_loop_hook(mlx, ft_hook, scene);
	mlx_loop_hook(minirt->mlx, key_press, minirt);
	mlx_loop_hook(minirt->mlx, render_on_request, minirt);
	mlx_resize_hook(minirt->mlx, window_resize, minirt);
	mlx_close_hook(minirt->mlx, window_close, minirt);
	mlx_loop(minirt->mlx);
	return (EXIT_SUCCESS);
}
