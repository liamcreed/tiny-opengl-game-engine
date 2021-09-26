#include "pch.h"
#include "graphics/graphics.h"

void window_init(window_t* window)
{
    if(!glfwInit())
    {
        printf(LOG_ERROR"Failed to initialize GLFW!\n");
        exit(-1);
    }

    //set window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    

    window->glfw_window_p = glfwCreateWindow(window->width, window->height, window->title, NULL, NULL);
    if(window == NULL)
    {
        printf(LOG_ERROR"Failed to create glfw window!\n");
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window->glfw_window_p);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf(LOG_ERROR"Failed to initialize glad!\n");
        exit(-1);
    }

    glfwSwapInterval(window->vsync);
    window->minimized = false;
    window->resized = false;

    window->closed = false;
}

void window_update(window_t* window)
{
    glfwPollEvents();
    glfwSwapBuffers(window->glfw_window_p);

    if(glfwWindowShouldClose(window->glfw_window_p))
        window->closed = true;
    else
        window->closed = false;

    //NOTE: i don't know if this is optimal but its fine
    int w, h;
    glfwGetWindowSize(window->glfw_window_p, &w, &h);
    if(window->width != w || window->height != h)
    {
        window->resized = true;
        window->width = w;
        window->height = h;
    }
    else
        window->resized = false;
}
void window_destroy(window_t* window)
{
    if(!window->closed)
    {
        glfwSetWindowShouldClose(window->glfw_window_p, true);
        window->closed = true;
    }
    glfwDestroyWindow(window->glfw_window_p);
    glfwTerminate();
}


bool key_pressed(window_t* window, key key)
{
    if(glfwGetKey(window->glfw_window_p, key) == GLFW_PRESS)
        return true;
    
    return false;
}

bool mouse_button_pressed(window_t* window, mouse_button button)
{
    if(glfwGetMouseButton(window->glfw_window_p, button) == GLFW_PRESS)
        return true;

    return false;
}

vec2_t mouse_position(window_t* window)
{
    double x, y;
    glfwGetCursorPos(window->glfw_window_p, &x, &y);
    return vec2_new(x, y);
}