/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 14:35:51 by kwurster          #+#    #+#             */
/*   Updated: 2025/05/22 13:02:30 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

/* WINDOW_UTILS */

void	click_object(t_minirt *minirt);
void	window_resize(int32_t width, int32_t height, t_minirt *minirt);
void	movement(t_minirt *minirt);
void	window_init(t_minirt *minirt);
t_color	trace_ray(t_scene *scene, t_ray ray, t_intersection *out);
void	measure_render_time(t_minirt *minirt, double render_start);

static bool	next_row(t_minirt *minirt, uint32_t *next_row)
{
	pthread_mutex_lock(&minirt->render_y_mutex);
	if (minirt->render_y >= minirt->scene.image_height)
	{
		pthread_mutex_unlock(&minirt->render_y_mutex);
		return (false);
	}
	*next_row = minirt->render_y++;
	pthread_mutex_unlock(&minirt->render_y_mutex);
	return (true);
}

static void	*render_image(t_minirt *minirt)
{
	uint32_t		x;
	uint32_t		y;
	t_ray			ray;
	t_intersection	ix;
	size_t			i;

	y = 0;
	while ((minirt->mt && next_row(minirt, &y))
		|| (!minirt->mt && y < minirt->scene.image_height))
	{
		x = 0;
		while (x < minirt->scene.image_width)
		{
			i = (y * minirt->scene.image_width + x) * 4;
			ray = get_viewport_ray(&minirt->scene,
					(float)x / (float)minirt->scene.image_width,
					(float)y / (float)minirt->scene.image_height);
			color_to_rgb2(trace_ray(&minirt->scene, ray, &ix),
				&minirt->image->pixels[i]);
			x++;
		}
		if (!minirt->mt)
			y++;
	}
	return (NULL);
}

static void	render(t_minirt *minirt)
{
	double		before_render;
	pthread_t	render_threads[NUM_THREADS];
	uint32_t	i;

	before_render = mlx_get_time();
	minirt->render_y = 0;
	minirt->mt = !pthread_mutex_init(&minirt->render_y_mutex, 0);
	if (!minirt->mt)
		render_image(minirt);
	else
	{
		minirt->mt = true;
		i = 0;
		while (i < NUM_THREADS)
		{
			if (pthread_create(&render_threads[i], 0,
					(void *(*)(void *))render_image, minirt))
				break ;
			i++;
		}
		while (i > 0)
			pthread_join(render_threads[--i], 0);
		pthread_mutex_destroy(&minirt->render_y_mutex);
	}
	measure_render_time(minirt, before_render);
}

static void	render_on_request(t_minirt *minirt)
{
	if ((!minirt->deferred_render && minirt->loop_state == DEFERRED_RENDER)
		|| minirt->loop_state == RENDER_NOW
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
