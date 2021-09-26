#include "pch.h"
#include "graphics/graphics.h"

void renderer_init(renderer_t* renderer)
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    renderer->index_count = 0;
    renderer->tex_slot_count = 1;

    //allocate on the heap
    renderer->quad_vertex_buffer = malloc(sizeof(vertex_t) * renderer->max_quad_count * 4);
    renderer->quad_index_buffer = malloc(sizeof(uint32_t) * renderer->max_quad_count * 6);

    //calc indices
    int index_offset = 0;
    for (int i = 0; i < renderer->max_quad_count * 6; i+=6)
    {
        renderer->quad_index_buffer[i +  0] = 0 + index_offset;
        renderer->quad_index_buffer[i +  1] = 1 + index_offset;
        renderer->quad_index_buffer[i +  2] = 2 + index_offset;
        renderer->quad_index_buffer[i +  3] = 2 + index_offset;
        renderer->quad_index_buffer[i +  4] = 3 + index_offset;
        renderer->quad_index_buffer[i +  5] = 0 + index_offset;
        
        index_offset+=4;
    }

    //generate vao, vbo and ibo
    glGenVertexArrays(1, &renderer->quad_vao);
    glBindVertexArray(renderer->quad_vao);

    glGenBuffers(1, &renderer->quad_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * renderer->max_quad_count * 4, NULL,  GL_DYNAMIC_DRAW);

    glGenBuffers(1, &renderer->quad_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->quad_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * renderer->max_quad_count * 6 , renderer->quad_index_buffer,  GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (const void*)offsetof(vertex_t, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (const void*)offsetof(vertex_t, color));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (const void*)offsetof(vertex_t, uv));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (const void*)offsetof(vertex_t, tex_index));
    glEnableVertexAttribArray(3);

    //create white texture
    texture_t white_tex;
    white_tex.width = 1;
    white_tex.height = 1;
    unsigned char white[4] = {255,255,255,255};
    white_tex.data = white; 
    texture_init(&white_tex);
    renderer->tex_slots[0] = white_tex;

    //init shaders
    shader_init(&renderer->quad_shader);
    shader_bind(&renderer->quad_shader);

    int samplers[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    int loc = glGetUniformLocation(renderer->quad_shader.id, "u_textures");
    glUniform1iv(loc, 16, samplers);

}
void renderer_start(renderer_t* renderer)
{
    //assign the pointer to first element of quad buffer
    renderer->quad_vertex_buffer_ptr = renderer->quad_vertex_buffer;
    renderer->index_count = 0;
    renderer->tex_slot_count = 1;
}
void renderer_set_view_proj(renderer_t* renderer, mat4_t view, mat4_t proj)
{
    int loc = glGetUniformLocation(renderer->quad_shader.id, "u_view_mat");
    if(loc == -1)
        printf(LOG_ERROR"Shader: no uniform with name %s found!\n", "u_view_mat");
    glUniformMatrix4fv(loc, 1, GL_TRUE, &view.data[0][0]);

    loc = glGetUniformLocation(renderer->quad_shader.id, "u_proj_mat");
    if(loc == -1)
        printf(LOG_ERROR"Shader: no uniform with name %s found!\n", "u_proj_mat");
    glUniformMatrix4fv(loc, 1, GL_TRUE, &proj.data[0][0]);
}
void renderer_end(renderer_t* renderer)
{
    //calculate size of vertex buffer
    uint32_t size = (uint8_t*)renderer->quad_vertex_buffer_ptr - (uint8_t*)renderer->quad_vertex_buffer;
    glBindBuffer(GL_ARRAY_BUFFER, renderer->quad_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, renderer->quad_vertex_buffer);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (uint32_t i = 0; i < renderer->tex_slot_count; i++)
    {
        texture_bind(&renderer->tex_slots[i], i);
    }

    shader_bind(&renderer->quad_shader);
    
    glBindVertexArray(renderer->quad_vao);
    //printf("%u\n", renderer->index_count);
    glDrawElements(GL_TRIANGLES, renderer->index_count, GL_UNSIGNED_INT, NULL);
}
void renderer_destroy(renderer_t* renderer)
{
    //free allocated memory
    free(renderer->quad_vertex_buffer);
    free(renderer->quad_index_buffer);

    //delete buffers
    glDeleteBuffers(1, &renderer->quad_vbo);
    glDeleteBuffers(1, &renderer->quad_ibo);
    glDeleteVertexArrays(1, &renderer->quad_vao);

    //delete shader
    shader_destroy(&renderer->quad_shader);
}

