# formulas and how we used them

## intersection

as the comments in the code say, this is how we expanded the P = O + t * D
when we had 
```
	D^2*t^2 + 2OC tD + (OC)^2 - R^2 = 0
```
then from this you can actually use the quadratic formula to extract the values (just think of t as x)
```
	D^2*t^2 + 2OC tD + (OC)^2 - R^2 = 0
		a		b		c
```
that means:
(for a, since a normalized vector * normalised vector is still 1, we can actually drop it. thats why we only need b and c)
```
	 a = 1 (|normal vector|² = 1) = D²
	 b = 2 * oc * D
	bc[B] = 2 * vec3_dot(oc, ray.direction);
	 c = oc² - r²
```
so the formula actually more complicated:
```
x = (-b +- sqrt(b^2 - 4ac)) / 2a
```
but the discriminant (that means the number that shows if there is an intersection at all) is just:
```
(b^2 - 4ac)
```
so in the code me do:

	// disc = b*b - 4*1*c
	discriminant = bc[B] * bc[B] - 4 * bc[C];

```C++
bool	sphere_intersect(t_sphere *sphere, t_ray ray, t_intersection *out)
{
	t_vec3			oc;
	float			bc[2];
	float			discriminant;
	float			sqrt_d;
	float			t[2];

	// P = O + t * D
	// (P - C)² = R²
	// (O + t * D - C)² = R²
	// (O-C + t * D)² = R²
	// D^2*t^2 + 2OC tD + (OC)^2 - R^2 = 0
	oc = vec3_subtract(ray.origin, sphere->pos);
	// a = 1 (|normal vector|² = 1) = D²
	// b = 2 * oc * D
	bc[B] = 2 * vec3_dot(oc, ray.direction);
	// c = oc² - r²
	bc[C] = vec3_squared_length(oc) - sphere->radius * sphere->radius;
	// disc = b*b - 4*1*c
	discriminant = bc[B] * bc[B] - 4 * bc[C];
	if (discriminant < 0)
		return (false);
	if (discriminant == 1.0)
	{
		// -b / 2*1
		t[B] = -bc[B] * 0.5;
		t[C] = t[B];
	}
	else
	{
		sqrt_d = sqrtf(discriminant);
		// (-b +- sqrt(disc)) / 2*1
		t[B] = (-bc[B] - sqrt_d) * 0.5;
		t[C] = (-bc[B] + sqrt_d) * 0.5;
	}
	// we want the closest intersection point thats not behind the camera/ ray origin
	if (t[B] < 0)
		t[B] = t[C];
	// if there is none we dont have an intersection in the ray's direction
	if (t[B] < 0)
		return (false);
	out->distance = t[B];
	// P = O + t * D
	out->point = vec3_add(ray.origin, vec3_multiply(ray.direction, out->distance));
	out->object = (t_object){.sphere = sphere, .type = SPHERE};
	return (true);
}
```

## colors and vectors:

our colours and vectors are all in the vec3 struct. and we calculate with them in a normalized way, so even colours get normalized to be between 0 and 1 (normal rdg values are between 0 and 255)

## notes on optimization:

- pass by value if struct is <= 16bytes (up to max 32bytes)
- throw all structs though alignment optimization

## two hit points with the formula

``` C++
t[0] = (-abc[B] - sqrt_d) / (2 * abc[A]);
t[1] = (-abc[B] + sqrt_d) / (2 * abc[A]);
```

will t[0] always be the entry?
yes, if the ray is normalized!!!!
so we should indeed normalize the ray for cylinders before.


    If abc[A] > 0 (which is typical for a properly normalized ray direction):
        t[0] will always be smaller than t[1]
        t[0] represents the "entry" point into the cylinder
        t[1] represents the "exit" point from the cylinder

    If abc[A] < 0 (which can happen in certain edge cases):
        t[0] will actually be larger than t[1]
        The roles are reversed: t[1] is the entry and t[0] is the exit

if we don't normalize before, we need to sort:
```
// Sort intersections to ensure we check the closer one first
if (t[0] > t[1])
{
    float temp = t[0];
    t[0] = t[1];
    t[1] = temp;
}
```
## cylinder

