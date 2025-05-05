/* compiling:
cc srcs/tests/set_bumpmap_test.c srcs/parse/set_textures.c srcs/util/scene_destroy.c srcs/util/scene_malloc.c srcs/parse/input_check.c srcs/parse/get_bumpmap.c lib/libft/libft.a 
lib/libft/libft_full.h
*/
#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "../../lib/libft/libft_full.h"
#include "../../include/miniRT_types.h"

int		set_bumpmap(char *obj_line, t_sphere *sphere);
void	bumpmap_destroy(t_bumpmap *bumpmap);
void	*rperror_get_next_line(char *line, char *errormessage);
int		open_bumpmap_file(char *line);

/**
 * Test the set_bumpmap function
 * Returns 0 if all tests pass, non-zero otherwise
 */
int main(void)
{
    int result;
    t_sphere sphere;
    char *test_file = "scene/textures/Brick_bump.ppm";
    int failed = 0;
    
    // Initialize the sphere structure
    sphere.bumpmap = NULL;
    
    printf("=== Testing set_bumpmap ===\n");
    
    // Test setting bumpmap on the sphere
    printf("Testing with valid bumpmap file: %s\n", test_file);
    result = set_bumpmap(test_file, &sphere);
    
    // Check if function succeeded
    if (result == 0)
    {
        printf("FAILED: set_bumpmap returned 0 (error)\n");
        failed = 1;
    }
    else
    {
        printf("PASSED: set_bumpmap returned 1 (success)\n");
        
        // Check if bumpmap was assigned to sphere
        if (sphere.bumpmap == NULL)
        {
            printf("FAILED: Bumpmap was not assigned to sphere\n");
            failed = 1;
        }
        else
        {
            printf("PASSED: Bumpmap was assigned to sphere\n");
            
            // Check dimensions
            if (sphere.bumpmap->width != 1024 || sphere.bumpmap->height != 1024)
            {
                printf("FAILED: Expected dimensions 300x300, got %zux%zu\n", 
                       sphere.bumpmap->width, sphere.bumpmap->height);
                failed = 1;
            }
            else
            {
                printf("PASSED: Bumpmap has the expected dimensions 300x300\n");
            }
            
            // Check if elevation data was loaded
            if (sphere.bumpmap->elevation == NULL)
            {
                printf("FAILED: Bumpmap elevation data is NULL\n");
                failed = 1;
            }
            else
            {
                printf("PASSED: Bumpmap elevation data was loaded\n");
                
                // Optional: Check a few values to ensure data was loaded correctly
                // This depends on what you expect in your test file
                printf("INFO: First few elevation values: %f, %f, %f\n",
                       sphere.bumpmap->elevation[0],
                       sphere.bumpmap->elevation[1],
                       sphere.bumpmap->elevation[2]);
            }
            
            // Clean up
            bumpmap_destroy(sphere.bumpmap);
            printf("INFO: Bumpmap destroyed\n");
        }
    }
    
    if (failed)
        printf("=== Test FAILED ===\n");
    else
        printf("=== All tests PASSED ===\n");
    
    return failed;
}
