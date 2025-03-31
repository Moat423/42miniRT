/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:09:53 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/31 14:22:39 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "miniRT_types.h"

/* RENDER */

int		render_loop(t_minirt *minirt);

/* TRACE_RAY */

t_color	trace_ray(t_scene *scene, t_ray ray);

/* SHADING */

t_color	shade(t_scene *scene, t_ray ray, t_intersection intersection);
t_color	specular(t_light light, t_vec3 light_dir, t_vec3 normal, t_vec3 diffuse_color, float lambertian, float distance);

/* INTERSECT */

bool	find_closest_intersection(t_scene *scene, t_ray ray, t_intersection *closest);
t_vec3	intersect_normal(t_intersection *intersection);

/* SPHERE */

bool	sphere_intersect(t_sphere *sphere, t_ray ray, t_intersection *out);

/* CYLINDER */

bool	cylinder_intersect(t_cylinder *cylinder, t_ray ray, t_intersection *out);

/* PLANE */

bool	plane_intersect(t_plane *plane, t_ray ray, t_intersection *out);

/* CIRCLE */

bool	circle_intersect(t_circle circle, t_ray ray, float *out_dist, t_vec3 *out_point);
bool	closer_circle_intersect(t_cylinder *cylinder, t_ray ray, float *o_dist, t_vec3 *o_pt);

/* CONE */

int		set_cone(char *line, t_cone *cone);

/* OBJECT */

t_color	object_color(t_object object);

/* CAMERA */

t_camera	camera_new(t_vec3 pos, t_vec3 dir, uint32_t fov);
t_ray		get_viewport_ray(t_scene *scene, float u, float v);

/* VEC3 */

t_vec3	vec3_new(float x, float y, float z);
float	vec3_dot(t_vec3 v1, t_vec3 v2);
float	vec3_squared_length(t_vec3 v);
t_vec3	vec3_cross(t_vec3 v1, t_vec3 v2);
t_vec3	vec3_normalize(t_vec3 v);
t_vec3	vec3_subtract(t_vec3 v1, t_vec3 v2);
t_vec3	vec3_add(t_vec3 v1, t_vec3 v2);
t_vec3	vec3_multiply(t_vec3 v, float scalar);
t_vec3	vec3_component_mul(t_vec3 v1, t_vec3 v2);
t_vec3	vec3_divide(t_vec3 v, float scalar);
t_vec3	vec3_calc_length_and_normalize(t_vec3 v, float *length);
t_vec3	vec3_pow(t_vec3 v, int scalar);

/* COLOR */

t_color	color_new(float r, float g, float b);
void	color_to_rgb(t_color color, uint8_t *r, uint8_t *g, uint8_t *b);
t_color	color_from_rgb(uint8_t r, uint8_t g, uint8_t b);
t_color	color_clamp(t_color color);
t_color	vec3_to_color(t_vec3 vec);

/* LIGHT */

t_color	calc_lights(const t_light light, const t_ray ray, t_intersection its, t_light_ray l);

/* INTERVAL */

t_interval	interval_new(float min, float max);
bool		interval_contains(t_interval interval, float x);
bool		interval_surrounds(t_interval interval, float x);

/* UTIL */

float	clamp(float value, float min, float max);
void	scene_destroy(t_scene *scene);
float	image_aspect_ratio(t_scene *scene);
bool	equal(float a, float b);
float	powi(float x, int z);

/*OBJS */

int	ft_malloc_scene_arrays(t_scene *scene);

/*PRINT_SCENE*/

void print_scene(const t_scene *scene);

#endif
