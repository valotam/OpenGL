Modern OpenGL Guide, SETUP
==========================

STEP 1. includes and defines
============================

```cpp
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
```

`//GLEW`: The OpenGL Extension Wrangler Library (GLEW) is a **cross-platform** open-source C/C++ extension loading library. GLEW provides efficient run-time mechanisms for determining which **OpenGL extensions** are supported on the target platform. OpenGL core and extension functionality is exposed in a single header file. GLEW has been tested on a variety of operating systems, including Windows, Linux, Mac OS X, FreeBSD, Irix, and Solaris.

[What is GLEW and how does it work?](https://gamedev.stackexchange.com/questions/112182/what-is-glew-and-how-does-it-work)

OpenGL is implemented by the graphics driver as a bunch of functions implemented according to the spec, not as a standalone library, therefore cannot be simply linked to your project.

You have to call a function (such as GetProcAddress, but it depends on the platform you're developing for) to get the function pointer at runtime from the driver for each OpenGL function.

GLEW exists because doing just that can be very difficult. It's much easier when you can just include a header and call one function during initialization, which is exactly what GLEW does. Plus, **GLEW is cross-platform**.

`//GLFW`: GLFW is an Open Source, multi-platform library for OpenGL, OpenGL ES and Vulkan development on the desktop. It provides a simple API for **creating windows**, **contexts** and **surfaces**, **receiving input and events**.

STEP 2. Init GLFW
=================

```cpp
int main()
{
    glfwInit();
    ...
```

STEP 3. Set all the required options for GLFW
=============================================

```cpp
int main()
{
  ...

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  ...
```

`GLFW_CONTEXT_VERSION_MAJOR and GLFW_CONTEXT_VERSION_MINOR`: Use OpenGL ver. 3.3

`glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE)`: ...

**CORE_PROFILE**:

`glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE)`: ...

**FORWARD_COMPAT**: All Versions considered with older version compatibilities, removing all deprecated functionality from the context

OpenGL 3.0 deprecated a lot of functionality, but that functionality was still available. OpenGL version 3.1 removed most of the deprecated functionality; at that point, it was no longer available. 3.2 instituted the core/compatibility distinction, where the spec was divided into two separate specifications, one with the removed functionality restored, and one without the removed functionality.

If you create a forward compatibility context, it will remove all deprecated functionality remaining in the core.

STEP 4. Create a GLFW window object that we can use for GLFW's functions
========================================================================

```cpp
int main()
{
  ...

  GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);

  int screenWidth, screenHeight;
  glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

  if (nullptr == window)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();

    return EXIT_FAILURE;
  }

  ...
```

`glfwGetFramebufferSize(window, &screenWidth, &screenHeight)` : Getting the actual width and height of the window screen relative to the density of screen.

On most window screens, you could effectively omit these lines and when you create an OpenGL viewport, just use these two values if you have higher resolution display. Because it's not natively 1920 by 1080, for example, instead if it was a retina version or higher density screen like 3840 by 2160, you would mess up. It would only put the content like in the bottom left and swing on the top right of the screen so it only be a quarter of the screen.

So this is getting the actual width and height of window screen relative to any pixel density changes. You want to ensure maximum compatibility because more and more high resolution screens are coming out.

`nullptr == window`: Check to be the window created successfully

`glfwMakeContextCurrent(window)`: ...

STEP 5. Set Experimental is TRUE
================================

```cpp
int main()
{
  ...

  glewExperimental = GL_TRUE;

  ...
```

`glewExperimental = GL_TRUE`: Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions

STEP 6. Initialize GLEW to setup the OpenGL Function pointers
=============================================================

```cpp
int main()
{
  ...

  if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

  ...
```

STEP 7. Define the viewport dimensions
======================================

```cpp
int main()
{
  ...

  glViewport(0, 0, screenWidth, screenHeight);

  ...
```

STEP 8. Loop
============

```cpp
int main()
{
  ...

  while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set clear color
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

  ...
```

STEP 9. Terminate GLFW, clearing any resources allocated by GLFW.
=================================================================

```cpp
int main()
{
  ...

  glfwTerminate();

  return EXIT_SUCCESS;
}
```

---

Source codes
============

```cpp
#include <iostream>

// GLEW
//#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

// The MAIN function, from here we start the application and run the game loop
int main()
{
    // Init GLFW
    glfwInit();

    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, screenWidth, screenHeight);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();

    return EXIT_SUCCESS;
}

```

---

References
==========

[Modern OpenGL 3.0+ Tutorials, [SETUP]](https://www.youtube.com/watch?annotation_id=annotation_3716210897&feature=iv&index=2&list=PLRtjMdoYXLf6zUMDJVRZYV-6g6n62vet8&src_vid=vGptI11wRxE&v=mGC4T9AG4nc)
