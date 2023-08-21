#pragma once
#include "pch.h"
#include "graphics/graphics.h"
#include "math/types.h"
#include "ecs/ecs.h"

typedef struct
{

    window_t window;
    renderer_t renderer;
    scene_t scene;

} game_data_t;

texture_t character_sheet;
texture_t animations;

sub_texture_t sub_hornet;
sub_texture_t sub_quirell;
sub_texture_t sub_mushroom;

entity_t player;
entity_t npc;
entity_t camera;
#define ENEMY_COUNT 100
entity_t enemies[ENEMY_COUNT];

void start(game_data_t *data)
{
    texture_load(&character_sheet, "res/tex/hornet.png");
    texture_load(&animations, "res/tex/characters.png");

    sub_quirell = sub_texture_create(&character_sheet, vec2_new(2, 1), vec2_new(16, 32));
    sub_hornet = sub_texture_create(&character_sheet, vec2_new(0, 1), vec2_new(16, 32));
    sub_mushroom = sub_texture_create(&animations, vec2_new(1, 0), vec2_new(32, 64));

    scene_init(&data->scene);

    camera = scene_create_entity(&data->scene, "camera");
    data->scene.camera_components[camera].active = true;
    data->scene.camera_components[camera].primary = true;
    data->scene.camera_components[camera].aspect = (float)data->window.width / (float)data->window.height;
    data->scene.camera_components[camera].ortho_size = 4.0f;

    typedef enum
    {
        BACKGROUND,
        MIDDLE,
        FOREGROUND
    } layers;
    
    npc = scene_create_entity(&data->scene, "npc");
    data->scene.transform_components[npc].size.y = 2.0f;
    data->scene.transform_components[npc].position.z = MIDDLE;
    data->scene.transform_components[npc].position.y = -3.0f;
    data->scene.sprite_components[npc].active = true;
    data->scene.sprite_components[npc].texture = &character_sheet;
    data->scene.sprite_components[npc].sub_texture = &sub_quirell;

    data->scene.physics_components[npc].size.x = data->scene.transform_components[npc].size.x;
    data->scene.physics_components[npc].size.y = data->scene.transform_components[npc].size.y;

    data->scene.physics_components[npc].active = true;

    for (uint32_t i = 0; i < ENEMY_COUNT; i++)
    {
        enemies[i] = scene_create_entity(&data->scene, "mushroom");
        data->scene.transform_components[enemies[i]].size.x = 2.0f;
        data->scene.transform_components[enemies[i]].size.y = 4.0f;
        data->scene.transform_components[enemies[i]].position.y = rand() % 100 - 50;
        data->scene.transform_components[enemies[i]].position.x = rand() % 100 - 50;
        data->scene.transform_components[enemies[i]].position.z = MIDDLE;
        data->scene.sprite_components[enemies[i]].active = true;
        data->scene.sprite_components[enemies[i]].texture = &animations;
        data->scene.sprite_components[enemies[i]].sub_texture = &sub_mushroom;

        data->scene.physics_components[enemies[i]].active = true;
        data->scene.physics_components[enemies[i]].size.x = data->scene.transform_components[enemies[i]].size.x;
        data->scene.physics_components[enemies[i]].size.y = data->scene.transform_components[enemies[i]].size.y;
    }

    player = scene_create_entity(&data->scene, "player");
    data->scene.transform_components[player].size.x = 1.0f;
    data->scene.transform_components[player].size.y = 2.0f;
    data->scene.transform_components[player].position.y = 0.0;
    data->scene.transform_components[player].position.z = FOREGROUND;
    data->scene.sprite_components[player].active = true;
    data->scene.sprite_components[player].texture = &character_sheet;
    data->scene.sprite_components[player].sub_texture = &sub_hornet;

    data->scene.physics_components[player].active = true;
    data->scene.physics_components[player].size.x = data->scene.transform_components[player].size.x;
    data->scene.physics_components[player].size.y = data->scene.transform_components[player].size.y;

    printf("%u\n", player);
    printf("%u\n", data->scene.entity_count);
}
void update(game_data_t *data, float delta_time)
{
    float speed = 4.0;
    if (key_pressed(&data->window, KEY_D))
    {
        data->scene.transform_components[player].size.x = 1.0f * fabs(data->scene.transform_components[player].size.x);
        data->scene.physics_components[player].velocity.x = speed;
    }
    else if (key_pressed(&data->window, KEY_A))
    {
        data->scene.transform_components[player].size.x = -1.0f * fabs(data->scene.transform_components[player].size.x);
        data->scene.physics_components[player].velocity.x = -speed;
    }else 
        data->scene.physics_components[player].velocity.x = 0;
    if (key_pressed(&data->window, KEY_W))
    {
        data->scene.physics_components[player].velocity.y = speed;
    }
    else if (key_pressed(&data->window, KEY_S))
    {
        data->scene.physics_components[player].velocity.y = -speed;
    }else 
        data->scene.physics_components[player].velocity.y = 0;
    
    /* typedef struct 
        {
            vec2_t min;
            vec2_t max;
        }aabb;

        aabb a;
        aabb b;

        a.min.x = data->scene.transform_components[player].position.x - (data->scene.transform_components[player].size.x/2);
        a.min.y = data->scene.transform_components[player].position.y - (data->scene.transform_components[player].size.y/2);
        a.max.x = data->scene.transform_components[player].position.x + (data->scene.transform_components[player].size.x/2);
        a.max.y = data->scene.transform_components[player].position.y + (data->scene.transform_components[player].size.y/2);

        b.min.x = data->scene.transform_components[npc].position.x - (data->scene.transform_components[npc].size.x/2);
        b.min.y = data->scene.transform_components[npc].position.y - (data->scene.transform_components[npc].size.y/2);
        b.max.x = data->scene.transform_components[npc].position.x + (data->scene.transform_components[npc].size.x/2);
        b.max.y = data->scene.transform_components[npc].position.y + (data->scene.transform_components[npc].size.y/2);

        if(a.max.x < b.min.x || a.min.x > b.max.x || a.max.y < b.min.y || a.min.y > b.max.y) printf("false\n");
        else printf("true\n"); */

    //camera controller
    static vec2_t last;
    if (mouse_button_pressed(&data->window, MOUSE_BUTTON_LEFT))
    {
        vec2_t offset;

        offset.x = (mouse_position(&data->window).x - last.x) / data->window.width;
        offset.y = (mouse_position(&data->window).y - last.y) / data->window.height;

        if (key_pressed(&data->window, KEY_LEFT_CONTROL))
        {
            data->scene.camera_components[camera].ortho_size += (((-(offset.x) + (offset.y)) / 2) * 10 * data->scene.camera_components[camera].ortho_size);
        }
        else if (key_pressed(&data->window, KEY_LEFT_SHIFT))
        {
            data->scene.transform_components[camera].position.x -= 2 * offset.x * data->scene.camera_components[camera].ortho_size * data->scene.camera_components[camera].aspect;
            data->scene.transform_components[camera].position.y += 2 * offset.y * data->scene.camera_components[camera].ortho_size;
        }
    }
    last = mouse_position(&data->window);

    data->scene.camera_components[camera].aspect = (float)data->window.width / (float)data->window.height;

    
}

void close(game_data_t *game_data)
{
    texture_destroy(&animations);
    texture_destroy(&character_sheet);
}

void init(game_data_t *data)
{
    data->window.width = 1280;
    data->window.height = 800;
    data->window.title = "Hello World";
    data->window.vsync = true;

    data->renderer.max_quad_count = 20000;
    data->renderer.quad_shader.vert_shader_src = glsl_load_from_file("res/shaders/quad.vert");
    data->renderer.quad_shader.frag_shader_src = glsl_load_from_file("res/shaders/quad.frag");
}
