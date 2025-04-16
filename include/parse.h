/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:29:00 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/16 12:22:46 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_types.h"

// parse_file.c

int	parse_scene(char *filename, t_scene *scene);

// set_utils.c

int	set_color(char *line, int start, t_vec3 *color);
int	set_vec(char *line, int start, t_vec3 *vec);
int	ft_substrtof(float *num, int start, char *line);

// set_scene.c

int	set_light(char *line, t_light *light);
int	set_camera(char *line, t_camera *camera);
int	set_ambient(char *line, t_ambient *ambient);

// set_scene_1.c

int	set_cylinder(char *line, t_cylinder *cylinder);
int	set_sphere(char *line, t_sphere *sphere);
int	set_plane(char *line, t_plane *plane);
int	set_cone(char *line, t_cone *cone);

// input_check

int	ft_parseerror(char *error, char *line);
int	input_check(int argc);
int	check_count(t_scene *scene, int camera, int ambient);
