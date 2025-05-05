/* compile:
 * cc -DTEST_MAIN srcs/tests/open_bumpmap_file_test.c srcs/parse/set_textures.c lib/libft/libft.a -o test_open_bumpmap
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

// Function prototype for the function we're testing
int open_bumpmap_file(char *line);

/**
 * Test the open_bumpmap_file function
 * Returns the number of failed tests
 */
int test_open_bumpmap_file(void)
{
    int failed_tests = 0;
    int fd;
    int test_fd;
    char test_file[] = "scene/textures/Brick_bump.ppm";
    char nonexistent_file[] = "nonexistent_file.ppm";
    
    printf("=== Testing open_bumpmap_file ===\n");
    
    // Create a temporary test file
    test_fd = open(test_file, O_WRONLY | O_CREAT, 0644);
    if (test_fd == -1)
    {
        printf("Failed to create test file\n");
        return 1;
    }
    
    // Write some content to the test file
    write(test_fd, "P3\n10 10\n255\n", 12);
    close(test_fd);
    
    // Test case 1: Valid file
    printf("Test 1: Valid file...\n");
    fd = open_bumpmap_file(test_file);
    if (fd == -1)
    {
        printf("FAILED: Could not open valid file\n");
        failed_tests++;
    }
    else
    {
        printf("PASSED: Successfully opened valid file\n");
        close(fd);
    }
    
    // Test case 2: Valid file with leading spaces
    printf("Test 2: Valid file with leading spaces...\n");
    fd = open_bumpmap_file("   scene/textures/Brick_bump.ppm");
    if (fd == -1)
    {
        printf("FAILED: Could not open valid file with leading spaces\n");
        failed_tests++;
    }
    else
    {
        printf("PASSED: Successfully opened valid file with leading spaces\n");
        close(fd);
    }
    
    // Test case 3: Non-existent file
    printf("Test 3: Non-existent file...\n");
    fd = open_bumpmap_file(nonexistent_file);
    if (fd != -1)
    {
        printf("FAILED: Opened non-existent file somehow\n");
        close(fd);
        failed_tests++;
    }
    else
    {
        printf("PASSED: Correctly failed to open non-existent file\n");
    }
    
    // Clean up
    unlink(test_file);
    
    printf("=== Test Summary: %d tests failed ===\n", failed_tests);
    return failed_tests;
}

// Main function for running the test independently
#ifdef TEST_MAIN
int main(void)
{
    return test_open_bumpmap_file();
}
#endif
