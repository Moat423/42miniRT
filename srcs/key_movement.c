/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_movement.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:10:13 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/22 12:51:20 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static t_vec3	update_camera_position(t_camera camera,
										t_vec3 dir, const float speed)
{
	return (vec3_add(camera.pos, vec3_multiply(dir, speed)));
}

static void	main_key_movement(t_minirt *minirt, t_vec3 right_direction,
		float speed)
{
	t_camera	*camera;

	camera = &minirt->scene.camera;
	if (mlx_is_key_down(minirt->mlx, MLX_KEY_S)
		|| mlx_is_key_down(minirt->mlx, MLX_KEY_DOWN))
		camera->pos = update_camera_position(*camera, camera->dir,
				-speed);
	if (mlx_is_key_down(minirt->mlx, MLX_KEY_W)
		|| mlx_is_key_down(minirt->mlx, MLX_KEY_UP))
		camera->pos = update_camera_position(*camera, camera->dir,
				speed);
	if (mlx_is_key_down(minirt->mlx, MLX_KEY_A)
		|| mlx_is_key_down(minirt->mlx, MLX_KEY_LEFT))
		camera->pos = update_camera_position(*camera, right_direction,
				-speed);
	if (mlx_is_key_down(minirt->mlx, MLX_KEY_D)
		|| mlx_is_key_down(minirt->mlx, MLX_KEY_RIGHT))
		camera->pos = update_camera_position(*camera, right_direction,
				speed);
}

bool	key_movement(t_minirt *minirt)
{
	t_camera	*camera;
	float		speed;
	t_vec3		initial_pos;

	speed = SPEED * minirt->render_time;
	if (mlx_is_key_down(minirt->mlx, MLX_KEY_LEFT_SHIFT))
		speed *= 5;
	camera = &minirt->scene.camera;
	initial_pos = camera->pos;
	main_key_movement(minirt, minirt->scene.camera.right, speed);
	if (mlx_is_key_down(minirt->mlx, MLX_KEY_Q))
		camera->pos = update_camera_position(*camera,
				minirt->scene.camera.up, -speed);
	if (mlx_is_key_down(minirt->mlx, MLX_KEY_E))
		camera->pos = update_camera_position(*camera,
				minirt->scene.camera.up, speed);
	return (ft_memcmp(&initial_pos, &camera->pos, sizeof(t_vec3)) != 0);
}
