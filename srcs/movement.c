/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:19:53 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/22 10:11:53 by moat             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static bool	mouse_movement(t_camera *camera, t_coords delta,
								t_vec3 right_direction, t_vec3 up_direction)
{
	float	pitch_delta;
	float	yaw_delta;
	t_quat	pitch_quat;
	t_quat	yaw_quat;
	t_quat	combined_rotation;

	if (delta.x != 0 || delta.y != 0)
	{
		pitch_delta = -delta.y * SENSITIVITY;
		yaw_delta = -delta.x * SENSITIVITY;
		pitch_quat = quat_from_axis_angle(right_direction, pitch_delta);
		yaw_quat = quat_from_axis_angle(up_direction, yaw_delta);
		combined_rotation = quat_mul(yaw_quat, pitch_quat);
		combined_rotation = quat_normalize(combined_rotation);
		camera->dir = quat_rotate_vec3(combined_rotation, camera->dir);
		camera->dir = vec3_normalize(camera->dir);
		camera->right = vec3_cross(camera->dir, camera->up);
		camera->right = vec3_normalize(camera->right);
		return (true);
	}
	return (false);
}

static	bool	camera_movement(t_minirt *minirt,
								t_vec3 right_direction, t_vec3 up_direction)
{
	static t_coords	last = {-1, -1};
	static bool		was_mouse_down = false;
	t_coords		current;
	t_camera		*camera;
	bool			ret;

	camera = &minirt->scene.camera;
	if (!mlx_is_mouse_down(minirt->mlx, MLX_MOUSE_BUTTON_RIGHT))
	{
		was_mouse_down = false;
		return (false);
	}
	mlx_get_mouse_pos(minirt->mlx, &(current.x), &current.y);
	if (!was_mouse_down)
	{
		last.x = current.x;
		last.y = current.y;
		was_mouse_down = true;
		return (false);
	}
	ret = mouse_movement(camera, (t_coords){(current.x - last.x),
			(current.y - last.y)}, right_direction, up_direction);
	last.x = current.x;
	last.y = current.y;
	return (ret);
}

static void	movement(t_minirt *minirt)
{
	t_vec3		right_direction;
	t_vec3		up_direction;

	right_direction = vec3_cross(minirt->scene.camera.dir,
			minirt->scene.camera.up);
	up_direction = vec3_cross(minirt->scene.camera.dir,
			minirt->scene.camera.right);
	if (camera_movement(minirt, right_direction, up_direction))
		minirt->loop_state = DEFERRED_RENDER;
	if (key_movement(minirt, right_direction, up_direction))
		minirt->loop_state = DEFERRED_RENDER;
	else if (minirt->loop_state == DEFERRED_RENDER)
		minirt->loop_state = RENDER_NOW;
	if (mlx_is_key_down(minirt->mlx, MLX_KEY_ESCAPE))
	{
		printf("Escape key pressed, closing window\n");
		mlx_close_window(minirt->mlx);
	}
}

void	key_press(void *param)
{
	t_minirt	*minirt;

	minirt = param;
  if (!minirt->mlx)
    return ;
	movement(minirt);
}
