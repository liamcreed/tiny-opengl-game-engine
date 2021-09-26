#pragma once
#include "pch.h"
#include "graphics/graphics.h"
#include "math/types.h"
#include "ecs/ecs.h"



typedef struct game_data_t
{
    window_t window;
    renderer_t renderer;

    scene_t scene;
}game_data_t;

void init(game_data_t* game_data)
{
    game_data->window.width        = 1280;
    game_data->window.height       = 800;
    game_data->window.title        = "Hello World";
    game_data->window.vsync        = true;

    game_data->renderer.max_quad_count = 20000;
    game_data->renderer.quad_shader.vert_shader_src = glsl_load_from_file("res/shaders/quad.vert");
    game_data->renderer.quad_shader.frag_shader_src = glsl_load_from_file("res/shaders/quad.frag");
}

sub_texture_t sub_hornet;
sub_texture_t sub_quirell;

texture_t character_sheet;
texture_t terrain_sheet;

entity_t player;
entity_t npc;
entity_t camera;

void start(game_data_t* game_data)
{  
    texture_load(&character_sheet, "res/tex/hornet.png");
    texture_load(&terrain_sheet, "res/tex/terrain.png");

    sub_quirell = sub_texture_create(&character_sheet, vec2_new(2,1), vec2_new(16,32));
    sub_hornet = sub_texture_create(&character_sheet, vec2_new(0,1), vec2_new(16,32));

    scene_init(&game_data->scene);

    npc = scene_create_entity(&game_data->scene);
    game_data->scene.transform_components[npc].size.y = 2.0f;
    game_data->scene.transform_components[npc].position.z = 0.0f;
    game_data->scene.sprite_components[npc].active = true;
    game_data->scene.sprite_components[player].texture =     &character_sheet;
    game_data->scene.sprite_components[player].sub_texture = &sub_quirell;

    player = scene_create_entity(&game_data->scene);
    game_data->scene.transform_components[player].size.y = 2.0f;
    game_data->scene.transform_components[player].position.z = 0.0f;
    game_data->scene.sprite_components[player].active = true;
    game_data->scene.sprite_components[player].texture =     &character_sheet;
    game_data->scene.sprite_components[player].sub_texture = &sub_hornet;
    
    camera = scene_create_entity(&game_data->scene);
    game_data->scene.camera_components[camera].active = true;
    game_data->scene.camera_components[camera].primary = true;
    game_data->scene.camera_components[camera].aspect =  (float)game_data->window.width/(float)game_data->window.height;
    game_data->scene.camera_components[camera].proj_mat = mat4_ortho_aspect(game_data->scene.camera_components[camera].aspect, game_data->scene.camera_components[camera].ortho_size, game_data->scene.camera_components[camera].far, game_data->scene.camera_components[camera].near);

    printf("%u\n", game_data->scene.entity_count);
}
void update(game_data_t* game_data, float delta_time)
{
    //camera controller
    static vec2_t last;
    if(mouse_button_pressed(&game_data->window, MOUSE_BUTTON_LEFT))
    {
        vec2_t offset;

        offset.x = (mouse_position(&game_data->window).x - last.x) / game_data->window.width;
        offset.y = (mouse_position(&game_data->window).y - last.y) / game_data->window.height;

        if(key_pressed(&game_data->window, KEY_LEFT_CONTROL))
        {
            game_data->scene.camera_components[camera].ortho_size += (((-(offset.x) +(offset.y))/2) * 10 * game_data->scene.camera_components[camera].ortho_size);

        }else if(key_pressed(&game_data->window, KEY_LEFT_SHIFT))
        {
            game_data->scene.transform_components[camera].position.x -= 2 * offset.x * game_data->scene.camera_components[camera].ortho_size * game_data->scene.camera_components[camera].aspect;
            game_data->scene.transform_components[camera].position.y += 2 * offset.y * game_data->scene.camera_components[camera].ortho_size;
        }
    }
    last = mouse_position(&game_data->window);
    game_data->scene.camera_components[camera].aspect = (float)game_data->window.width/(float)game_data->window.height;

    

    float speed = 9.81 * delta_time;
    if(key_pressed(&game_data->window, KEY_D))
    {
        game_data->scene.transform_components[player].size.x = 1.0f * fabs(game_data->scene.transform_components[player].size.x);
        game_data->scene.transform_components[player].position.x += speed;
    }
    if(key_pressed(&game_data->window, KEY_A))
    {
        game_data->scene.transform_components[player].size.x =  -1.0f * fabs(game_data->scene.transform_components[player].size.x);
        game_data->scene.transform_components[player].position.x -= speed;
    }
    if(key_pressed(&game_data->window, KEY_W))
    {
        game_data->scene.transform_components[player].position.y += speed;
    }
    if(key_pressed(&game_data->window, KEY_S))
    {
        game_data->scene.transform_components[player].position.y -= speed; 
    }

    for (float i = 0; i < 50; i++)
    {   
        for (float j = 0; j < 50; j++)
        {
            draw_quad(&game_data->renderer, vec3_new(i-25,j-5,0), vec3_new(1,1,0), vec4_new(i/50, j/50, 1.0 - ((j+i)/2)/50, 1.0f));
        }
    }


    scene_update(&game_data->scene, &game_data->renderer);
}

void close(game_data_t* game_data)
{
    texture_destroy(&terrain_sheet);
    texture_destroy(&character_sheet);
}
