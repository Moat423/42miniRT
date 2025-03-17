/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:09:53 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/17 13:31:33 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <fcntl.h>
# include "../lib/mlx/include/MLX42/MLX42.h"
# include "../lib/libft/libft_full.h"

typedef	struct s_scene
{
	t_ambient	ambient;
	t_camera	camera;
	t_light		*lights;
	t_sphere	*spheres;
	t_plane		*planes;
	t_cylinder	*cylinders;
	size_t		light_count;
	size_t		sphere_count;
	size_t		plane_count;
	size_t		cylinder_count;
}	t_scene;

typedef struct s_vec3
{
	float	x;
	float	y;
	float	z;
}	t_vec3;

typedef t_vec3 t_color;

typedef struct s_ambient
{
	t_color	color;
	/// 0.0-1.0
	float	brightness;
}	t_ambient;

typedef struct s_camera
{
	t_vec3	pos;
	/// normalized
	t_vec3	dir;
	/// 0-180
	int		fov;
}	t_camera;

typedef struct s_light
{
	t_vec3	pos;
	t_color	color;
	/// 0.0-1.0
	float	brightness;
}	t_light;

typedef struct s_sphere
{
	t_vec3	pos;
	t_color	color;
	float	diameter;
}	t_sphere;

typedef struct s_plane
{
	t_vec3	pos;
	t_vec3	normal;
	t_color	color;
}	t_plane;

typedef struct s_cylinder
{
	t_vec3	pos;
	/// normalized
	t_vec3	axis;
	t_color	color;
	float	diameter;
	float	height;
}	t_cylinder;

#endif
