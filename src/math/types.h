#pragma once

typedef struct mat4_t
{
    float data[4][4];
} mat4_t;

typedef struct vec2_t
{
    float x, y;
} vec2_t;

typedef struct vec3_t
{
    float x, y, z;
} vec3_t;

typedef struct vec4_t
{
    float x, y, z, w;
} vec4_t;

mat4_t mat4_new(float value);
vec2_t vec2_new(float x, float y);
vec3_t vec3_new(float x, float y, float z);
vec4_t vec4_new(float x, float y, float z, float w);

mat4_t mat4_ortho(float left, float right, float top, float bottom, float far, float near);
mat4_t mat4_ortho_aspect(float aspect, float size, float far, float near);

mat4_t mat4_scale(mat4_t matrix, vec3_t vector);
mat4_t mat4_translate(mat4_t matrix, vec3_t vector);

mat4_t mat4_multiply(mat4_t mat1, mat4_t mat2);
mat4_t mat4_inverse(mat4_t matrix);


float lerp(float a, float b, float f);