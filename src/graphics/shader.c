#include "pch.h"

#include "graphics/graphics.h"

const char* glsl_load_from_file(const char* path)
{
    char *buffer = 0;
    long length;
    FILE *f = fopen(path, "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer)
        {
            fread(buffer, 1, length, f);
        }
        fclose(f);
        buffer[length] = '\0';
    }else
    {
        printf(LOG_ERROR"Failed to open file: %s\n", path);
        exit(-1);
    }
    
    if (buffer)  
        return buffer; 
    else 
    {
        printf(LOG_ERROR"Failed write file to string!\n"); 
        exit(-1);
    }
}

void shader_init(shader_t* shader)
{
    if(shader->vert_shader_src == NULL || shader->frag_shader_src == NULL)
    {
        printf(LOG_ERROR"No fragment or vertex shader source code provided when shader_init() was called!\n"); 
        exit(-1);
    }
    uint32_t vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &shader->vert_shader_src, NULL);
    glCompileShader(vert_shader);

    int succes;
    char info_log[512];
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &succes);
    if(!succes)
    {
        glGetShaderInfoLog(vert_shader,  sizeof(info_log), NULL, info_log);
        printf(LOG_ERROR"OPENGL: Failed to compile VERTEX shader: %s\n", info_log); 
    }

    uint32_t frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &shader->frag_shader_src, NULL);
    glCompileShader(frag_shader);

    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &succes);
    if(!succes)
    {
        glGetShaderInfoLog(frag_shader,  sizeof(info_log), NULL, info_log);
        printf(LOG_ERROR"OPENGL: Failed to compile FRAGMENT shader: %s\n", info_log); 
    }
    
    shader->id =  glCreateProgram();
    glAttachShader(shader->id, vert_shader);
    glAttachShader(shader->id, frag_shader);
    glLinkProgram(shader->id);

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    glUseProgram(shader->id);
}
void shader_bind(shader_t* shader)
{
    glUseProgram(shader->id);
}
void shader_destroy(shader_t* shader)
{
    glDeleteProgram(shader->id);
}

void shader_set_uniform_mat4(shader_t* shader, const char* name, mat4_t matrix)
{
    int loc = glGetUniformLocation(shader->id, name);
    if(loc == -1)
        printf(LOG_ERROR"Shader: no uniform with name %s found!\n", name);
    glUniformMatrix4fv(loc, 1, GL_TRUE, &matrix.data[0][0]);
}