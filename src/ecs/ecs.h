#pragma once

#define MAX_ENTITIES 1024

typedef struct base_component_t
{
    bool active;

    const char* name;
    const char* tag;
}base_component_t;

typedef struct transform_component_t
{
    vec3_t position;
    vec3_t size;
}transform_component_t;

typedef struct sprite_component_t
{
    bool active;
    
    vec4_t tint_color;
    texture_t *texture;
    sub_texture_t *sub_texture;
}sprite_component_t;

typedef struct camera_component_t
{
    bool active;
    bool primary;

    mat4_t view_mat;
    mat4_t proj_mat;

    float far, near;

    float aspect;
    float ortho_size;
}camera_component_t;






typedef uint32_t entity_t;

typedef struct scene_t
{
    uint32_t entity_count;
    base_component_t      base_componenents    [MAX_ENTITIES];
    transform_component_t transform_components [MAX_ENTITIES];
    sprite_component_t    sprite_components    [MAX_ENTITIES];
    camera_component_t    camera_components    [MAX_ENTITIES];
}scene_t;

entity_t scene_create_entity(scene_t* scene)
{
    if(scene->entity_count >= MAX_ENTITIES)
    {
        printf(LOG_ERROR"Maximum entities reached!\n");
        exit(-1);
    }
    
    entity_t id = scene->entity_count;
    scene->entity_count+=1;
    return id;
}

void scene_init(scene_t* scene)
{
    scene->entity_count = 0;

    for (entity_t e = 0; e < MAX_ENTITIES; e++)
    {
        scene->base_componenents[e].active      =  true;
        scene->base_componenents[e].name        =  "entity";
        scene->base_componenents[e].tag         =  "";

        scene->transform_components[e].position =  vec3_new(0.0f ,0.0f, 0.0f);
        scene->transform_components[e].size     =  vec3_new(1.0f, 1.0f, 0.0f);

        scene->sprite_components[e].active      =  false;
        scene->sprite_components[e].tint_color  =  vec4_new(1.0f, 1.0f, 1.0f, 1.0f);
        scene->sprite_components[e].texture     =  NULL;
        scene->sprite_components[e].sub_texture =  NULL;

        scene->camera_components[e].active      =  false;
        scene->camera_components[e].primary     =  false;
        scene->camera_components[e].far         =  1.0f;
        scene->camera_components[e].near        = -1.0f;
        scene->camera_components[e].ortho_size  =  1.0f;
        scene->camera_components[e].view_mat    =  mat4_new(1.0f);
        scene->camera_components[e].proj_mat    =  mat4_ortho_aspect(1.0f, 1.0f, 1.0f, -1.0f);
    }
}
void scene_update(scene_t *scene, renderer_t *renderer)
{
    for (entity_t e = 0; e < scene->entity_count; e++)
    {
        if (scene->base_componenents[e].active)
        {
            //update cameras
            if (scene->camera_components[e].active && scene->camera_components[e].primary)
            {
                scene->camera_components[e].view_mat = mat4_translate(mat4_new(1.0f), vec3_new(-scene->transform_components[e].position.x, -scene->transform_components[e].position.y, 0.0f));
                scene->camera_components[e].proj_mat = mat4_ortho_aspect(scene->camera_components[e].aspect, scene->camera_components[e].ortho_size, scene->camera_components[e].far, scene->camera_components[e].near);
                renderer_set_view_proj(renderer, scene->camera_components[e].view_mat, scene->camera_components[e].proj_mat);
            }
            //render
            if (scene->sprite_components[e].active)
            {
                if (scene->sprite_components[e].texture == NULL || scene->sprite_components[e].sub_texture == NULL)
                {
                    draw_quad(renderer, scene->transform_components[e].position, scene->transform_components[e].size, scene->sprite_components[e].tint_color);
                }
                else
                {
                    draw_textured_quad(renderer, scene->transform_components[e].position, scene->transform_components[e].size, scene->sprite_components[e].tint_color, scene->sprite_components[e].texture, scene->sprite_components[e].sub_texture);
                }
            }
        }
    }
}