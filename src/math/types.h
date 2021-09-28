#pragma once

typedef struct 
{
    float data[4][4];
} mat4_t;

typedef struct 
{
    float x, y;
} vec2_t;

typedef struct 
{
    float x, y, z;
} vec3_t;

typedef struct 
{
    float x, y, z, w;
} vec4_t;

mat4_t mat4_new(const float value);
vec2_t vec2_new(const float x, const float y);
vec3_t vec3_new(const float x, const float y, const float z);
vec4_t vec4_new(const float x, const float y, const float z, const float w);

float vec2_dot(const vec2_t a, const vec2_t b);

mat4_t mat4_ortho(const float left, const float right, const float top, const float bottom, const float far, const float near);
mat4_t mat4_ortho_aspect(const float aspect, const float size, const float far, float near);

mat4_t mat4_scale(const mat4_t matrix, const vec3_t vector);
mat4_t mat4_translate(const mat4_t matrix, const vec3_t vector);

mat4_t mat4_multiply(const mat4_t mat1, const mat4_t mat2);
mat4_t mat4_inverse(const mat4_t matrix);


float lerp(const float a, const float b, const float f);