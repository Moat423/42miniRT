#include "../../include/miniRT_types.h"

// Helper functions to print common data types
void print_vec3(const char *name, t_vec3 vec) {
	printf("%s: (%.2f, %.2f, %.2f)\n", name, vec.x, vec.y, vec.z);
}

void print_color(const char *name, t_color color) {
	printf("%s: (R:%.2f, G:%.2f, B:%.2f)\n", name, color.x, color.y, color.z);
}

// Print camera details
void print_camera(const t_camera *camera) {
	printf("CAMERA:\n");
	print_vec3("  Position", camera->pos);
	print_vec3("  Direction", camera->dir);
	print_vec3("  Up vector", camera->up);
	print_vec3("  Right vector", camera->right);
	printf("  FOV: %u degrees\n\n", camera->fov);
}

// Print ambient light
void print_ambient_light(const t_ambient *ambient) {
	printf("AMBIENT LIGHT:\n");
	print_color("  Color", ambient->color);
	printf("  Brightness: %.2f\n\n", ambient->brightness);
}

// Print lights array
void print_lights(const t_light *lights, size_t light_count) {
	printf("LIGHTS (%zu):\n", light_count);
	if (light_count > 0 && lights) {
		for (size_t i = 0; i < light_count; i++) {
			printf("  Light #%zu:\n", i + 1);
			print_vec3("	Position", lights[i].pos);
			print_color("	Color", lights[i].color);
			printf("	Brightness: %.2f\n", lights[i].brightness);
		}
	} else {
		printf("  No lights defined\n");
	}
	printf("\n");
}

// Print spheres array
void print_spheres(const t_sphere *spheres, size_t sphere_count) {
	printf("SPHERES (%zu):\n", sphere_count);
	if (sphere_count > 0 && spheres) {
		for (size_t i = 0; i < sphere_count; i++) {
			printf("  Sphere #%zu:\n", i + 1);
			print_vec3("	Position", spheres[i].pos);
			print_color("	Color", spheres[i].color);
			printf("	Radius: %.2f\n", spheres[i].radius);
		}
	} else {
		printf("  No spheres defined\n");
	}
	printf("\n");
}

// Print planes array
void print_planes(const t_plane *planes, size_t plane_count) {
	printf("PLANES (%zu):\n", plane_count);
	if (plane_count > 0 && planes) {
		for (size_t i = 0; i < plane_count; i++) {
			printf("  Plane #%zu:\n", i + 1);
			print_vec3("	Position", planes[i].pos);
			print_vec3("	Normal", planes[i].normal);
			print_color("	Color", planes[i].color);
		}
	} else {
		printf("  No planes defined\n");
	}
	printf("\n");
}

// Print cylinders array
void print_cylinders(const t_cylinder *cylinders, size_t cylinder_count) {
	printf("CYLINDERS (%zu):\n", cylinder_count);
	if (cylinder_count > 0 && cylinders) {
		for (size_t i = 0; i < cylinder_count; i++) {
			printf("  Cylinder #%zu:\n", i + 1);
			print_vec3("	Position", cylinders[i].pos);
			print_vec3("	Axis", cylinders[i].axis);
			print_color("	Color", cylinders[i].color);
			printf("	Radius: %.2f\n", cylinders[i].radius);
			printf("	Height: %.2f\n", cylinders[i].height);
		}
	} else {
		printf("  No cylinders defined\n");
	}
	printf("\n");
}

// Print cones array
void print_cones(const t_cone *cones, size_t cone_count) {
	printf("CONES (%zu):\n", cone_count);
	if (cone_count > 0 && cones) {
		for (size_t i = 0; i < cone_count; i++) {
			printf("  Cone #%zu:\n", i + 1);
			print_vec3("	Top position", cones[i].top);
			print_vec3("	Bottom position", cones[i].bottom);
			print_vec3("	Axis", cones[i].axis);
			print_color("	Color", cones[i].color);
			printf("	Radius: %.2f\n", cones[i].radius);
			printf("	Height: %.2f\n", cones[i].height);
			printf("	Slant: %.2f\n", cones[i].slant);
		}
	} else {
		printf("  No cones defined\n");
	}
	printf("\n");
}

// Print image dimensions
void print_image_dimensions(unsigned int width, unsigned int height) {
	printf("IMAGE DIMENSIONS:\n");
	printf("  Width: %u pixels\n", width);
	printf("  Height: %u pixels\n", height);
}

// Main function to print all scene contents
void	print_scene(const t_scene *scene)
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
	print_spheres(scene->spheres, scene->sphere_count);
	print_planes(scene->planes, scene->plane_count);
	print_cylinders(scene->cylinders, scene->cylinder_count);
	print_cones(scene->cones, scene->cone_count);
	print_image_dimensions(scene->image_width, scene->image_height);
	printf("==================================\n\n");
}
