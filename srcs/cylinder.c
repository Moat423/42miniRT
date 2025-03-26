/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 18:38:32 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/03/26 11:57:29 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

bool	circle_intersect(t_circle circle, t_ray ray, float *out_dist, t_vec3 *out_point);
bool	closer_circle_intersect(t_cylinder *cylinder, t_ray ray, float *o_dist, t_vec3 *o_pt);

t_vec3	cyl_surface_normal(t_cylinder *cylinder, t_vec3 point, float m)
{
	return (vec3_normalize(vec3_subtract(point, vec3_add(cylinder->bottom, vec3_multiply(cylinder->axis, m)))));
}

/// returns the projection of vector v on the axis of the cylinder
/// also known as "m"
/// if the resulting float value is between 0 and height, the point is on the cylinder body
float	projected_len_on_axis(t_cylinder *cyl, t_vec3 v)
{
	return (vec3_dot(cyl->axis, vec3_subtract(v, cyl->bottom)));
}

bool	cylinder_intersect(t_cylinder *cylinder, t_ray ray, t_intersection *out)
{
	t_vec3	oc;
	float	abc[3];
	float	discriminant;
	float	sqrt_d;
	float	t[4];
	t_vec3	hit_point;
	float	hit_proj; //projection of hit_point along axis
	int		i;

	i = 0;
	oc = vec3_subtract(ray.origin, cylinder->pos);
	abc[A] = vec3_squared_length(ray.direction) - vec3_dot(ray.direction, cylinder->axis) * vec3_dot(ray.direction, cylinder->axis);
	abc[C] = vec3_squared_length(oc) - (vec3_dot(oc, cylinder->axis) * vec3_dot(oc, cylinder->axis)) - cylinder->radius * cylinder->radius;
	abc[B] = 2 * (vec3_dot(ray.direction, oc) - vec3_dot(ray.direction, cylinder->axis) * vec3_dot(oc, cylinder->axis));
	discriminant = abc[B] * abc[B] - 4 * abc[A] * abc[C];
	out->object = (t_object){.cylinder = cylinder, .type = CYLINDER};
	out->normal = cylinder->axis;
	if (discriminant < EPSILON && discriminant > -EPSILON)
		return (closer_circle_intersect(cylinder, ray, &(out->distance), &(out->point)));
	if (discriminant < 0)
		return (false);
	sqrt_d = sqrtf(discriminant);
	t[0] = (-abc[B] - sqrt_d) / (2 * abc[A]);
	t[1] = (-abc[B] + sqrt_d) / (2 * abc[A]);
	// cylinder body finite range lock
	if (interval_contains(ray.range, t[0]))
	{
		hit_point = vec3_add(ray.origin, vec3_multiply(ray.direction, t[0]));
		hit_proj = projected_len_on_axis(cylinder, hit_point);
		if (hit_proj >= 0 && hit_proj <= cylinder->height)
		{
			out->distance = t[0];
			out->point = hit_point;
			out->object = (t_object){.cylinder = cylinder, .type = CYLINDER};
			out->normal = cyl_surface_normal(cylinder, hit_point, hit_proj);
			return (true);
		}
	}
	if (!interval_contains(ray.range, t[1]))
		return (closer_circle_intersect(cylinder, ray, &(out->distance), &(out->point)));
	hit_point = vec3_add(ray.origin, vec3_multiply(ray.direction, t[1]));
	hit_proj = projected_len_on_axis(cylinder, hit_point);
	if (hit_proj >= 0 && hit_proj <= cylinder->height)
	{
		// circle true hit
		if (closer_circle_intersect(cylinder, ray, &t[0], &(out->point)))
		{
			if (t[0] < t[1])
			{
				out->distance = t[0];
				return (true);
			}
		}
	}
	else
		return (closer_circle_intersect(cylinder, ray, &(out->distance), &(out->point)));
	out->distance = t[1];
	out->point = hit_point;
	out->normal = cyl_surface_normal(cylinder, hit_point, hit_proj);
	return (true);
}

bool	closer_circle_intersect(t_cylinder *cylinder, t_ray ray, float *o_dist, t_vec3 *o_pt)
{
	t_vec3	hit_point[2];
	float	hit_dist[2];

	// if (vec3_squared_length(vec3_subtract(cylinder->top, ray.origin)) <= vec3_squared_length(vec3_subtract(cylinder->bottom, ray.origin)))
	if (!circle_intersect((t_circle){cylinder->top, cylinder->axis, cylinder->radius}, ray, &(hit_dist[0]), &(hit_point[0])))
		return (circle_intersect((t_circle){cylinder->bottom, cylinder->axis, cylinder->radius}, ray, o_dist, o_pt));
	if (!circle_intersect((t_circle){cylinder->bottom, cylinder->axis, cylinder->radius}, ray, &(hit_dist[1]), &(hit_point[1])))
	{
		*o_dist = hit_dist[0];
		*o_pt = hit_point[0];
		return (true);
	}
	if (hit_dist[0] < hit_dist[1])
	{
		*o_dist = hit_dist[0];
		*o_pt = hit_point[0];
		return (true);
	}
	*o_dist = hit_dist[1];
	*o_pt = hit_point[1];
	return (true);
}

bool	circle_intersect(t_circle circle, t_ray ray, float *o_dist, t_vec3 *o_pt)
{
	float	t;
	float	denom;
	t_vec3	hit_point;

	denom = vec3_dot(circle.normal, ray.direction);
	if (denom < EPSILON && denom > -EPSILON)
		return (false);
	t = vec3_dot(vec3_subtract(circle.pos, ray.origin), circle.normal)
		/ denom;
	if (!interval_contains(ray.range, t))
		return (false);
	hit_point = vec3_add(ray.origin, vec3_multiply(ray.direction, t));
	// the (1.0f + EPSILON) is to account for floating point errors, can be taken out
	if (vec3_squared_length(vec3_subtract(hit_point, circle.pos)) <=
		circle.radius * circle.radius * (1.0f + EPSILON))
	{
		*o_dist = t;
		*o_pt = hit_point;
		return (true);
	}
	return (false);
}