the reason we do this is, the substraction creates a vector from the top cap of the cylinder to the point that was hit by the ray.
when we dot that with a vector, that is NORMALIZED, we get the projection of the vector onto the axis of the cylinder.
because
a · b = |a| × |b| × cos(θ)

Where θ is the angle between them.

When b is a unit vector (normalized, as cylinder->axis should be), this simplifies to:
Code

a · b = |a| × cos(θ)

		hit_proj = vec3_dot(vec3_subtract(hit_point, cylinder->top), cylinder->axis);
		if (hit_proj <= 0 && hit_proj >= -cylinder->height)
so what we and up with is a scaled projection of the vector onto the axis of the cylinder.

we calculate the length of a, then we scale it with cos(θ) and now that our a is kind of normalized, and we scaled it, it acutally is the corresponding projection

### meaning of the coeficcients

    A coefficient (coef[A]):
        Represents the squared length of the ray direction vector perpendicular to the cylinder axis
        Geometrically: How fast the ray is moving toward/away from the cylinder axis
        If A = 0: Ray is traveling parallel to the cylinder surface (special case)

    B coefficient (coef[B]):
        Represents twice the initial "approach velocity" - how quickly the ray is initially approaching/leaving the cylinder axis
        Combines the ray direction and the initial position relative to the cylinder
        If B = 0: Ray starts at a point where its distance to the cylinder axis is momentarily constant

    C coefficient (coef[C]):
        Represents the initial squared distance between ray origin and cylinder surface
        Specifically: (distance from ray origin to cylinder axis)² - (cylinder radius)²
        If C = 0: Ray starts exactly on the cylinder surface
        If C < 0: Ray starts inside the cylinder

	(fabs(abc[C]) < EPSILON)) 
means the ray origin on surface and therefore if we return false here, we prevent shadow acne

	if (fabs(abc[A]) < EPSILON)
means a is basically zero and the whole things turns into a linear quations instead! that is more easily solvable and doesn't require a root.
but if B is 
		if (fabs(abc[B]) < EPSILON)
then that would mean a divigion by zero or something really small, which would be bad or impossible.


The standard quadratic formula can produce completely incorrect results in some edge cases due to catastrophic cancellation.
therefore we could use the citardauq formula.

    When B is negative, we compute q using (-B - √(...))
    When B is positive, we compute q using (-B + √(...))
    Then we use q to compute both roots in a way that maintains precision

	if (coef[1] < 0)
		q = -0.5 * (coef[1] - discriminant);
	else
		q = -0.5 * (coef[1] + discriminant);
	t[0] = q / coef[0];
	t[1] = coef[2] / q;

but this works well for us:

	t[0] = (-coef[1] + sqrt(discriminant)) / (2 * coef[0]);
	t[1] = (-coef[1] - sqrt(discriminant)) / (2 * coef[0]);

	If B is very large compared to the discriminant, we lose precision.

## plane

we decided to have a double sided plane, meaning you can see it from both sides
thats why we do 
	if (denom > EPSILON || denom < -EPSILON)
that means we are checking if the denominator is not 0, that means the plane is not perpendicular to us.
since denom is positiv if the view direction is in line with the plane normal, and its negative, if the plane normal points in the other direction.
so if it points in the other direction, we thought, we should still be able to see it.
```C++
bool	plane_intersect(t_plane *plane, t_ray ray, t_intersection *out)
{
	float	t;
	float	denom;

	denom =  vec3_dot(plane->normal, ray.direction);
	if (denom > EPSILON || denom < -EPSILON)
	{
		t = vec3_dot(vec3_subtract(plane->pos, ray.origin), plane->normal) / denom;
		if (!interval_contains(ray.range, t))
			return (false);
		out->distance = t;
		out->object.plane = plane;
		out->object.type = PLANE;
		out->point = vec3_add(ray.origin, vec3_multiply(ray.direction, t));
		return (true);
	}
	return (false);
}
```
## specular
srcs:
https://medium.com/@lennnart/vex-ray-tracer-4-blinn-phong-shading-f0bf3042ba92
https://www.cg.tuwien.ac.at/courses/EinfVisComp/Skriptum/SS13/EVC-21%20Beleuchtung%20+%20Schattierung.pdf

