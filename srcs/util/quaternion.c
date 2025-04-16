#include "../../include/miniRT.h"

// Create a quaternion from axis-angle representation
t_quat quat_from_axis_angle(t_vec3 axis, float angle)
{
    t_quat q;
    float half_angle = angle * 0.5f;
    float sin_half = sinf(half_angle);
    
    // Normalize the axis
    axis = vec3_normalize(axis);
    
    q.w = cosf(half_angle);
    q.x = axis.x * sin_half;
    q.y = axis.y * sin_half;
    q.z = axis.z * sin_half;
    
    return q;
}

// Normalize a quaternion
t_quat quat_normalize(t_quat q)
{
    t_quat result;
    float length = sqrtf(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
    
    if (length < 0.000001f)
        return (t_quat){1.0f, 0.0f, 0.0f, 0.0f};
    
    result.w = q.w / length;
    result.x = q.x / length;
    result.y = q.y / length;
    result.z = q.z / length;
    
    return result;
}

// Multiply two quaternions
t_quat quat_mul(t_quat a, t_quat b)
{
    t_quat result;
    
    result.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
    result.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
    result.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
    result.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
    
    return result;
}

// Rotate a vector using a quaternion
t_vec3 quat_rotate_vec3(t_quat q, t_vec3 v)
{
    // Create a quaternion from the vector
    t_quat p = {0, v.x, v.y, v.z};
    
    // q * p * q^-1 operation
    // For a unit quaternion, q^-1 = q conjugate = {q.w, -q.x, -q.y, -q.z}
    t_quat q_conj = {q.w, -q.x, -q.y, -q.z};
    t_quat temp = quat_mul(q, p);
    t_quat result = quat_mul(temp, q_conj);
    
    return (t_vec3){result.x, result.y, result.z};
}
