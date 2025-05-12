/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:29:00 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/12 11:05:09 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_types.h"

// parse_file.c

int		parse_scene(char *filename, t_scene *scene);

// set_utils.c

int		set_color(char *line, int start, t_vec3 *color);
int		set_vec(char *line, int start, t_vec3 *vec);
int		ft_substrtof(float *num, int start, char *line);
int		ft_set_texture_flag(int *i, char *line);

// set_scene.c

int		set_light(char *line, t_light *light);
int		set_camera(char *line, t_camera *camera);
int		set_ambient(char *line, t_ambient *ambient);

// set_scene_1.c

int		set_cylinder(char *line, t_cylinder *cylinder);
int		set_sphere(char *line, t_sphere *sphere);
int		set_plane(char *line, t_plane *plane);
int		set_cone(char *line, t_cone *cone);

// input_check

int		ft_parseerror(char *error, char *line, int position);
int		input_check(int argc);
int		check_count(t_scene *scene, int camera, int ambient);

// print_scene

void	print_vec3(const char *name, t_vec3 vec);
void	print_color(const char *name, t_color color);
void	print_camera(const t_camera *camera);
void	print_ambient_light(const t_ambient *ambient);
void	print_lights(const t_light *lights, size_t light_count);
void	print_image_dimensions(unsigned int width, unsigned int height);
void	print_spheres(const t_sphere *spheres, size_t sphere_count);
void	print_planes(const t_plane *planes, size_t plane_count);
void	print_cylinders(const t_cylinder *cylinders, size_t cylinder_count);
void	print_cones(const t_cone *cones, size_t cone_count);

// textures

void	*rperror_get_next_line(char *line, char *errormessage);
char	*parse_file_head(int fd);
int		set_bumpmap(char *obj_line, t_bumpmap *bumpmap);
int		open_bumpmap_file(char *line);

int		get_val(int fd);
t_bumpmap	*allocate_bumpmap(int fd);
