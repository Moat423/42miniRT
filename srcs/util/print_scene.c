/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_scene.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:43:27 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/16 14:53:52 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT_types.h"
#include "../../include/parse.h"

// Print image dimensions
void	print_image_dimensions(unsigned int width, unsigned int height)
{
	printf("IMAGE DIMENSIONS:\n");
	printf("  Width: %u pixels\n", width);
	printf("  Height: %u pixels\n", height);
}

// Main function to print all scene contents
void	print_scene(const	t_scene *scene)
{
	if (!scene)
	{
		printf("Error: NULL scene pointer\n");
		return ;
	}
	printf("\n==================================\n");
	printf("SCENE CONTENTS (Date: %s)\n", __DATE__ " " __TIME__);
	printf("==================================\n\n");
	print_camera(&scene->camera);
	print_ambient_light(&scene->ambient);
	print_lights(scene->lights, scene->light_count);
	print_spheres(scene->objs.spheres, scene->objs.sphere_count);
	print_planes(scene->objs.planes, scene->objs.plane_count);
	print_cylinders(scene->objs.cylinders, scene->objs.cylinder_count);
	print_cones(scene->objs.cones, scene->objs.cone_count);
	print_image_dimensions(scene->image_width, scene->image_height);
	printf("==================================\n\n");
}
