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
    float min;  // Will be used as u
    float max;  // Will be used as v
} t_point;

// Function to create a 3D point
t_vec3 point(float x, float y, float z)
{
    t_vec3 p;
    p.x = x;
    p.y = y;
    p.z = z;
    return p;
}

// The planar mapping function using fmod
// disregarding negative numbers
t_point planar_map_fmod(const t_vec3 plane_point)
{
    t_point pt;

    pt.min = fmod(plane_point.x, 1.0);
    pt.max = fmod(plane_point.z, 1.0);
    pt.min += (pt.min < 0) * 1.0f;
    pt.max += (pt.max < 0) * 1.0f;
    return pt;
}

// Alternative implementation using modf
// disregarding negative numbers
t_point planar_map_modf(const t_vec3 plane_point)
{
    t_point pt;
    double integral_part;
    
    pt.min = modf(plane_point.x, &integral_part);
    pt.max = modf(plane_point.z, &integral_part);
    pt.min += (pt.min < 0) * 1.0f;
    pt.max += (pt.max < 0) * 1.0f;
    return pt;
}

t_point	planar_map(const t_vec3 plane_point)
{
	t_point pt;

    pt.min = plane_point.x - floorf(plane_point.x);
    pt.max = plane_point.z - floorf(plane_point.z);
	return (pt);
}

// Function to check if two float values are approximately equal
bool float_eq(float a, float b, float epsilon)
{
    return fabs(a - b) < epsilon;
}

