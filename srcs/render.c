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

void	click_object(t_minirt *minirt);
void	window_resize(int32_t width, int32_t height, t_minirt *minirt);
void	movement(t_minirt *minirt);
void	window_init(t_minirt *minirt);

t_color	trace_ray(t_scene *scene, t_ray ray, t_intersection *out)
{
	if (find_closest_intersection(scene, ray, out))
		return (color_clamp(shade(scene, ray, *out)));
	return (color_new(0, 0, 0));
}

static void	render_image(t_minirt *minirt)
{
	uint32_t		x;
	uint32_t		y;
	t_ray			ray;
	size_t			i;
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

static void	render_on_request(t_minirt *minirt)
{
	if (minirt->loop_state == RENDER_NOW
		|| (minirt->loop_state == RESIZING
			&& mlx_get_time() - minirt->last_resize_time > 0.5))
	{
		render(minirt);
		minirt->loop_state = NO_ACTION;
	}
}

int	render_loop(t_minirt *minirt)
{
	window_init(minirt);
	render(minirt);
	mlx_loop_hook(minirt->mlx, (t_param_fn) click_object, minirt);
	mlx_loop_hook(minirt->mlx, (t_param_fn) movement, minirt);
	mlx_loop_hook(minirt->mlx, (t_param_fn) render_on_request, minirt);
	mlx_resize_hook(minirt->mlx, (mlx_resizefunc) window_resize, minirt);
	mlx_loop(minirt->mlx);
	return (EXIT_SUCCESS);
}
