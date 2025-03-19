/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:29:00 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/03/19 12:43:21 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// parse_file.c
int	ft_parseerror(char *error, char *line);
int	input_check(int argc, char **argv);
int	parse_file(int fd, t_scene *scene);
// set_scene.c
int	set_color(char *line, int start, t_vec3 *color);
int	set_light(char *line, t_light *light);
int	set_vec(char *line, int start, t_vec3 *vec);
int	set_camera(char *line, t_camera *camera);
int	set_ambient(char *line, t_ambient *ambient);
// set_scene_1.c
int	set_cylinder(char *line, t_cylinder *cylinder);
int	set_sphere(char *line, t_sphere *sphere);
int	set_plane(char *line, t_plane *plane);
// input_check
int	ft_parseerror(char *error, char *line);
