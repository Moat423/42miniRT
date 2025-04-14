/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_types.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:38:25 by kwurster          #+#    #+#             */
/*   Updated: 2025/04/14 14:07:29 by lmeubrin         ###   ########.fr       */
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

# ifndef MATERIAL_COLOR // default object specular color as specular coefficient
#  define MATERIAL_COLOR (t_color){0.9, 0.9, 0.9}
# endif

# ifndef OBJ_NUM // number of different object-kinds we can parse in general
#  define OBJ_NUM 5
# endif

# ifndef SPEED // movement speed for keyboard motions
#  define SPEED 0.01f
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
	t_vec3	top;
	t_vec3	bottom;
	/// normalized
	t_vec3	axis;
	t_color	color;
	float	radius;
	float	height;
}	t_cylinder;

typedef struct s_cone
{
	t_vec3	top;
	t_vec3	bottom;
	/// normalized
	t_vec3	axis;
	t_color	color;
	float	slant;
	float	radius;
	float	height;
}	t_cone;

typedef struct s_ray
{
	t_vec3		origin;
	/// normalized
	t_vec3		direction;
	t_interval	range;
}	t_ray;

typedef struct s_circle
{
	t_vec3	center;
	t_vec3	normal;
	float	radius;
} t_circle;

typedef enum e_object_type
{
	SPHERE,
	PLANE,
	CYLINDER,
	LIGHT,
	CONE,
}	t_object_type;

enum e_hit_type
{
	FALSE,
	TRUE,
	CIRCLE
};
typedef struct s_object
{
	union {
		t_sphere	*sphere;
		t_plane		*plane;
		t_cylinder	*cylinder;
		t_cone		*cone;
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
	/// Whether normal was calculated already or still needs to be calculated
	/// In case it was not calculated, the normal field is filled with
	/// a vec that may be unnormalized, meaning it has to be normalized before use
	bool			normal_calculated;
}	t_intersection;

// struct used in cone calculation to prepare values for discriminant
// calculation and not double calculate these dot products:
// oc: vector origin to center of object
// d dot n: ray directiond dot normal of the object
// oc dot n: oc vector dot  normal of the object
// coeff: for cone: coeff = 1 + cone->slant * cone->slant;
// m: hit point projected onto the normal.
// 	m is the scalar that normal gets scaled by to reach the projected hit point
typedef struct s_calc
{
	t_vec3	oc;
	float	d_dot_n;
	float	oc_dot_n;
	float	coeff;
	float	height;
}	t_calc;

typedef bool	(*t_intersect_fn)(void *data, t_ray ray, t_intersection *out);

typedef struct s_scene
{
	t_camera	camera;
	t_ambient	ambient;
	t_light		*lights;
	t_sphere	*spheres;
	t_plane		*planes;
	t_cylinder	*cylinders;
	t_cone		*cones;
	size_t		light_count;
	size_t		sphere_count;
	size_t		plane_count;
	size_t		cylinder_count;
	size_t		cone_count;
	uint32_t	image_width;
	uint32_t	image_height;
}	t_scene;

typedef enum e_loop_state
{
	NO_ACTION,
	RESIZING,
	DEFERRED_RENDER,
	RENDER_NOW,
	NO_CHANGE,
}	t_loop_state;

typedef struct s_minirt
{
	t_scene			scene;
	/// must be initialized to null
	mlx_t			*mlx;
	mlx_image_t		*image;
	double			last_resize_time;
	t_loop_state	loop_state;
}	t_minirt;

typedef struct s_light_ray
{
	t_vec3	direction;
	float	distance;
	float	attenuation;
	float	lambert;
}	t_light_ray;

#endif
