/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_scene_utils_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:47:11 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/20 12:34:16 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT_types.h"
#include "../../include/parse.h"

// Print spheres array
void	print_spheres(const t_sphere *spheres, size_t sphere_count)
{
	size_t	i;

	i = 0;
	printf("SPHERES (%zu):\n", sphere_count);
	if (sphere_count > 0 && spheres)
	{
		while (i < sphere_count)
		{
			printf("  Sphere #%zu:\n", i + 1);
			print_vec3("	Position", spheres[i].pos);
			print_color("	Color", spheres[i].color);
			printf("	Radius: %.2f\n", spheres[i].radius);
			if (spheres[i].texturing.type == TEXTURE)
				printf("	texturing: TEXTURE\n");
			if (spheres[i].texturing.type == CHECKERS)
				printf("	texturing: CHECKERS\n");
			if (spheres[i].texturing.bumpmap)
				printf("	texturing: BUMP\n");
			i++;
		}
	}
	else
		printf("  No spheres defined\n");
	printf("\n");
}

// Print planes array
void	print_planes(const t_plane *planes, size_t plane_count)
{
	size_t	i;

	i = 0;
	printf("PLANES (%zu):\n", plane_count);
	if (plane_count > 0 && planes)
	{
		while (i < plane_count)
		{
			printf("  Plane #%zu:\n", i + 1);
			print_vec3("	Position", planes[i].pos);
			print_vec3("	Normal", planes[i].normal);
			print_color("	Color", planes[i].color);
			i++;
		}
	}
	else
		printf("  No planes defined\n");
	printf("\n");
}

// Print cylinders array
void	print_cylinders(const t_cylinder *cylinders, size_t cylinder_count)
{
	size_t	i;

	i = 0;
	printf("CYLINDERS (%zu):\n", cylinder_count);
	if (cylinder_count > 0 && cylinders)
	{
		while (i < cylinder_count)
		{
			printf("  Cylinder #%zu:\n", i + 1);
			print_vec3("	Position", cylinders[i].pos);
			print_vec3("	Axis", cylinders[i].axis);
			print_color("	Color", cylinders[i].color);
			printf("	Radius: %.2f\n", cylinders[i].radius);
			printf("	Height: %.2f\n", cylinders[i].height);
			i++;
		}
	}
	else
		printf("  No cylinders defined\n");
	printf("\n");
}

// Print cones array
void	print_cones(const t_cone *cones, size_t cone_count)
{
	size_t	i;

	i = 0;
	printf("CONES (%zu):\n", cone_count);
	if (cone_count > 0 && cones)
	{
		while (i < cone_count)
		{
			printf("  Cone #%zu:\n", i + 1);
			print_vec3("	Top position", cones[i].top);
			print_vec3("	Bottom position", cones[i].bottom);
			print_vec3("	Axis", cones[i].axis);
			print_color("	Color", cones[i].color);
			printf("	Radius: %.2f\n", cones[i].radius);
			printf("	Height: %.2f\n", cones[i].height);
			printf("	Slant: %.2f\n", cones[i].slant);
			i++;
		}
	}
	else
		printf("  No cones defined\n");
	printf("\n");
}
