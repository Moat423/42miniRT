#include "../include/miniRT.h"

/**
 * Finds the closest point on an AABB to a given point.
 * This clamps the point to the AABB boundaries in each dimension.
 */
t_vec3	aabb_closest_point(t_aabb aabb, t_vec3 point)
{
	t_vec3	result;

	result.x = fmaxf(aabb.min.x, fminf(point.x, aabb.max.x));
	result.y = fmaxf(aabb.min.y, fminf(point.y, aabb.max.y));
	result.z = fmaxf(aabb.min.z, fminf(point.z, aabb.max.z));

	return (result);
}

/**
 * Creates an AABB for a sphere.
 * The AABB is centered at the sphere's position with
 * dimensions determined by the sphere's radius.
 */
t_aabb	sphere_aabb(t_sphere *sphere)
{
	t_aabb	aabb;

	aabb.min = vec3_new(
		sphere->pos.x - sphere->radius,
		sphere->pos.y - sphere->radius,
		sphere->pos.z - sphere->radius
	);

	aabb.max = vec3_new(
		sphere->pos.x + sphere->radius,
		sphere->pos.y + sphere->radius,
		sphere->pos.z + sphere->radius
	);

	return (aabb);
}

/**
 * Creates an AABB for a cylinder.
 * The AABB encompasses both circular ends and the body of the cylinder.
 */
t_aabb	cylinder_aabb(t_cylinder *cylinder)
{
	t_aabb	aabb;

	aabb.min.x = fminf(cylinder->top.x, cylinder->bottom.x) - cylinder->radius;
	aabb.min.y = fminf(cylinder->top.y, cylinder->bottom.y) - cylinder->radius;
	aabb.min.z = fminf(cylinder->top.z, cylinder->bottom.z) - cylinder->radius;
	aabb.max.x = fmaxf(cylinder->top.x, cylinder->bottom.x) + cylinder->radius;
	aabb.max.y = fmaxf(cylinder->top.y, cylinder->bottom.y) + cylinder->radius;
	aabb.max.z = fmaxf(cylinder->top.z, cylinder->bottom.z) + cylinder->radius;

	return (aabb);
}

/**
 * Creates an AABB for a cone.
 * The AABB encompasses the cone's base circle and its apex.
 */
t_aabb	cone_aabb(t_cone *cone)
{
	t_aabb	aabb;

	aabb.min.x = fminf(cone->top.x, cone->bottom.x) - cone->radius;
	aabb.min.y = fminf(cone->top.y, cone->bottom.y) - cone->radius;
	aabb.min.z = fminf(cone->top.z, cone->bottom.z) - cone->radius;
	aabb.max.x = fmaxf(cone->top.x, cone->bottom.x) + cone->radius;
	aabb.max.y = fmaxf(cone->top.y, cone->bottom.y) + cone->radius;
	aabb.max.z = fmaxf(cone->top.z, cone->bottom.z) + cone->radius;

	return (aabb);
}

/**
 * Creates an AABB for any object type (except planes).
 * For planes, returns an "infinite" AABB since they extend infinitely.
 */
t_aabb	create_aabb_from_object(t_object object)
{
	t_aabb	aabb;

	if (object.type == SPHERE)
		return (sphere_aabb(object.sphere));
	else if (object.type == CYLINDER)
		return (cylinder_aabb(object.cylinder));
	else if (object.type == CONE)
		return (cone_aabb(object.cone));
	else
	{
		aabb.min = vec3_new(-INFINITY, -INFINITY, -INFINITY);
		aabb.max = vec3_new(INFINITY, INFINITY, INFINITY);
		return (aabb);
	}
}
