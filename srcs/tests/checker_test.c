#include <stdio.h>
#include <time.h>

typedef struct s_vec3
{
	float	x;
	float	y;
	float	z;
}	t_vec3;

typedef t_vec3			t_color;

typedef struct s_checkers
{
	float	width;
	float	height;
	t_color	color_a;
	t_color	color_b;
}	t_checkers;

t_checkers	uv_checkers(void)
{
	t_checkers	ch;

	ch.width = 2;
	ch.height = 2;
	ch.color_a = (t_vec3){255, 255, 255};
	ch.color_b = (t_vec3){0, 0, 0};
	return (ch);
}
// Function to print color values
void print_color(t_color color) {
	printf("RGB(%.1f, %.1f, %.1f)", color.x, color.y, color.z);
}

// Test function to verify the checkerboard pattern logic
void test_checkerboard_pattern() {
	printf("=== Testing Checkerboard Pattern ===\n");
	t_checkers checkers = uv_checkers();
	float u;
	float v;
	t_color result;
	
	printf("Checkers properties: width=%.1f, height=%.1f\n", 
		   checkers.width, checkers.height);
	printf("Color A: "); print_color(checkers.color_a); printf("\n");
	printf("Color B: "); print_color(checkers.color_b); printf("\n\n");
	
	float test_coords[][2] = {
		{0.0, 0.0}, {0.3, 0.3}, {0.7, 0.3}, {0.3, 0.7}, {0.7, 0.7},
		{1.0, 1.0}, {1.3, 1.3}, {1.7, 1.3}, {1.3, 1.7}, {1.7, 1.7}
	};
	
	printf("UV Coordinate Tests:\n");
	for (int i = 0; i < 10; i++) {
		u = test_coords[i][0];
		v = test_coords[i][1];
		result = uv_pattern_at(checkers, u, v);
		printf("UV(%.1f, %.1f) -> ", u, v);
		print_color(result);
		printf("\n");
	}
	// {0, 0, (-1.0)}, {1, 0, 0}, {0, 0, 1}, {-1, 0, 0}, 0, 1, 0},
	float test_coords2[][2] = {
	{0, 0}, {0.5, 0}, {0, 0.5},
		{0.5, 0.5}, {1, 1}
	};

	printf("UV Coordinate Test 2:\n");
	for (int i = 0; i < 5; i++) {
		u = test_coords2[i][0];
		v = test_coords2[i][1];
		result = uv_pattern_at(checkers, u, v);
		printf("UV(%.1f, %.1f) -> ", u, v);
		print_color(result);
		printf("\n");
		if ((i == 1 || i == 2) && (result.x == 255 && result.y == 255 && result.z == 255))
	  		printf("OK\n");
		else if ((i != 1 && 1 != 2) && (result.x == 0))
		   printf("OK\n");
		else
			printf("!!Error!! not the right colour\n");
	}
}

// Performance test function
void performance_test(int iterations) {
    printf("\n=== Performance Test (%d iterations) ===\n", iterations);
    t_checkers checkers = uv_checkers();
    
    clock_t start = clock();
    
    // Variables to prevent compiler optimization
    float sum_r = 0, sum_g = 0, sum_b = 0;
    
    for (int i = 0; i < iterations; i++) {
        // Generate varying UV coordinates
        float u = (float)i / iterations * 10.0;
        float v = (float)(i * 7 % 97) / 97.0 * 10.0;
        
        t_color result = uv_pattern_at(checkers, u, v);
        
        // Use the results to prevent optimization
        sum_r += result.x;
        sum_g += result.y;
        sum_b += result.z;
    }
    
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("Time taken: %f seconds\n", time_spent);
    printf("Operations per second: %f\n", iterations / time_spent);
    
    // Print sums to ensure operations weren't optimized away
    printf("Checksum: %.1f\n", (sum_r + sum_g + sum_b));
}

int main() {
	// Test the checkerboard pattern functionality
	test_checkerboard_pattern();
	
	// Performance test with 10 million iterations
	performance_test(1000000000);
	
	return 0;
}