void draw_quad(renderer_t* renderer, vec3_t position, vec3_t size, vec4_t color)
{
    //white texture
    float tex_index = 0.0f;
    vec2_t no_uv = vec2_new(0.0f, 0.0f);
    renderer->quad_vertex_buffer_ptr->position = vec3_new(position.x - (size.x/2), position.y - (size.y/2), position.z);
    renderer->quad_vertex_buffer_ptr->color = color;
    renderer->quad_vertex_buffer_ptr->uv = no_uv;
    renderer->quad_vertex_buffer_ptr->tex_index = tex_index;
    renderer->quad_vertex_buffer_ptr++;

    renderer->quad_vertex_buffer_ptr->position = vec3_new(position.x + (size.x/2), position.y - (size.y/2), position.z);
    renderer->quad_vertex_buffer_ptr->color = color;
    renderer->quad_vertex_buffer_ptr->uv = no_uv;
    renderer->quad_vertex_buffer_ptr->tex_index = tex_index;
    renderer->quad_vertex_buffer_ptr++;
     
    renderer->quad_vertex_buffer_ptr->position = vec3_new(position.x + (size.x/2), position.y + (size.y/2), position.z);
    renderer->quad_vertex_buffer_ptr->color = color;
    renderer->quad_vertex_buffer_ptr->uv = no_uv;
    renderer->quad_vertex_buffer_ptr->tex_index = tex_index;
    renderer->quad_vertex_buffer_ptr++;

    renderer->quad_vertex_buffer_ptr->position = vec3_new(position.x - (size.x/2), position.y + (size.y/2), position.z);
    renderer->quad_vertex_buffer_ptr->color = color;
    renderer->quad_vertex_buffer_ptr->uv = no_uv;
    renderer->quad_vertex_buffer_ptr->tex_index = tex_index;
    renderer->quad_vertex_buffer_ptr++;
    renderer->index_count+=6;
}

void draw_textured_quad(renderer_t* renderer, vec3_t position, vec3_t size, vec4_t color, texture_t* texture_sheet, sub_texture_t* sub_texture)
{
    //check if texture is already used
    float tex_index = 0.0f;
    for (uint32_t i = 0; i < renderer->tex_slot_count; i++)
    {
        if(renderer->tex_slots[i].id == texture_sheet->id)
        {
            tex_index = (float)i;
            break;
        }
    }
    
    if(tex_index == 0.0f)
    {
        tex_index = (float)renderer->tex_slot_count;
        renderer->tex_slots[renderer->tex_slot_count] = *texture_sheet;
        renderer->tex_slot_count+=1;
    } 

    renderer->quad_vertex_buffer_ptr->position = vec3_new(position.x - (size.x/2), position.y - (size.y/2), position.z);
    renderer->quad_vertex_buffer_ptr->color = color;
    renderer->quad_vertex_buffer_ptr->uv = sub_texture->tex_coords[0];
    renderer->quad_vertex_buffer_ptr->tex_index = tex_index;
    renderer->quad_vertex_buffer_ptr++;

    renderer->quad_vertex_buffer_ptr->position = vec3_new(position.x + (size.x/2), position.y - (size.y/2), position.z);
    renderer->quad_vertex_buffer_ptr->color = color;
    renderer->quad_vertex_buffer_ptr->uv = sub_texture->tex_coords[1];
    renderer->quad_vertex_buffer_ptr->tex_index = tex_index;
    renderer->quad_vertex_buffer_ptr++;
     
    renderer->quad_vertex_buffer_ptr->position = vec3_new(position.x + (size.x/2), position.y + (size.y/2), position.z);
    renderer->quad_vertex_buffer_ptr->color = color;
    renderer->quad_vertex_buffer_ptr->uv = sub_texture->tex_coords[2];
    renderer->quad_vertex_buffer_ptr->tex_index = tex_index;
    renderer->quad_vertex_buffer_ptr++;

    renderer->quad_vertex_buffer_ptr->position = vec3_new(position.x - (size.x/2), position.y + (size.y/2), position.z);
    renderer->quad_vertex_buffer_ptr->color = color;
    renderer->quad_vertex_buffer_ptr->uv = sub_texture->tex_coords[3];
    renderer->quad_vertex_buffer_ptr->tex_index = tex_index;
    renderer->quad_vertex_buffer_ptr++;
    renderer->index_count+=6;
}