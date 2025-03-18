/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:09:53 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/18 14:29:14 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

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

typedef struct s_vec3
{
	float	x;
	float	y;
	float	z;
}	t_vec3;

typedef t_vec3	t_color;

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

typedef struct s_ray
{
	t_vec3	origin;
	/// normalized
	t_vec3	direction;
}	t_ray;

typedef enum e_object_type
{
	SPHERE,
	PLANE,
	CYLINDER,
}	t_object_type;

/// When successfully intersecting a Ray with an Object we can return
/// a Intersection struct
typedef struct s_intersection
{
	t_vec3			point;
	t_vec3			normal;
	/// alias of the object from the scene which was intersected
	void			*object;
	t_object_type	type;
}	t_intersection;

typedef	struct s_scene
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

/* CAMERA */
t_camera	camera_new(t_vec3 pos, t_vec3 dir, uint32_t fov);
t_ray		get_viewport_ray(t_scene *scene, float u, float v);

/* VEC3 */

t_vec3	vec3_new(float x, float y, float z);
float	vec3_dot(t_vec3 v1, t_vec3 v2);
t_vec3	vec3_cross(t_vec3 v1, t_vec3 v2);
t_vec3	vec3_normalize(t_vec3 v);
t_vec3	vec3_subtract(t_vec3 v1, t_vec3 v2);
t_vec3	vec3_add(t_vec3 v1, t_vec3 v2);
t_vec3	vec3_multiply(t_vec3 v, float scalar);
t_vec3	vec3_component_mul(t_vec3 v1, t_vec3 v2);

/* COLOR */

t_color	color_new(float r, float g, float b);
void	color_to_rgb(t_color color, uint8_t *r, uint8_t *g, uint8_t *b);
t_color	color_from_rgb(uint8_t r, uint8_t g, uint8_t b);

/* UTIL */

float	clamp(float value, float min, float max);
void	scene_destroy(t_scene *scene);
float	image_aspect_ratio(t_scene *scene);

#endif