int main()
{
    // Define test points from the table
    t_vec3 test_points[] = {
        point(0.25, 0, 0.5),
        point(0.25, 0, -0.25),
        point(0.25, 0.5, -0.25),
        point(1.25, 0, 0.5),
        point(0.25, 0, -1.75),
        point(1, 0, -1),
        point(0, 0, 0)
    };

    // Define expected UV coordinates from the table
    float expected_uv[][2] = {
        {0.25, 0.5},
        {0.25, 0.75},
        {0.25, 0.75},
        {0.25, 0.5},
        {0.25, 0.25},
        {0.0, 0.0},
        {0.0, 0.0}
    };

    // Test description strings to identify each test case
    const char* test_descriptions[] = {
        "point(0.25, 0, 0.5)",
        "point(0.25, 0, -0.25)",
        "point(0.25, 0.5, -0.25)",
        "point(1.25, 0, 0.5)",
        "point(0.25, 0, -1.75)",
        "point(1, 0, -1)",
        "point(0, 0, 0)"
    };

    // Run tests with timing for fmod implementation
    printf("=== Testing Planar Mapping Function (fmod implementation) ===\n");
    printf("| %-25s | %-8s | %-8s | %-8s | %-8s | %-7s |\n", 
           "Point", "Expected U", "Actual U", "Expected V", "Actual V", "Result");
    printf("|---------------------------|----------|----------|----------|----------|----------|\n");

    int pass_count_fmod = 0;
    int total_tests = sizeof(test_points) / sizeof(test_points[0]);
    float epsilon = 0.01; // Tolerance for float comparison
    
    clock_t start_time = clock();

    for (int i = 0; i < total_tests; i++)
    {
        t_vec3 test_point = test_points[i];
        float expected_u = expected_uv[i][0];
        float expected_v = expected_uv[i][1];
        
        // Call the function to test
        t_point result = planar_map_fmod(test_point);
        float actual_u = result.min;
        float actual_v = result.max;
        
        // Check if results match expected values
        bool u_pass = float_eq(actual_u, expected_u, epsilon);
        bool v_pass = float_eq(actual_v, expected_v, epsilon);
        bool test_pass = u_pass && v_pass;
        
        if (test_pass)
            pass_count_fmod++;
        
        printf("| %-25s | %-8.2f | %-8.2f | %-8.2f | %-8.2f | %-7s |\n",
               test_descriptions[i],
               expected_u, actual_u,
               expected_v, actual_v,
               test_pass ? "PASS" : "FAIL");
    }
    
    clock_t end_time = clock();
    double time_spent_fmod = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    
    printf("\nFmod Implementation Results: %d/%d tests passed\n", pass_count_fmod, total_tests);
    printf("Total time: %f seconds\n", time_spent_fmod);

    // Run tests with timing for modf implementation
    printf("\n=== Testing Planar Mapping Function (modf implementation) ===\n");
    printf("| %-25s | %-8s | %-8s | %-8s | %-8s | %-7s |\n", 
           "Point", "Expected U", "Actual U", "Expected V", "Actual V", "Result");
    printf("|---------------------------|----------|----------|----------|----------|----------|\n");

    int pass_count_modf = 0;
    
    start_time = clock();

    for (int i = 0; i < total_tests; i++)
    {
        t_vec3 test_point = test_points[i];
        float expected_u = expected_uv[i][0];
        float expected_v = expected_uv[i][1];
        
        // Call the function to test
        t_point result = planar_map_modf(test_point);
        float actual_u = result.min;
        float actual_v = result.max;
        
        // Check if results match expected values
        bool u_pass = float_eq(actual_u, expected_u, epsilon);
        bool v_pass = float_eq(actual_v, expected_v, epsilon);
        bool test_pass = u_pass && v_pass;
        
        if (test_pass)
            pass_count_modf++;
        
        printf("| %-25s | %-8.2f | %-8.2f | %-8.2f | %-8.2f | %-7s |\n",
               test_descriptions[i],
               expected_u, actual_u,
               expected_v, actual_v,
               test_pass ? "PASS" : "FAIL");
    }
    
    end_time = clock();
    double time_spent_modf = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    
    printf("\nModf Implementation Results: %d/%d tests passed\n", pass_count_modf, total_tests);
    printf("Total time: %f seconds\n", time_spent_modf);

    // Performance test
    printf("\n=== Performance Test (10,000,000 iterations) ===\n");
    
    // Test fmod performance
    start_time = clock();
    t_vec3 sample_point = point(0.5, 0.7, 0.3);
    t_point result_fmod;
    
    for (int i = 0; i < 10000000; i++) {
        // Slightly modify the point to prevent optimization
        sample_point.x = 0.5 + (i % 100) * 0.01;
        sample_point.y = 0.7;
        sample_point.z = 0.3 + (i % 75) * 0.01;
        
        result_fmod = planar_map_fmod(sample_point);
    }
    
    end_time = clock();
    time_spent_fmod = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    
    printf("Fmod Performance:\n");
    printf("Time taken: %f seconds\n", time_spent_fmod);
    printf("Operations per second: %f\n", 10000000.0 / time_spent_fmod);
    
    // Test modf performance
    start_time = clock();
    t_point result_modf;
    
    for (int i = 0; i < 10000000; i++) {
        // Slightly modify the point to prevent optimization
        sample_point.x = 0.5 + (i % 100) * 0.01;
        sample_point.y = 0.7;
        sample_point.z = 0.3 + (i % 75) * 0.01;
        
        result_modf = planar_map_modf(sample_point);
    }
    
    end_time = clock();
    time_spent_modf = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    
    printf("\nModf Performance:\n");
    printf("Time taken: %f seconds\n", time_spent_modf);
    printf("Operations per second: %f\n", 10000000.0 / time_spent_modf);
    
    // Performance comparison
    printf("\n=== Performance Comparison ===\n");
    if (time_spent_fmod < time_spent_modf) {
        double percent_faster = (time_spent_modf / time_spent_fmod - 1.0) * 100.0;
        printf("fmod is %.2f%% faster than modf\n", percent_faster);
    } else {
        double percent_faster = (time_spent_fmod / time_spent_modf - 1.0) * 100.0;
        printf("modf is %.2f%% faster than fmod\n", percent_faster);
    }
    
    // Print final results to prevent optimization
    printf("\nFinal results to prevent optimization:\n");
    printf("fmod: u=%.6f, v=%.6f\n", result_fmod.min, result_fmod.max);
    printf("modf: u=%.6f, v=%.6f\n", result_modf.min, result_modf.max);
    
    return (total_tests - (pass_count_fmod < pass_count_modf ? pass_count_fmod : pass_count_modf));
}
