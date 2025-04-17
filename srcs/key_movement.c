/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_movement.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:10:13 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/15 16:11:22 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static t_vec3	update_camera_position(t_camera camera,
										t_vec3 dir, const float speed)
{
	return (vec3_add(camera.pos, vec3_multiply(dir, speed)));
}

static bool	main_key_movement(t_minirt *minirt, t_vec3 right_direction)
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

bool	key_movement(t_minirt *minirt)
{
	t_camera	*camera;

	camera = &minirt->scene.camera;
	if (main_key_movement(minirt, minirt->scene.camera.right))
		return (true);
	else if (mlx_is_key_down(minirt->mlx, MLX_KEY_Q))
		camera->pos = update_camera_position(*camera, minirt->scene.camera.up, -SPEED);
	else if (mlx_is_key_down(minirt->mlx, MLX_KEY_E))
		camera->pos = update_camera_position(*camera, minirt->scene.camera.up, SPEED);
	else
		return (false);
	return (true);
}
