#include "pch.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include "graphics/graphics.h"

void texture_load(texture_t* texture, const char* path)
{
    stbi_set_flip_vertically_on_load(true);
    texture->data = stbi_load(path, &texture->width, &texture->height, &texture->channel_count, 0);

    if(texture->data == NULL)
    {
        printf(LOG_ERROR"Failed to load texture: %s/n", path);
        exit(-1);
    }
    texture_init(texture);
}
void texture_init(texture_t* texture)
{
    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data);
    glGenerateMipmap(GL_TEXTURE_2D);

}
void texture_bind(texture_t* texture, uint32_t index)
{
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}
void texture_destroy(texture_t* texture)
{
    free(texture->data);
}

sub_texture_t sub_texture_create(texture_t* texture, vec2_t coords, vec2_t size)
{
    vec2_t min = {(coords.x * size.x) / texture->width, (coords.y * size.y) / texture->height };
    vec2_t max = {((coords.x + 1) * size.x) / texture->width, ((coords.y + 1) * size.y) / texture->height };

    sub_texture_t sub_texture;
    sub_texture.tex_coords[0] = vec2_new(min.x,  min.y);
    sub_texture.tex_coords[1] = vec2_new(max.x,  min.y);
    sub_texture.tex_coords[2] = vec2_new(max.x,  max.y);
    sub_texture.tex_coords[3] = vec2_new(min.x,  max.y);
    return sub_texture;
}