this is the formula for the specular skalar, that skales the light reflection.
I spec = ks * I * cos^p(σ) = ks * I * (r dot v)
this is the "specular" in our code.
 and as you can see, the part with cosinus actually gets replaced with (r dot v)
with v being the view direction (our ray direction) and r being the reflection direction.
But we don't have the reflection direction.
However, there is an approximation for the angle that comes out of that dot product, it is:
n * h ≈ r * v
n is the surface normal, and h we can get, it is the half direction in between the light direction and the ray direction (ray direciton  = view direction)

please refer to the image in the second link under sources for further info.

but what is ks? that is the reflection koefficient of the material.

usually you would shoot another ray from that location, see what it hits and get the reflected color from that object. but we are not doing it recusively, to save time. therefore this simplifies the calculation as we just use white light:
``` C++
				spec_color = vec3_multiply((t_vec3){1, 1, 1}, specular);

```
## light

The Phong reflection model consists of three components:

    Ambient Reflection: Constant light that illuminates all surfaces regardless of light sources
    Code

Ambient = Ka * Ia   (where Ka is ambient coefficient, Ia is ambient intensity)

Diffuse Reflection: Light that is scattered equally in all directions (Lambertian)
Code

Diffuse = Kd * Id * max(0, N·L)

    Kd: diffuse coefficient (material property)
    Id: diffuse intensity (light source property)
    N: surface normal
    L: light direction (from surface to light)

Specular Reflection: Shiny highlights that depend on viewing direction
Code

