/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:19:53 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/14 14:16:59 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static t_vec3	update_camera_position(t_camera camera,
										t_vec3 dir, const float speed)
{
	return (vec3_add(camera.pos, vec3_multiply(dir, speed)));
}

static bool	main_key_movement(t_minirt *minirt, t_vec3 right_direction,
							t_vec3 up_direction)
{
	t_camera	*camera;

	camera = &minirt->scene.camera;
	if (mlx_is_key_down(minirt->mlx, MLX_KEY_S)
		|| mlx_is_key_down(minirt->mlx, MLX_KEY_DOWN))
		camera->pos = update_camera_position(*camera, camera->dir, -SPEED);
	else if (mlx_is_key_down(minirt->mlx, MLX_KEY_W)
		|| mlx_is_key_down(minirt->mlx, MLX_KEY_UP))
		camera->pos = update_camera_position(*camera, camera->dir, SPEED);
	else if (mlx_is_key_down(minirt->mlx, MLX_KEY_A)
		|| mlx_is_key_down(minirt->mlx, MLX_KEY_LEFT))
		camera->pos = update_camera_position(*camera, right_direction, -SPEED);
	else if (mlx_is_key_down(minirt->mlx, MLX_KEY_D)
		|| mlx_is_key_down(minirt->mlx, MLX_KEY_RIGHT))
		camera->pos = update_camera_position(*camera, right_direction, SPEED);
	else
		return (false);
	return (true);
}

static bool	key_movement(t_minirt *minirt)
{
	t_camera	*camera;
	t_vec3		right_direction;
	t_vec3		up_direction;

	camera = &minirt->scene.camera;
	right_direction = vec3_cross(camera->dir, camera->up);
	up_direction = vec3_cross(camera->dir, camera->right);
	if (main_key_movement(minirt, right_direction, up_direction))
		return (true);
	else if (mlx_is_key_down(minirt->mlx, MLX_KEY_Q))
		camera->pos = update_camera_position(*camera, up_direction, -SPEED);
	else if (mlx_is_key_down(minirt->mlx, MLX_KEY_E))
		camera->pos = update_camera_position(*camera, up_direction, SPEED);
	else
		return (false);
	return (true);
}

static void	movement(t_minirt *minirt)
{
	t_scene		*scene;

	scene = &minirt->scene;
	if (key_movement(minirt))
		minirt->loop_state = DEFERRED_RENDER;
	else if (minirt->loop_state == DEFERRED_RENDER)
		minirt->loop_state = RENDER_NOW;
}

void	key_press(void *param)
{
	t_minirt	*minirt;

	minirt = param;
	if (mlx_is_key_down(minirt->mlx, MLX_KEY_ESCAPE))
	{
		printf("Escape key pressed, closing window\n");
		mlx_close_window(minirt->mlx);
	}
	movement(minirt);
}
