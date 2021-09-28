#include "main.h"

int main(void)
{
    game_data_t data;
    init(&data);
    window_init(&data.window);
    renderer_init(&data.renderer);

    start(&data);
    do
    {
        //calc delta time
        static float last_time;
        float time = (float)glfwGetTime();
        float delta_time = time - last_time;
        last_time = time;

        //framerate
        static float counter;
        counter += delta_time;
        if(counter >= 0.5)
        {
            printf("fps: %d time: %f\n", (int)(1.0f/delta_time), delta_time);
            counter = 0;
        }
        
        renderer_start(&data.renderer);
        
        update(&data, delta_time);
        scene_update(&data.scene, &data.renderer, delta_time);
        
        renderer_end(&data.renderer);
        window_update(&data.window);
    } // 
    while (!data.window.closed);
    
    close(&data);

    //destroy engine
    renderer_destroy(&data.renderer);
    window_destroy(&data.window);

    return 0;
}
