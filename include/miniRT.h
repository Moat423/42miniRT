/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:09:53 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/19 14:20:21 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "miniRT_types.h"

/* RENDER */

int		render_loop(t_minirt *minirt);
t_color	trace_ray(t_scene *scene, t_ray ray);

/* INTERSECT */

t_intersection	find_closest_intersection(t_scene *scene, t_ray ray);
t_intersection	sphere_intersect(t_sphere *sphere, t_ray ray);

/* SURFACE NORMAL */

t_vec3	sphere_normal(t_sphere sphere, t_vec3 point);

/* OBJECT */

t_color	object_color(t_object object);
t_vec3	object_normal(t_object object, t_vec3 point);

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

/* COLOR */

t_color	color_new(float r, float g, float b);
void	color_to_rgb(t_color color, uint8_t *r, uint8_t *g, uint8_t *b);
t_color	color_from_rgb(uint8_t r, uint8_t g, uint8_t b);

/* UTIL */

float	clamp(float value, float min, float max);
void	scene_destroy(t_scene *scene);
float	image_aspect_ratio(t_scene *scene);

#endif