Specular = Ks * Is * (max(0, R·V))^shininess

    Ks: specular coefficient (material property) (can also be object colour, but then it's not really real)
    Is: specular intensity (light source property) (light brightness)
    R: reflection vector = 2*(N·L)*N - L
    V: view vector (from surface to viewer/camera) (meaning ray inverted)
    shininess: material property (higher = sharper highlights)

actually, there are two approaches:

Original Phong Model

Specular = Ks * Is * (max(0, R·V))^shininess
where R = 2*(N·L)*N - L

Blinn-Phong Model (the half-vector approach)

Specular = Ks * Is * (max(0, N·H))^shininess
where H = normalize(L + V)

	mixed_color = vec3_component_mul(MATERIAL_COLOR, l_color);
	specular = powi(fmaxf(vec3_dot(its.normal, half_dir), 0.0f), 128);
	spec_color = vec3_multiply(vec3_component_mul((t_color){0.9, 0.9, 0.9}, light.color), specular * l.attenuation);

last line can have mixed_color instead of the component mul

### shininess

For non-metals: white/gray specular color (0.9, 0.9, 0.9) with moderate shininess (16-32)
For metals: colored specular matching the metal color with high shininess (64-128)

If using Blinn-Phong, use 4x higher shininess than original Phong
Example: 32 in original Phong ≈ 128 in Blinn-Phong

## cone

formula:
```
C is the vertex of the cone
V is the axis vector
k is the tangent of half angle of the cone

 A ray is defined in the following way:

   P = O + D*t
and 
   P - C = D*t + X
where C is a center point of a shape that we hit and X equals O-C. 


   a   = D|D - (1+k*k)*(D|V)^2
   b/2 = D|X - (1+k*k)*(D|V)*(X|V)
   c   = X|X - (1+k*k)*(X|V)^2

```

therefore:
```
a = (d dot d) - (1 + slant * slant) * (d dot axis)^2
b = root((d dot oc) - (1 + slant * slant) * (d dot axis) (oc dot axis)
c = (oc dot oc) - (1 + slant * slant) * (oc dot axis)^2
```

but in our case, when dealing with rays, discriminant says the following:

    If discriminant < 0, the ray misses the cone completely (no real solutions)
    If discriminant = 0, the ray grazes the cone (exactly one intersection point)
    If discriminant > 0, the ray intersects the cone at two points


If a=0a=0, it implies that the ray is parallel to one of the cone's sides.
A larger value of a indicates that the ray direction diverges more from the cone's axis.
this check when calculating the discriminator:

```
	if (fabs(abc[A]) < EPSILON)
	{
		// Special case - solve linear equation
		if (fabs(abc[B]) < EPSILON)
			return (false);  // No solution
		t[0] = -abc[C] / abc[B];
		t[1] = t[0]; // Both solutions are the same
		return ();
	}
```
is an optimization, since when A is 0, we don't have to do a root, we can solve a liniare equation.


since i implemented both original_phong and blinn phong here is how the function needs to be when using original phong:
```
t_color	calc_lights(const t_light light, const t_ray ray,
					t_intersection its, t_light_ray l)
{
	t_color	diffuse;
	t_color	spec_color;
	float	n_dot_l;
	t_vec3	reflection;

	l.attenuation = light.brightness / (l.distance * l.distance / LIGHT_DIST);
	diffuse = vec3_component_mul(object_color(its.object), light.color);
	diffuse = vec3_multiply(diffuse, l.lambert * l.attenuation);
	n_dot_l = vec3_dot(its.normal, l.direction);
	reflection = vec3_subtract(
			vec3_multiply(its.normal, 2.0f * n_dot_l),
			l.direction
			);
	spec_color = original_phong(
			vec3_multiply(ray.direction, -1.0),
			l.attenuation, light.color, reflection
			);
	return (vec3_add(diffuse, spec_color));
}
```
and here it is how it has to be to use the Blinn-Phong Model:
```


t_color	calc_lights(const t_light light, const t_ray ray,
					t_intersection its, t_light_ray l)
{
	t_color	diffuse;
	t_color	spec_color;

	l.attenuation = light.brightness / (l.distance * l.distance / LIGHT_DIST);
	diffuse = vec3_component_mul(object_color(its.object), light.color);
	diffuse = vec3_multiply(diffuse, l.lambert * l.attenuation);
	spec_color = blinn_phong(vec3_normalize(
			vec3_add(l.direction, vec3_multiply(ray.direction, -1.0))),
	 		its.normal, l.attenuation, light.color);
	return (vec3_add(diffuse, spec_color));
}
```

# Patterns

## checkerboard

http://www.raytracerchallenge.com/bonus/texture-mapping.html

my attempt is modeled after the above link


testing for speed:
 in the file "checker_test.c"
speed testing gave me the following results for some modifications of the function (as this function will have to be called so much, i would like to optimize)

Original Implementation: 151,579,090 ops/sec (6.60 seconds)

```C
u2 = (int)floor(u * checkers.width);
v2 = (int)floor(v * checkers.height);
if ((u2 + v2) & 1)
```
Optimized Version with Negative Handling: 139,389,704 ops/sec (7.17 seconds)

```C
u *= checkers.width;
v *= checkers.height;
int u2 = (int)u - (u < 0 && u != (int)u);
int v2 = (int)v - (v < 0 && v != (int)v);
return (u2 ^ v2) & 1 ? checkers.color_a : checkers.color_b;
```

Optimized Version for Positive Numbers Only: 163,494,812 ops/sec (6.12 seconds)

```C
u *= checkers.width;
v *= checkers.height;
u2 = (int)u;
v2 = (int)v;
if ((u2 + v2) & 1)
```

so the last one is the fastest, but it means taking a certain risk and assuming my numbers are ALWAYS positive (which u v coords usually are)

## planar texture-mapping

Fmod Performance:
Time taken: 0.207405 seconds
Operations per second: 48214845.350884

```C
// The planar mapping function using fmod
t_point planar_map_fmod(const t_vec3 plane_point)
{
    t_point pt;

    pt.min = fmod(plane_point.x, 1.0);
    pt.max = fmod(plane_point.z, 1.0);
    pt.min += (pt.min < 0) * 1.0f;
    pt.max += (pt.max < 0) * 1.0f;
    return pt;
}
```

Modf Performance:
Time taken: 0.204562 seconds
Operations per second: 48884934.640842

```C
// Alternative implementation using modf
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
```

ownft Performance:
Time taken: 0.167835 seconds
Operations per second: 59582327.881550

```C
t_point	planar_map(const t_vec3 plane_point)
{
	t_point pt;

    pt.min = plane_point.x - floorf(plane_point.x);
    pt.max = plane_point.z - floorf(plane_point.z);
	return (pt);
}
```

this last one can leave out the handling of negative numbers since that is 

# Bump mapping

srcs: https://medium.com/@muhammedcan.erbudak/ray-tracing-from-scratch-texture-normal-bump-mapping-22ece96038bf
