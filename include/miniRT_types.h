/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_types.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:38:25 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/25 17:05:41 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_TYPES_H
# define MINIRT_TYPES_H

# include <stddef.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <fcntl.h>
# include <stdbool.h>
# include <stdint.h>
# include <errno.h>
# include <string.h>
# include "../lib/mlx/include/MLX42/MLX42.h"
# include "../lib/libft/libft_full.h"

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

# ifndef EPSILON // tollerance value for floating point comparisons
#  define EPSILON 0.0001
# endif

typedef enum e_abc
{
	A,
	B,
	C,
}	t_abc;

typedef struct s_vec3
{
	float	x;
	float	y;
	float	z;
}	t_vec3;

typedef t_vec3	t_color;

typedef struct s_interval
{
	float	min;
	float	max;
}	t_interval;

typedef struct s_ambient
{
	t_color	color;
	/// 0.0-1.0
	float	brightness;
}	t_ambient;

typedef struct s_camera
{
	t_vec3		pos;
	/// normalized
	t_vec3		dir;
	/// normalized up direction
	t_vec3		up;
	/// normalized right direction
	t_vec3		right;
	/// 0-180
	uint32_t	fov;
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
	float	radius;
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
	float	radius;
	float	height;
	t_vec3	top;
	t_vec3	bottom;
}	t_cylinder;

typedef struct s_ray
{
	t_vec3		origin;
	/// normalized
	t_vec3		direction;
	t_interval	range;
}	t_ray;

typedef struct s_circle
{
	t_vec3	pos;
	t_vec3	normal;
	float	radius;
} t_circle;

typedef enum e_object_type
{
	SPHERE,
	PLANE,
	CYLINDER,
	LIGHT,
}	t_object_type;

typedef struct s_object
{
	union {
		t_sphere	*sphere;
		t_plane		*plane;
		t_cylinder	*cylinder;
		void		*any;
	};
	t_object_type	type;
	//intersect_fn	intersect_fn;
}	t_object;

/// When successfully intersecting a Ray with an Object we can return
/// a Intersection struct
typedef struct s_intersection
{
	t_vec3			point;
	t_vec3			normal;
	/// alias of the object from the scene which was intersected
	t_object		object;
	float			distance;
}	t_intersection;

typedef bool	(*t_intersect_fn)(void *data, t_ray ray, t_intersection *out);

typedef struct s_scene
{
	t_camera	camera;
	t_ambient	ambient;
	t_light		*lights;
	t_sphere	*spheres;
	t_plane		*planes;
	t_cylinder	*cylinders;
	size_t		light_count;
	size_t		sphere_count;
	size_t		plane_count;
	size_t		cylinder_count;
	uint32_t	image_width;
	uint32_t	image_height;
}	t_scene;

typedef struct s_minirt
{
	t_scene	scene;
	/// must be initialized to null
	mlx_t		*mlx;
	mlx_image_t	*image;
	double		last_render_request_time;
}	t_minirt;

#endif
