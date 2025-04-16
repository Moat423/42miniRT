/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_scene_utils_0.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:44:44 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/16 14:53:49 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT_types.h"
#include "../../include/parse.h"

// Helper functions to print common data types
void	print_vec3(const char *name, t_vec3 vec)
{
	printf("%s: (%.2f, %.2f, %.2f)\n", name, vec.x, vec.y, vec.z);
}

void	print_color(const char *name, t_color color)
{
	printf("%s: (R:%.2f, G:%.2f, B:%.2f)\n", name, color.x, color.y, color.z);
}

// Print camera details
void	print_camera(const t_camera *camera)
{
	printf("CAMERA:\n");
	print_vec3("  Position", camera->pos);
	print_vec3("  Direction", camera->dir);
	print_vec3("  Up vector", camera->up);
	print_vec3("  Right vector", camera->right);
	printf("  FOV: %u degrees\n\n", camera->fov);
}

// Print ambient light
void	print_ambient_light(const t_ambient *ambient)
{
	printf("AMBIENT LIGHT:\n");
	print_color("  Color", ambient->color);
	printf("  Brightness: %.2f\n\n", ambient->brightness);
}

// Print lights array
void	print_lights(const t_light *lights, size_t light_count)
{
	size_t	i;

	i = 0;
	printf("LIGHTS (%zu):\n", light_count);
	if (light_count > 0 && lights)
	{
		while (i < light_count)
		{
			printf("  Light #%zu:\n", i + 1);
			print_vec3("	Position", lights[i].pos);
			print_color("	Color", lights[i].color);
			printf("	Brightness: %.2f\n", lights[i].brightness);
			i++;
		}
	}
	else
		printf("  No lights defined\n");
	printf("\n");
}
