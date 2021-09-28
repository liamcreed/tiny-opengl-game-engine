#include "pch.h"
#include "math/types.h"

mat4_t mat4_new(const float value)
{
    mat4_t matrix;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i == j)
                matrix.data[i][j] = value;
            else
                matrix.data[i][j] = 0;
        }
    }
    return matrix;
}
vec2_t vec2_new(const float x, const float y)
{
    vec2_t vec = {x,y};
    return vec;
}
vec3_t vec3_new(const float x, const float y, const float z)
{
    vec3_t vec = {x,y,z};
    return vec;
}
vec4_t vec4_new(const float x, const float y, const float z, const float w)
{
    vec4_t vec = {x,y,z,w};
    return vec;
}
float vec2_dot(const vec2_t a, const vec2_t b)
{
    return a.x * b.x + a.y * b.y;
}
mat4_t mat4_ortho(const float left, const float right, const float top, const float bottom, const float far, const float near)
{
    mat4_t mat4_ret;
    mat4_ret.data[0][0] = 2 / (right - left);
    mat4_ret.data[0][1] = 0;
    mat4_ret.data[0][2] = 0;
    mat4_ret.data[0][3] = 0;
    
    mat4_ret.data[1][0] = 0;
    mat4_ret.data[1][1] = 2 / (top - bottom);
    mat4_ret.data[1][2] = 0;
    mat4_ret.data[1][3] = 0;

    mat4_ret.data[2][0] = 0;
    mat4_ret.data[2][1] = 0;
    mat4_ret.data[2][2] = -2 / (far - near);
    mat4_ret.data[2][3] = 0;
    
    mat4_ret.data[3][0] = -((right + left) / (right - left));
    mat4_ret.data[3][1] = -((top + bottom) / (top - bottom));
    mat4_ret.data[3][2] = -((far + near) / (far - near));
    mat4_ret.data[3][3] = 1;
    
    return mat4_ret;
}
mat4_t mat4_ortho_aspect(const float aspect, const float size, const float far, const float near)
{
    return mat4_ortho(-aspect * size, aspect * size, size, -size, far, near);
}

