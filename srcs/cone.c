/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:28:02 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/22 15:41:28 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

// gets discriminant of cone
// the axis is normalized and therefore the normal of the cone
// coeff: 1 + slant * slant
// a = (d dot d) - (1 + slant * slant) * (d dot axis)^2
// b = root((d dot oc) - (1 + slant * slant) * (d dot axis) (oc dot axis)
// c = (oc dot oc) - (1 + slant * slant) * (oc dot axis)^2
static	float	get_discriminant(const t_vec3 ray_dir, const t_calc cc,
							float abc[3])
{
	abc[A] = vec3_squared_length(ray_dir) - cc.coeff * cc.d_dot_n * cc.d_dot_n;
	abc[C] = vec3_squared_length(cc.oc) - cc.coeff * cc.oc_dot_n * cc.oc_dot_n;
	abc[B] = 2 * (vec3_dot(ray_dir, cc.oc)
			- cc.coeff * cc.d_dot_n * cc.oc_dot_n);
	return (abc[B] * abc[B] - 4 * abc[A] * abc[C]);
}

static t_calc	prep_cone_calc(t_ray ray, const t_cone *cone)
{
	t_calc	cc;

	cc.oc = vec3_subtract(ray.origin, cone->top);
	cc.d_dot_n = vec3_dot(ray.direction, cone->axis);
	cc.oc_dot_n = vec3_dot(cc.oc, cone->axis);
	cc.coeff = 1 + cone->slant * cone->slant;
	cc.height = cone->height;
	return (cc);
}

static bool	cone_calc(const t_cone *cone, const t_ray ray,
						float t[2], t_calc *cc)
{
	float		abc[3];
	float		discriminant;
	float		sqrt_d;

	*cc = prep_cone_calc(ray, cone);
	discriminant = get_discriminant(ray.direction, *cc, abc);
	if (discriminant < 0 || (fabs(abc[C]) < EPSILON))
		return (false);
	sqrt_d = sqrtf(discriminant);
	t[0] = (-abc[B] - sqrt_d) / (2 * abc[A]);
	t[1] = (-abc[B] + sqrt_d) / (2 * abc[A]);
	return (true);
}

//returns the hit projection along the normal
//or -1 if none was found
static float	cone_body_hit(t_calc cc, const t_ray ray,
							t_intersection *out, float *t)
{
	t_vec3		hit_point;
	float		hit_proj;
	int			i;

	i = -1;
	while (++i < 2)
	{
		if (interval_contains(ray.range, t[i]))
		{
			hit_point = vec3_add(ray.origin,
					vec3_multiply(ray.direction, t[i]));
			hit_proj = cc.d_dot_n * t[i] + cc.oc_dot_n;
			if (hit_proj >= 0 && hit_proj <= cc.height)
			{
				out->distance = t[i];
				out->point = hit_point;
				return (hit_proj);
			}
		}
	}
	out->distance = 0;
	return (-1);
}

bool	cone_intersect(t_cone *co, const t_ray ray, t_intersection *out)
{
	t_calc		cc;
	float		t[2];
	t_vec3		hit_point;
	float		hit_proj;

	out->object = (t_object){.cone = co, .type = CONE};
	out->normal = co->axis;
	out->normal_calculated = true;
	if (!cone_calc(co, ray, t, &cc))
		return (cc.d_dot_n < 0 && circle_intersect((t_circle){co->bottom,
				co->axis, co->radius}, ray, &(out->distance), &(out->point)));
	hit_proj = cone_body_hit(cc, ray, out, t);
	if (hit_proj == -1)
		return (circle_intersect((t_circle){co->bottom,
				co->axis, co->radius}, ray, &(out->distance), &(out->point)));
	if (circle_intersect((t_circle){co->bottom, co->axis, co->radius},
		ray, &(t[1]), &(hit_point)) && out->distance > t[1])
	{
		out->distance = t[1];
		out->point = hit_point;
		return (true);
	}
	out->normal = calc_cone_normal(out->point, co, hit_proj);
	out->normal_calculated = false;
	return (true);
}
