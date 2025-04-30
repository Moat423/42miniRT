#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

typedef struct s_vec3
{
    float x;
    float y;
    float z;
} t_vec3;

typedef struct s_point
{
    float min;
    float max;
} t_point;

//compile together with checkers.c

t_point	spherical_map(t_vec3 sphere_point, float radius);

// Function to create a 3D point
t_vec3 point(float x, float y, float z)
{
    t_vec3 p;
    p.x = x;
    p.y = y;
    p.z = z;
    return p;
}

// Function to check if two float values are approximately equal
bool float_eq(float a, float b, float epsilon)
{
    return fabs(a - b) < epsilon;
}

int main()
{
    // Define test points from the table
    // Format: [x, y, z] for each point
    t_vec3 test_points[] = {
        point(0, 0, -1),      // point(0, 0, -1)
        point(1, 0, 0),       // point(1, 0, 0)
        point(0, 0, 1),       // point(0, 0, 1)
        point(-1, 0, 0),      // point(-1, 0, 0)
        point(0, 1, 0),       // point(0, 1, 0)
        point(0, -1, 0),      // point(0, -1, 0)
        point(sqrt(2)/2, sqrt(2)/2, 0) // point(√2/2, √2/2, 0)
    };

    // Define expected UV coordinates from the table
    // Format: [u, v] for each test case
    float expected_uv[][2] = {
        {0.0, 0.5},   // For point(0, 0, -1)
        {0.25, 0.5},  // For point(1, 0, 0)
        {0.5, 0.5},   // For point(0, 0, 1)
        {0.75, 0.5},  // For point(-1, 0, 0)
        {0.5, 1.0},   // For point(0, 1, 0)
        {0.5, 0.0},   // For point(0, -1, 0)
        {0.25, 0.75}  // For point(√2/2, √2/2, 0)
    };

    // Test description strings to identify each test case
    const char* test_descriptions[] = {
        "point(0, 0, -1)",
        "point(1, 0, 0)",
        "point(0, 0, 1)",
        "point(-1, 0, 0)",
        "point(0, 1, 0)",
        "point(0, -1, 0)",
        "point(√2/2, √2/2, 0)"
    };

    // Run tests with timing
    printf("=== Testing Spherical Mapping Function ===\n");
    printf("| %-20s | %-8s | %-8s | %-8s | %-8s | %-7s |\n", 
           "Point", "Expected U", "Actual U", "Expected V", "Actual V", "Result");
    printf("|----------------------|----------|----------|----------|----------|----------|\n");

    int pass_count = 0;
    int total_tests = sizeof(test_points) / sizeof(test_points[0]);
    float radius = 1.0; // Using unit sphere
    float epsilon = 0.01; // Tolerance for float comparison
    
    clock_t start_time = clock();

    for (int i = 0; i < total_tests; i++)
    {
        t_vec3 test_point = test_points[i];
        float expected_u = expected_uv[i][0];
        float expected_v = expected_uv[i][1];
        
        // Call the function to test
        t_point result = spherical_map(test_point, radius);
        float actual_u = result.min;
        float actual_v = result.max;
        
        // Check if results match expected values
        bool u_pass = float_eq(actual_u, expected_u, epsilon);
        bool v_pass = float_eq(actual_v, expected_v, epsilon);
        bool test_pass = u_pass && v_pass;
        
        if (test_pass)
            pass_count++;
        
        printf("| %-20s | %-8.2f | %-8.2f | %-8.2f | %-8.2f | %-7s |\n",
               test_descriptions[i],
               expected_u, actual_u,
               expected_v, actual_v,
               test_pass ? "PASS" : "FAIL");
    }
    
    clock_t end_time = clock();
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    
    printf("\nTest Results: %d/%d tests passed\n", pass_count, total_tests);
    printf("Total time: %f seconds\n", time_spent);

    // Performance test
    printf("\n=== Performance Test (1,000,000 iterations) ===\n");
    
    start_time = clock();
    t_vec3 sample_point = point(0.5, 0.7, 0.3);
    t_point result;
    
    for (int i = 0; i < 1000000; i++) {
        // Slightly modify the point to prevent optimization
        sample_point.x = 0.5 + (i % 100) * 0.001;
        sample_point.y = 0.7 + (i % 50) * 0.001;
        sample_point.z = 0.3 + (i % 75) * 0.001;
        
        result = spherical_map(sample_point, radius);
    }
    
    end_time = clock();
    time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    
    printf("Time taken: %f seconds\n", time_spent);
    printf("Operations per second: %f\n", 1000000.0 / time_spent);
    
    // Print a result to prevent optimization
    printf("Final result: u=%.6f, v=%.6f\n", result.min, result.max);
    
    return (total_tests - pass_count); // Return 0 if all tests pass
}