mat4_t mat4_translate(const mat4_t matrix, const vec3_t vector)
{
    mat4_t result = matrix;
    result.data[0][3] = matrix.data[0][0] * vector.x + matrix.data[0][1] * vector.y + matrix.data[0][2] * vector.z + matrix.data[0][3];
    result.data[1][3] = matrix.data[1][0] * vector.x + matrix.data[1][1] * vector.y + matrix.data[1][2] * vector.z + matrix.data[1][3];
    result.data[2][3] = matrix.data[2][0] * vector.x + matrix.data[2][1] * vector.y + matrix.data[2][2] * vector.z + matrix.data[2][3];
    result.data[3][3] = matrix.data[3][0] * vector.x + matrix.data[3][1] * vector.y + matrix.data[3][2] * vector.z + matrix.data[3][3];
    return result;
}
mat4_t mat4_multiply(const mat4_t mat1, const mat4_t mat2)
{
    mat4_t result;
    result.data[0][0] = mat1.data[0][0]* mat2.data[0][0]+mat1.data[1][0]* mat2.data[0][1]+mat1.data[2][0]*mat2.data[0][2]+mat1.data[3][0]*mat2.data[0][3];

    result.data[0][1] = mat1.data[0][1]* mat2.data[0][0]+mat1.data[1][1]* mat2.data[0][1]+mat1.data[2][1]*mat2.data[0][2]+mat1.data[3][1]*mat2.data[0][3];
    result.data[0][2] = mat1.data[0][2]* mat2.data[0][0]+mat1.data[1][2]* mat2.data[0][1]+mat1.data[2][2]*mat2.data[0][2]+mat1.data[3][2]*mat2.data[0][3];
    result.data[0][3] = mat1.data[0][3]* mat2.data[0][0]+mat1.data[1][3]* mat2.data[0][1]+mat1.data[2][3]*mat2.data[0][2]+mat1.data[3][3]*mat2.data[0][3];

    result.data[1][0] = mat1.data[0][0]* mat2.data[1][0]+mat1.data[1][0]* mat2.data[1][1]+mat1.data[2][0]*mat2.data[1][2]+mat1.data[3][0]*mat2.data[1][3];
    result.data[1][1] = mat1.data[0][1]* mat2.data[1][0]+mat1.data[1][1]* mat2.data[1][1]+mat1.data[2][1]*mat2.data[1][2]+mat1.data[3][1]*mat2.data[1][3];
    result.data[1][2] = mat1.data[0][2]* mat2.data[1][0]+mat1.data[1][2]* mat2.data[1][1]+mat1.data[2][2]*mat2.data[1][2]+mat1.data[3][2]*mat2.data[1][3];
    result.data[1][3] = mat1.data[0][3]* mat2.data[1][0]+mat1.data[1][3]* mat2.data[1][1]+mat1.data[2][3]*mat2.data[1][2]+mat1.data[3][3]*mat2.data[1][3];

    result.data[2][0] = mat1.data[0][0]* mat2.data[2][0]+mat1.data[1][0]* mat2.data[2][1]+mat1.data[2][0]*mat2.data[2][2]+mat1.data[3][0]*mat2.data[2][3];
    result.data[2][1] = mat1.data[0][1]* mat2.data[2][0]+mat1.data[1][1]* mat2.data[2][1]+mat1.data[2][1]*mat2.data[2][2]+mat1.data[3][1]*mat2.data[2][3];
    result.data[2][2] = mat1.data[0][2]* mat2.data[2][0]+mat1.data[1][2]* mat2.data[2][1]+mat1.data[2][2]*mat2.data[2][2]+mat1.data[3][2]*mat2.data[2][3];
    result.data[2][3] = mat1.data[0][3]* mat2.data[2][0]+mat1.data[1][3]* mat2.data[2][1]+mat1.data[2][3]*mat2.data[2][2]+mat1.data[3][3]*mat2.data[2][3];

    result.data[3][0] = mat1.data[0][0]* mat2.data[3][0]+mat1.data[1][0]* mat2.data[3][1]+mat1.data[2][0]*mat2.data[3][2]+mat1.data[3][0]*mat2.data[3][3];
    result.data[3][1] = mat1.data[0][1]* mat2.data[3][0]+mat1.data[1][1]* mat2.data[3][1]+mat1.data[2][1]*mat2.data[3][2]+mat1.data[3][1]*mat2.data[3][3];
    result.data[3][2] = mat1.data[0][2]* mat2.data[3][0]+mat1.data[1][2]* mat2.data[3][1]+mat1.data[2][2]*mat2.data[3][2]+mat1.data[3][2]*mat2.data[3][3];
    result.data[3][3] = mat1.data[0][3]* mat2.data[3][0]+mat1.data[1][3]* mat2.data[3][1]+mat1.data[2][3]*mat2.data[3][2]+mat1.data[3][3]*mat2.data[3][3];
    return result;
}
mat4_t mat4_inverse(const mat4_t matrix)
{
    mat4_t result;
	result.data[0][0] =   matrix.data[0][0];   result.data[0][1] = matrix.data[1][0];  result.data[0][2] = matrix.data[2][0];  result.data[0][3] = 0.0f;
	result.data[1][0] =   matrix.data[0][1];   result.data[1][1] = matrix.data[1][1];  result.data[1][2] = matrix.data[2][1];  result.data[1][3] = 0.0f;
	result.data[2][0] =   matrix.data[0][2];   result.data[2][1] = matrix.data[1][2];  result.data[2][2] = matrix.data[2][2];  result.data[2][3] = 0.0f;
	result.data[3][0] = -(matrix.data[3][0] *  result.data[0][0] + matrix.data[3][1] * result.data[1][0] + matrix.data[3][2] * result.data[2][0]);
	result.data[3][1] = -(matrix.data[3][0] *  result.data[0][1] + matrix.data[3][1] * result.data[1][1] + matrix.data[3][2] * result.data[2][1]);
	result.data[3][2] = -(matrix.data[3][0] *  result.data[0][2] + matrix.data[3][1] * result.data[1][2] + matrix.data[3][2] * result.data[2][2]);
	result.data[3][3] = 1.0f;
	return result;
}
float lerp(const float a, const float b, const float f)
{
    return (a * (1.0 - f)) + (b * f);
}