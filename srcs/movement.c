/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:19:53 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/22 14:03:57 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static float limit_pitch_delta(t_vec3 dir, const float pitch_delta)
{
	float				current_pitch;
	float				new_pitch;
	static const float	max_pitch = M_PI / 2 * 0.99f;

	if (dir.y > 1.0)
		dir.y = 1.0;
	current_pitch = asinf(dir.y);
	new_pitch = current_pitch + pitch_delta;
	if (new_pitch > max_pitch)
		return (max_pitch - current_pitch);
	else if (new_pitch < -max_pitch)
		return (-max_pitch - current_pitch);
	return (pitch_delta);
}

static bool	mouse_movement(t_camera *camera, t_coords delta)
{
	float				pitch_delta;
	t_quat				pitch_quat;
	t_quat				yaw_quat;
	t_quat				combined_rotation;
	static const t_vec3	world_up = {0, 1, 0};

	if (delta.x != 0 || delta.y != 0)
	{
		pitch_delta = -delta.y * SENSITIVITY;
		pitch_delta = limit_pitch_delta(camera->dir, pitch_delta);
		pitch_quat = quat_from_axis_angle(camera->right, pitch_delta);
		yaw_quat = quat_from_axis_angle(world_up, -delta.x * SENSITIVITY);
		combined_rotation = quat_normalize(quat_mul(yaw_quat, pitch_quat));
		camera->dir = vec3_normalize(
				quat_rotate_vec3(combined_rotation, camera->dir));
		camera->right = vec3_normalize(vec3_cross(camera->dir, world_up));
		camera->up = vec3_normalize(vec3_cross(camera->right, camera->dir));
		return (true);
	}
	return (false);
}

static	bool	camera_movement(t_minirt *minirt)
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
			(current.y - last.y)});
	last.x = current.x;
	last.y = current.y;
	return (ret);
}

static void	movement(t_minirt *minirt)
{
	if (camera_movement(minirt))
		minirt->loop_state = DEFERRED_RENDER;
	if (key_movement(minirt))
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
	movement(minirt);
}
