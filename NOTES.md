# miniRT

## Camera System & Ray Generation

The camera in a ray tracer is defined by:
- **Position**: Where the camera is located in 3D space
- **Direction**: Where it's looking (forward vector)
- **Up vector**: Which way is "up" from the camera's perspective
- **Field of View (FOV)**: How wide the camera can see

To generate a ray through a pixel:

1. Establish camera's local coordinate system:
   - Forward = direction (normalized)
   - Right = cross product of direction and up
   - Up = possibly recalculated to ensure orthogonality

2. Convert screen coordinates to rays:
   ```
   // For each pixel (u,v) where u,v range from 0 to 1:
   half_fov = FOV_in_radians / 2
   h = tan(half_fov)  // Viewport height factor

   viewport_height = 2 * h
   viewport_width = viewport_height * aspect_ratio

   // Calculate offset from center of viewport
   viewport_u = right_vector * viewport_width * (0.5 - u)
   viewport_v = up_vector * viewport_height * (0.5 - v)

   // Final ray direction
   ray_direction = normalize(direction + viewport_u + viewport_v)
   ```

This maps each pixel to a ray starting at the camera and passing through the corresponding point on an imaginary viewport.

## Object Intersection

### Abstract Object Interface

Common interface for all objects:
```
- intersect(ray): returns distance to intersection, if any
- normal_at(point): returns surface normal at given point
- color(): returns object's base color
```

### Sphere Intersection

The math for ray-sphere intersection:
```
// Given:
// - Sphere with center C and radius r
// - Ray with origin O and direction D

// 1. Calculate vector from ray origin to sphere center
oc = O - C

// 2. Set up quadratic equation coefficients
a = D • D  // dot product (usually 1 if direction is normalized)
b = 2 * (oc • D)
c = (oc • oc) - r*r

// 3. Calculate discriminant
discriminant = b*b - 4*a*c

// 4. If discriminant < 0, no intersection
// Otherwise, calculate intersection distance:
t = (-b - sqrt(discriminant)) / (2*a)
// Check if t > 0 (intersection is in front of ray)
```

To get the surface normal at the intersection point:
```
normal = normalize(intersection_point - sphere_center)
```

### Plane Intersection

Plane intersection is simpler:
```
// Given:
// - Plane with point P0 and normal n
// - Ray with origin O and direction D

// 1. Check if ray is parallel to plane
denom = n • D
if |denom| < epsilon then no intersection

// 2. Calculate intersection distance
t = (n • (P0 - O)) / denom
if t < 0 then intersection is behind ray origin

// Intersection point = O + t*D
```

Normal at any point on a plane is simply the plane's normal vector.

### Cylinder Intersection

Cylinders are more complex as they consist of three parts:
1. The cylindrical body
2. Top cap (circular plane)
3. Bottom cap (circular plane)

Each part needs separate intersection calculations:

For the body:
```
// Calculate vectors perpendicular to cylinder axis
delta_d = D - (D • axis) * axis
delta_p = (O - center) - ((O - center) • axis) * axis

// Set up quadratic equation
a = delta_d • delta_d
b = 2 * delta_p • delta_d
c = delta_p • delta_p - radius*radius
discriminant = b*b - 4*a*c

// Solve and check if points are within cylinder height
```

For caps:
```
// Check intersection with plane at cap position
// Then verify point is within cap radius
```

Normal calculation varies:
- On body: towards cylinder axis
- On caps: axis direction (top) or negative axis direction (bottom)

## Lighting and Shading

### Basic Components

1. **Ambient Light**: Global illumination applied everywhere
2. **Point Lights**: Light sources at specific positions

### Lighting Calculation

For each intersection point:
```
final_color = ambient_color * ambient_intensity

for each light:
    // Calculate vector to light
    light_dir = normalize(light_position - intersection_point)

    // Check for shadows (ray from point to light)
    if not in_shadow:
        // Diffuse lighting (Lambert's law)
        light_intensity = max(0, normal • light_dir)
        diffuse = object_color * light_color * light_intensity * light_brightness

        final_color += diffuse
```

### Shadow Calculation

To determine if a point is in shadow:
```
// Create shadow ray
shadow_origin = intersection_point + normal * small_offset  // avoid self-intersection
shadow_direction = normalize(light_position - shadow_origin)

// Check all objects for intersection
distance_to_light = |light_position - shadow_origin|

for each object:
    if object.intersect(shadow_ray) < distance_to_light:
        return "in shadow"
```

## Scene Rendering Process

1. For each pixel:
   - Generate ray from camera through pixel
   - Find closest object intersection
   - Calculate lighting at intersection point
   - Determine final pixel color

2. Find closest intersection:
   ```
   closest_distance = infinity
   closest_object = null

   for each object:
       if object.intersect(ray) < closest_distance:
           update closest_distance and closest_object
   ```

3. Calculate color at intersection point:
   - Apply ambient light
   - For each light source:
     - Check if point is in shadow
     - If not, add diffuse lighting contribution

## Steps for Implementation

1. Implement vector mathematics (Vec3 class)
2. Create Ray and Camera classes
3. Define Object interface and implement specific shapes
4. Add lighting calculations
5. Implement the main render loop
6. Handle file I/O for scene description
