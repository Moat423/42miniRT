/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 14:35:51 by kwurster          #+#    #+#             */
/*   Updated: 2025/04/22 16:01:05 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

/* WINDOW_UTILS */

void	click_object(void *param);
void	window_resize(int32_t width, int32_t height, void *param);
void	window_close(void *param);

static void	render_image(t_minirt *minirt)
{
	uint32_t	x;
	uint32_t	y;
	t_ray		ray;
	size_t		i;
	t_intersection	ix;

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
			color_to_rgb(trace_ray(&minirt->scene, ray, &ix),
				&minirt->image->pixels[i],
				&minirt->image->pixels[i + 1],
				&minirt->image->pixels[i + 2]);
			minirt->image->pixels[i + 3] = 255;
			x++;
		}
		y++;
	}
}

static void	render(t_minirt *minirt)
{
	double	before_render;

	before_render = mlx_get_time();
	render_image(minirt);
	printf("Rendered frame in %f seconds\n", mlx_get_time() - before_render);
}

static void	render_on_request(void *param)
{
	t_minirt	*minirt;

	minirt = param;
	if (minirt->loop_state == RENDER_NOW
		|| (minirt->loop_state == RESIZING
			&& mlx_get_time() - minirt->last_resize_time > 0.5))
	{
		render(minirt);
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
	minirt->mlx = mlx_init(scene->image_width,
			scene->image_height, "miniRT", true);
	if (!minirt->mlx)
	{
		printf("Failed to initialize MLX: %s\n", mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	minirt->image = mlx_new_image(minirt->mlx,
			scene->image_width, scene->image_height);
	if (!minirt->image)
	{
		mlx_close_window(minirt->mlx);
		printf("Failed to create image: %s\n", mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	render(minirt);
	if (mlx_image_to_window(minirt->mlx, minirt->image, 0, 0) == -1)
	{
		mlx_close_window(minirt->mlx);
		printf("Failed to draw image to window: %s\n", mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	mlx_loop_hook(minirt->mlx, click_object, minirt);
	mlx_loop_hook(minirt->mlx, key_press, minirt);
	mlx_loop_hook(minirt->mlx, render_on_request, minirt);
	mlx_resize_hook(minirt->mlx, window_resize, minirt);
	mlx_close_hook(minirt->mlx, window_close, minirt);
	mlx_loop(minirt->mlx);
	return (EXIT_SUCCESS);
}
