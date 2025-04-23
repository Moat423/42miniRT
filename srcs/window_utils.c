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

void	window_image_init(t_minirt *minirt)
{
	minirt->image = mlx_new_image(minirt->mlx,
			minirt->scene.image_width, minirt->scene.image_height);
	if (!minirt->image)
	{
		printf("Failed to create image: %s\n", mlx_strerror(mlx_errno));
		minirt_exit(minirt, EXIT_FAILURE);
	}
	if (mlx_image_to_window(minirt->mlx, minirt->image, 0, 0) == -1)
	{
		printf("Failed to draw image to window: %s\n", mlx_strerror(mlx_errno));
		minirt_exit(minirt, EXIT_FAILURE);
	}
}

void	window_init(t_minirt *minirt)
{
	t_scene	*scene;

	scene = &minirt->scene;
	minirt->mlx = mlx_init(scene->image_width,
			scene->image_height, "miniRT", true);
	if (!minirt->mlx)
	{
		printf("Failed to initialize MLX: %s\n", mlx_strerror(mlx_errno));
		minirt_exit(minirt, EXIT_FAILURE);
	}
	window_image_init(minirt);
}

static void	print_intersect_at_screen_coord(t_minirt *minirt,
	uint32_t x, uint32_t y)
{
	t_intersection	ix;
	t_ray			ray;

	ray = get_viewport_ray(&minirt->scene,
			(float) x / (float) minirt->scene.image_width,
			(float) y / (float) minirt->scene.image_height);
	trace_ray(&minirt->scene, ray, &ix);
	if (ix.object.any == NULL)
	{
		printf("No intersection found\n");
		return ;
	}
	printf("---------------\n");
	printf("pixel at %d %d\n", x, y);
	printf("cam: %f %f %f\n", ray.origin.x, ray.origin.y, ray.origin.z);
	printf("distance: %f\n", ix.distance);
	printf("intersection: %f %f %f\n", ix.point.x, ix.point.y, ix.point.z);
	printf("object type: %d\n", ix.object.type);
	printf("surface normal: %f %f %f\n", intersect_normal(&ix).x,
		intersect_normal(&ix).y,
		intersect_normal(&ix).z);
	printf("---------------\n");
}

void	click_object(t_minirt *minirt)
{
	static int		x[2] = {0, 0};
	static int		y[2] = {0, 0};
	static float	last_cam_pos[2] = {-INFINITY, -INFINITY};

	if (mlx_is_mouse_down(minirt->mlx, MLX_MOUSE_BUTTON_LEFT))
	{
		mlx_get_mouse_pos(minirt->mlx, x, y);
		last_cam_pos[0] = vec3_squared_length(vec3_add(minirt->scene.camera.pos,
					minirt->scene.camera.dir));
		if (x[0] != x[1] || y[0] != y[1] || last_cam_pos[1] != last_cam_pos[0])
			print_intersect_at_screen_coord(minirt, x[0], y[0]);
		x[1] = x[0];
		y[1] = y[0];
		last_cam_pos[1] = last_cam_pos[0];
	}
}

void	window_resize(int32_t width, int32_t height, t_minirt *minirt)
{
	if (minirt->image && minirt->mlx)
	{
		minirt->scene.image_width = width;
		minirt->scene.image_height = height;
		mlx_delete_image(minirt->mlx, minirt->image);
		window_image_init(minirt);
		minirt->loop_state = RESIZING;
		minirt->last_resize_time = mlx_get_time();
		printf("Resized window to %dx%d\n", width, height);
	}
}
