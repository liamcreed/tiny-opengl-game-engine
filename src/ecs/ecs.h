#pragma once

#define MAX_ENTITIES 4096

typedef struct
{
    bool active;

    const char *name;
    const char *tag;
} base_component_t;

typedef struct
{
    vec3_t position;
    vec3_t size;
} transform_component_t;

typedef struct
{
    bool active;

    vec4_t tint_color;
    texture_t *texture;
    sub_texture_t *sub_texture;
} sprite_component_t;

typedef struct
{
    bool active;
    bool primary;

    float aspect;
    float ortho_size;
} camera_component_t;

typedef struct
{
    bool active;
    vec2_t velocity;
    float restitution;
    float mass;

    //collider
    bool collider;
    vec2_t size;
    vec2_t min;
    vec2_t max;
} physics_component_t;

typedef uint32_t entity_t;

typedef struct
{
    uint32_t entity_count;
    base_component_t base_componenents[MAX_ENTITIES];
    transform_component_t transform_components[MAX_ENTITIES];
    sprite_component_t sprite_components[MAX_ENTITIES];
    camera_component_t camera_components[MAX_ENTITIES];
    physics_component_t physics_components[MAX_ENTITIES];
} scene_t;

entity_t scene_create_entity(scene_t *scene, const char *name)
{

    if (scene->entity_count > MAX_ENTITIES)
    {
        printf(LOG_ERROR "Maximum entities reached!\n");
        exit(-1);
    }

    entity_t id = scene->entity_count;
    scene->base_componenents[id].active = true;
    scene->base_componenents[id].name = name;
    scene->base_componenents[id].tag = "";

    scene->transform_components[id].position = vec3_new(0.0f, 0.0f, 0.0f);
    scene->transform_components[id].size = vec3_new(1.0f, 1.0f, 0.0f);

    scene->sprite_components[id].active = false;
    scene->sprite_components[id].tint_color = vec4_new(1.0f, 1.0f, 1.0f, 1.0f);
    scene->sprite_components[id].texture = NULL;
    scene->sprite_components[id].sub_texture = NULL;

    scene->physics_components[id].active = false;
    scene->physics_components[id].mass = 1.0f;
    scene->physics_components[id].restitution = 1.0f;
    scene->physics_components[id].collider = true;
    scene->physics_components[id].size = vec2_new(scene->transform_components[id].size.x, scene->transform_components[id].size.y);
    scene->physics_components[id].min.x = scene->transform_components[id].position.x - (scene->physics_components[id].size.x / 2);
    scene->physics_components[id].min.y = scene->transform_components[id].position.y - (scene->physics_components[id].size.y / 2);
    scene->physics_components[id].max.x = scene->transform_components[id].position.x + (scene->physics_components[id].size.x / 2);
    scene->physics_components[id].max.y = scene->transform_components[id].position.y + (scene->physics_components[id].size.y / 2);

    scene->camera_components[id].active = false;
    scene->camera_components[id].primary = false;
    scene->camera_components[id].aspect = 1.0f;
    scene->camera_components[id].ortho_size = 1.0f;

    scene->entity_count += 1;
    return id;
}

void scene_init(scene_t *scene)
{
    scene->entity_count = 0;
}

void scene_update(scene_t *scene, renderer_t *renderer, float delta_time)
{
    for (entity_t e = 0; e < scene->entity_count; e++)
    {
        if (scene->base_componenents[e].active)
        {
            //update physics
            {
                if (scene->physics_components[e].active)
                {
                    //check for collision
                    if (scene->physics_components[e].collider)
                    {
                        for (entity_t f = 0; f < scene->entity_count; f++)
                        {
                            if (e != f && scene->physics_components[f].active && scene->physics_components[f].collider)
                            {
                                scene->physics_components[e].min.x = scene->transform_components[e].position.x - (scene->physics_components[e].size.x / 2);
                                scene->physics_components[e].min.y = scene->transform_components[e].position.y - (scene->physics_components[e].size.y / 2);
                                scene->physics_components[e].max.x = scene->transform_components[e].position.x + (scene->physics_components[e].size.x / 2);
                                scene->physics_components[e].max.y = scene->transform_components[e].position.y + (scene->physics_components[e].size.y / 2);

                                scene->physics_components[f].min.x = scene->transform_components[f].position.x - (scene->physics_components[f].size.x / 2);
                                scene->physics_components[f].min.y = scene->transform_components[f].position.y - (scene->physics_components[f].size.y / 2);
                                scene->physics_components[f].max.x = scene->transform_components[f].position.x + (scene->physics_components[f].size.x / 2);
                                scene->physics_components[f].max.y = scene->transform_components[f].position.y + (scene->physics_components[f].size.y / 2);

                                if (!(scene->physics_components[e].max.x < scene->physics_components[f].min.x 
                                    || scene->physics_components[e].min.x > scene->physics_components[f].max.x 
                                    || scene->physics_components[e].max.y < scene->physics_components[f].min.y 
                                    || scene->physics_components[e].min.y > scene->physics_components[f].max.y))
                                {
                                    printf("%u %u\n", e, f);
                                    scene->sprite_components[e].tint_color = vec4_new(1.0f, 0.0f, 0.0f, 1.0f);
                                    scene->sprite_components[f].tint_color = vec4_new(1.0f, 0.0f, 0.0f, 1.0f);
                                }
                                else
                                {
                                    // no collision
                                    scene->sprite_components[f].tint_color = vec4_new(1.0f, 1.0f, 1.0f, 1.0f);
                                }
                            }
                        }
                    }
                    scene->transform_components[e].position.x += scene->physics_components[e].velocity.x * delta_time;
                    scene->transform_components[e].position.y += scene->physics_components[e].velocity.y * delta_time;
                }

                //update cameras
                {

                    if (scene->camera_components[e].active && scene->camera_components[e].primary)
                    {
                        mat4_t view_mat = mat4_translate(mat4_new(1.0f), vec3_new(-scene->transform_components[e].position.x, -scene->transform_components[e].position.y, 0.0f));
                        mat4_t proj_mat = mat4_ortho_aspect(scene->camera_components[e].aspect, scene->camera_components[e].ortho_size, 100.0f, -100.0f);
                        renderer_set_view_proj(renderer, view_mat, proj_mat);
                    }
                }

                //render
                {
                    /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */
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
    }
}
