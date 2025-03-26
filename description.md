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

```C++

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

