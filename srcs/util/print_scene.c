#include "../../include/miniRT_types.h"

// Helper function to print a t_vec3
void print_vec3(const char *name, t_vec3 vec) {
	printf("%s: (%.2f, %.2f, %.2f)\n", name, vec.x, vec.y, vec.z);
}

// Helper function to print a t_color
void print_color(const char *name, t_color color) {
	printf("%s: (R:%.2f, G:%.2f, B:%.2f)\n", name, color.x, color.y, color.z);
}

// Print all contents of the scene struct
void print_scene(const t_scene *scene) {
	if (!scene) {
		printf("Error: NULL scene pointer\n");
		return;
	}

	printf("\n==================================\n");
	printf("SCENE CONTENTS (Date: 2025-03-20 12:11:28)\n");
	printf("==================================\n\n");

	// Print camera details
	printf("CAMERA:\n");
	print_vec3("  Position", scene->camera.pos);
	print_vec3("  Direction", scene->camera.dir);
	print_vec3("  Up vector", scene->camera.up);
	print_vec3("  Right vector", scene->camera.right);
	printf("  FOV: %u degrees\n\n", scene->camera.fov);

	// Print ambient light
	printf("AMBIENT LIGHT:\n");
	print_color("  Color", scene->ambient.color);
	printf("  Brightness: %.2f\n\n", scene->ambient.brightness);

	// Print lights
	printf("LIGHTS (%zu):\n", scene->light_count);
	if (scene->light_count > 0 && scene->lights) {
		for (size_t i = 0; i < scene->light_count; i++) {
			printf("  Light #%zu:\n", i + 1);
			print_vec3("	Position", scene->lights[i].pos);
			print_color("	Color", scene->lights[i].color);
			printf("	Brightness: %.2f\n", scene->lights[i].brightness);
		}
	} else {
		printf("  No lights defined\n");
	}
	printf("\n");

	// Print spheres
	printf("SPHERES (%zu):\n", scene->sphere_count);
	if (scene->sphere_count > 0 && scene->spheres) {
		for (size_t i = 0; i < scene->sphere_count; i++) {
			printf("  Sphere #%zu:\n", i + 1);
			print_vec3("	Position", scene->spheres[i].pos);
			print_color("	Color", scene->spheres[i].color);
			printf("	Radius: %.2f\n", scene->spheres[i].radius);
		}
	} else {
		printf("  No spheres defined\n");
	}
	printf("\n");

	// Print planes
	printf("PLANES (%zu):\n", scene->plane_count);
	if (scene->plane_count > 0 && scene->planes) {
		for (size_t i = 0; i < scene->plane_count; i++) {
			printf("  Plane #%zu:\n", i + 1);
			print_vec3("	Position", scene->planes[i].pos);
			print_vec3("	Normal", scene->planes[i].normal);
			print_color("	Color", scene->planes[i].color);
		}
	} else {
		printf("  No planes defined\n");
	}
	printf("\n");

	// Print cylinders
	printf("CYLINDERS (%zu):\n", scene->cylinder_count);
	if (scene->cylinder_count > 0 && scene->cylinders) {
		for (size_t i = 0; i < scene->cylinder_count; i++) {
			printf("  Cylinder #%zu:\n", i + 1);
			print_vec3("	Position", scene->cylinders[i].pos);
			print_vec3("	Axis", scene->cylinders[i].axis);
			print_color("	Color", scene->cylinders[i].color);
			printf("	Diameter: %.2f\n", scene->cylinders[i].diameter);
			printf("	Height: %.2f\n", scene->cylinders[i].height);
		}
	} else {
		printf("  No cylinders defined\n");
	}
	printf("\n");

	// Print image dimensions
	printf("IMAGE DIMENSIONS:\n");
	printf("  Width: %u pixels\n", scene->image_width);
	printf("  Height: %u pixels\n", scene->image_height);
	printf("==================================\n\n");
}
