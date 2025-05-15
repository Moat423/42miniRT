/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_types.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:38:25 by kwurster          #+#    #+#             */
/*   Updated: 2025/05/15 15:50:19 by lmeubrin         ###   ########.fr       */
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

# ifndef EPSILON
/// tolerance value for floating point comparisons
#  define EPSILON 0.0001
# endif

# ifndef OBJ_NUM
/// number of different object-kinds we can parse in general
#  define OBJ_NUM 5
# endif

# ifndef SPEED // movement speed for keyboard motions
#  define SPEED 0.01f
# endif

# ifndef SENSITIVITY // mouse movement SENSITIVITY
#  define SENSITIVITY 0.002f
# endif

# ifndef BONUS
// BONUS compile flag to limit lights
#  define MAX_LIGHTS 1
# else
#  define MAX_LIGHTS UINT_MAX
# endif

# ifndef SHININESS
/// a higher value makes the material more metallic
/// -> the specular reflection gets more concentrated at one spot
#  define SHININESS 32
# endif

# ifndef LIGHT_DIST
/// a higher value will make lights shine further
#  define LIGHT_DIST 30.0f
# endif

# ifndef BUMP_STRENGTH
#  define BUMP_STRENGTH 3
# endif //BUMP_STRENGTH

# define _USE_MATH_DEFINES

typedef void			(*t_param_fn)(void *);

typedef struct s_light	t_light;

typedef enum e_abc
{
	A,
	B,
	C,
}	t_abc;

typedef enum e_texture
{
	NONE,
	CHECKERS,
}	t_texture;

typedef struct e_bumpmap
{
	float	*elevation;
	size_t	width;
	size_t	height;
}	t_bumpmap;

typedef struct s_vec3
{
	float	x;
	float	y;
	float	z;
}	t_vec3;

//obj_colouring can be a pointer to t_checkers
//or a pointer to mlx_texture_t depending on the texture type
typedef struct e_texturing
{
	void		*obj_colouring;
	t_bumpmap	*bumpmap;
	t_texture	type;
} t_texturing;

typedef t_vec3			t_color;

typedef struct s_aabb
{
	t_vec3	min;
	t_vec3	max;
}	t_aabb;

typedef struct s_quat
{
	float	w;
	float	x;
	float	y;
	float	z;
}	t_quat;

typedef struct s_coords
{
	int32_t	x;
	int32_t	y;
}	t_coords;

// interval used for cutting of view distance or
// ephemeral to describe any interval
typedef struct s_interval
{
	float	min;
	float	max;
}	t_interval;

typedef struct s_point
{
	float	u;
	float	v;
}	t_point;

// describes ambient light parsed from file
// brightness: in range 0.0-1.0
typedef struct s_ambient
{
	t_color	color;
	float	brightness;
}	t_ambient;

// camera parsed from file
//pos: postition of camera in 3d sapce as coords saved in vec3
//dir: normalized
//up: normalized up direction
//right: normalized right direction
// fov: 0-180
typedef struct s_camera
{
	t_vec3		pos;
	t_vec3		dir;
	t_vec3		up;
	t_vec3		right;
	uint32_t	fov;
}	t_camera;

typedef struct s_sphere
{
	t_vec3		pos;
	t_color		color;
	t_light		**lights;
	float		radius;
	t_texture	texture;
	t_bumpmap	*bumpmap;
}	t_sphere;

typedef struct s_plane
{
	t_vec3		pos;
	t_vec3		normal;
	t_color		color;
	t_texture	texture;
}	t_plane;

// expects the axis to be normalized
typedef struct s_cylinder
{
	t_vec3	pos;
	t_vec3	top;
	t_vec3	bottom;
	t_vec3	axis;
	t_color	color;
	t_light	**lights;
	float	radius;
	float	height;
}	t_cylinder;

// expects the axis to be normalized
typedef struct s_cone
{
	t_vec3	top;
	t_vec3	bottom;
	t_vec3	axis;
	t_color	color;
	t_light	**lights;
	float	slant;
	float	radius;
	float	height;
}	t_cone;

typedef struct s_objects
{
	union {
		struct {
			t_sphere	*spheres;
			t_plane		*planes;
			t_cylinder	*cylinders;
			t_cone		*cones;
		};
		void			*objs[4];
	};
	union {
		struct {
			size_t		sphere_count;
			size_t		plane_count;
			size_t		cylinder_count;
			size_t		cone_count;
		};
		size_t			objs_n[4];
	};
}	t_objects;

/// Objects here are scene objects copied by value,
/// be careful about double free!!
/// brightness in between 0.0-1.0
typedef struct s_light
{
	t_objects	objs;
	t_vec3		pos;
	t_color		color;
	float		brightness;
}	t_light;

// describes a ray
/// expects direction to be normalized
typedef struct s_ray
{
	t_vec3		origin;
	t_vec3		direction;
	t_interval	range;
}	t_ray;

typedef struct s_circle
{
	t_vec3	center;
	t_vec3	normal;
	float	radius;
}	t_circle;

typedef enum e_object_type
{
	SPHERE,
	PLANE,
	CYLINDER,
	CONE,
	LIGHT,
}	t_object_type;

typedef enum e_hit_type
{
	X_FALSE,
	X_TRUE,
	X_CIRCLE
}	t_hit_type;

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
}	t_object;

/// When successfully intersecting a Ray with an Object we can return
/// a Intersection struct
/// object: alias of the object from the scene which was intersected
/// normal_calculated:
/// Whether normal was calculated already or still needs to be calculated
/// In case it was not calculated, the normal field is filled with
/// a vec that may be unnormalized, meaning it has to be normalized before use
typedef struct s_intersection
{
	t_vec3			point;
	t_vec3			normal;
	t_object		object;
	float			distance;
	bool			normal_calculated;
	t_color			color;
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

typedef bool			(*t_intersect_fn)(void *data,
						t_ray ray, t_intersection *out);

typedef struct s_scene
{
	t_objects	objs;
	t_camera	camera;
	t_ambient	ambient;
	t_light		*lights;
	size_t		light_count;
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

/// scene and mlx must be initialized to null
typedef struct s_minirt
{
	t_scene			scene;
	mlx_t			*mlx;
	mlx_image_t		*image;
	double			last_resize_time;
	int32_t			mouse_pos_x;
	int32_t			mouse_pos_y;
	t_loop_state	loop_state;
}	t_minirt;

typedef struct s_light_ray
{
	t_vec3	direction;
	float	distance;
	float	attenuation;
	float	lambert;
}	t_light_ray;

typedef struct s_checkers
{
	float	width;
	float	height;
	t_color	color_a;
	t_color	color_b;
}	t_checkers;

#endif
