#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

typedef struct s_vec3
{
    float x;
    float y;
    float z;
} t_vec3;

typedef t_vec3 t_color;

typedef struct s_point
{
    float min;  // Will be used as u
    float max;  // Will be used as v
} t_point;

typedef struct s_checkers
{
    float   width;
    float   height;
    t_color color_a;
    t_color color_b;
} t_checkers;

t_color	uv_pattern_at(const t_checkers checkers, t_point pt);
t_checkers	default_uv_checkers(void);
t_point	spherical_map(const t_vec3 sphere_point, const float radius);
t_checkers	uv_checkers(int width, int height, t_color color_a, t_color color_b);
t_color	pattern_at(const t_vec3 sphere_point, const float radius);

// Function to create a 3D point
t_vec3 point(float x, float y, float z)
{
    t_vec3 p;
    p.x = x;
    p.y = y;
    p.z = z;
    return p;
}

// Pattern at with specific checkers parameters
t_color pattern_at_with_params(const t_vec3 sphere_point, const float radius, 
                              int width, int height, t_color color_a, t_color color_b)
{
    t_checkers checkers;
    t_point pt;

    checkers = uv_checkers(width, height, color_a, color_b);
    pt = spherical_map(sphere_point, radius);
    return (uv_pattern_at(checkers, pt));
}

// Function to check if two colors are equal
bool color_eq(t_color a, t_color b)
{
    return (a.x == b.x && a.y == b.y && a.z == b.z);
}

// Function to print a color
void print_color(t_color color)
{
    printf("RGB(%.1f, %.1f, %.1f)", color.x, color.y, color.z);
}

// Function to get color name
const char* color_name(t_color color)
{
    if (color.x == 0 && color.y == 0 && color.z == 0)
        return "black";
    else if (color.x == 255 && color.y == 255 && color.z == 255)
        return "white";
    else
        return "other";
}

int main()
{
    // Define constants
    const t_color white = {255, 255, 255};
    const t_color black = {0, 0, 0};
    float radius = 1.0;  // Using unit sphere
    
    // Define test points from the scenario
    t_vec3 test_points[] = {
        point(0.4315, 0.4670, 0.7719),
        point(-0.9654, 0.2552, -0.0534),
        point(0.1039, 0.7090, 0.6975),
        point(-0.4986, -0.7856, -0.3663),
        point(-0.0317, -0.9395, 0.3411),
        point(0.4809, -0.7721, 0.4154),
        point(0.0285, -0.9612, -0.2745),
        point(-0.5734, -0.2162, -0.7903),
        point(0.7688, -0.1470, 0.6223),
        point(-0.7652, 0.2175, 0.6060)
    };
    
    // Define expected colors
    t_color expected_colors[] = {
        white,  // For point(0.4315, 0.4670, 0.7719)
        black,  // For point(-0.9654, 0.2552, -0.0534)
        white,  // For point(0.1039, 0.7090, 0.6975)
        black,  // For point(-0.4986, -0.7856, -0.3663)
        black,  // For point(-0.0317, -0.9395, 0.3411)
        black,  // For point(0.4809, -0.7721, 0.4154)
        black,  // For point(0.0285, -0.9612, -0.2745)
        white,  // For point(-0.5734, -0.2162, -0.7903)
        black,  // For point(0.7688, -0.1470, 0.6223)
        black   // For point(-0.7652, 0.2175, 0.6060)
    };

    // Test description strings to identify each test case
    const char* test_descriptions[] = {
        "point(0.4315, 0.4670, 0.7719)",
        "point(-0.9654, 0.2552, -0.0534)",
        "point(0.1039, 0.7090, 0.6975)",
        "point(-0.4986, -0.7856, -0.3663)",
        "point(-0.0317, -0.9395, 0.3411)",
        "point(0.4809, -0.7721, 0.4154)",
        "point(0.0285, -0.9612, -0.2745)",
        "point(-0.5734, -0.2162, -0.7903)",
        "point(0.7688, -0.1470, 0.6223)",
        "point(-0.7652, 0.2175, 0.6060)"
    };

    // Run tests with specified checkers parameters (16x8)
    printf("=== Testing Texture Map Pattern with Spherical Mapping ===\n");
    printf("Checkers parameters: width=16, height=8\n");
    printf("| %-30s | %-10s | %-10s | %-7s |\n", 
           "Point", "Expected", "Actual", "Result");
    printf("|--------------------------------|------------|------------|----------|\n");

    int pass_count = 0;
    int total_tests = sizeof(test_points) / sizeof(test_points[0]);
    
    clock_t start_time = clock();

    for (int i = 0; i < total_tests; i++)
    {
        t_vec3 test_point = test_points[i];
        t_color expected = expected_colors[i];
        
        // Call the function to test with specific parameters (16x8)
        t_color result = pattern_at_with_params(test_point, radius, 16, 8, white, black);
        
        // Check if results match expected values
        bool test_pass = color_eq(result, expected);
        
        if (test_pass)
            pass_count++;
        
        printf("| %-30s | %-10s | %-10s | %-7s |\n",
               test_descriptions[i],
               color_name(expected),
               color_name(result),
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
    t_color result;
    
    for (int i = 0; i < 1000000; i++) {
        // Slightly modify the point to prevent optimization
        sample_point.x = 0.5 + (i % 100) * 0.001;
        sample_point.y = 0.7 + (i % 50) * 0.001;
        sample_point.z = 0.3 + (i % 75) * 0.001;
        
        result = pattern_at_with_params(sample_point, radius, 16, 8, white, black);
    }
    
    end_time = clock();
    time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    
    printf("Time taken: %f seconds\n", time_spent);
    printf("Operations per second: %f\n", 1000000.0 / time_spent);
    
    // Print a result to prevent optimization
    printf("Final result: ");
    print_color(result);
    printf("\n");
    
    return (total_tests - pass_count); // Return 0 if all tests pass
}
