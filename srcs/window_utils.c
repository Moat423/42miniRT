/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:35:58 by kwurster          #+#    #+#             */
/*   Updated: 2025/04/22 15:59:42 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static void	print_intersect_at_screen_coord(void *param, uint32_t x, uint32_t y)
{
	t_minirt		*minirt;
	t_intersection	ix;
	t_ray			ray;

	minirt = param;
	ray = get_viewport_ray(&minirt->scene,
			(float) x / (float) minirt->scene.image_width,
			(float) y / (float) minirt->scene.image_height);
	if (vec3_squared_length(trace_ray(&minirt->scene, ray, &ix)) == 0)
	{
		printf("No intersection found\n");
		return ;
	}
	printf("---------------\n");
	printf("Screen Coordinate: %d %d\n", x, y);
	printf("Ray origin: %f %f %f\n", ray.origin.x, ray.origin.y, ray.origin.z);
	printf("Distance: %f\n", ix.distance);
	printf("Point: %f %f %f\n", ix.point.x, ix.point.y, ix.point.z);
	printf("Object type: %d\n", ix.object.type);
	printf("Normal: %f %f %f\n", intersect_normal(&ix).x,
		intersect_normal(&ix).y,
		intersect_normal(&ix).z);
	printf("---------------\n");
}

void	click_object(void *param)
{
	t_minirt	*minirt;
	int			x;
	int			y;

	minirt = param;
	if (mlx_is_mouse_down(minirt->mlx, MLX_MOUSE_BUTTON_LEFT))
	{
		mlx_get_mouse_pos(minirt->mlx, &x, &y);
		print_intersect_at_screen_coord(param, x, y);
	}
}

void	window_resize(int32_t width, int32_t height, void *param)
{
	t_minirt	*minirt;

	minirt = param;
	if (minirt->image && minirt->mlx)
	{
		minirt->scene.image_width = width;
		minirt->scene.image_height = height;
		// TODO: error handling like in render_loop
		mlx_delete_image(minirt->mlx, minirt->image);
		minirt->image = mlx_new_image(minirt->mlx,
				minirt->scene.image_width, minirt->scene.image_height);
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
