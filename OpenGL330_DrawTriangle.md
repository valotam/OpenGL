Modern OpenGL Guide, Draw Triangle
==================================

STEP 0. SETUP
=============

STEP 1. Write shader source
===========================

```cpp
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";
```

`"#version 330 core"`: GLSL version 3.3

OpenGL Shading Language (abbreviated: **GLSL**), is a high-level shading language with a syntax based on the C programming language. It was created by the OpenGL ARB (OpenGL Architecture Review Board) to give developers more **direct control of the graphics pipeline** without having to use ARB assembly language or hardware-specific languages.

A fragment shader is the same as pixel shader. One main difference from vertex shader is that **a vertex shader can manipulate the attributes of vertices.** which are the corner points of your polygons.

**The fragment shader on the other hand takes care of how the pixels between the vertices look.** They are interpolated between the defined vertices following specific rules.

For example, if you want your polygon to be completely red, you would define all vertices red. If you want for specific effects like a gradient between the vertices, you have to do that in the fragment shader.

Put another way, the vertex shader is part of the early steps in the graphic pipeline, somewhere between model coordinate transformation and polygon clipping I think. At that point, nothing is really done yet.

However, the fragment/pixel shader is part of the rasterization step, where the image is calculated and the pixels between the vertices are filled in or "coloured".

[![IMAGE ALT TEXT HERE](https://upload.wikimedia.org/wikipedia/commons/5/54/3D-Pipeline.png)](https://en.wikipedia.org/wiki/Graphics_pipeline#Shader)

In vertex shader source

`layout (location = 0) in vec3 position;`: Input vertex data, different for all executions of this shader. As same as variable declaration in C++.

`gl_Position = vec4(position.x, position.y, position.z, 1.0);`: Output position of the vertex, in clip space, represented as homogeneous coordinate.

In fragment shader source

`out vec4 color;`: Ouput data

`color = vec4(1.0f, 0.5f, 0.2f, 1.0f);`: Output color = orange

STEP 2. Build and compile our shader program
============================================

Vertex shader
-------------

```cpp
int main()
{
  ...

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  ...
}
```

Check for compile time errors
-----------------------------

```cpp
int main()
{
  ...

  GLint success;
  GLchar infoLog[512];

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

    ...
}
```

Fragment shader
---------------

```cpp
int main()
{
  ...

  GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
  glShaderSource( fragmentShader, 1, &fragmentShaderSource, NULL );
  glCompileShader( fragmentShader );

  ...
}

```

Check for compile time errors
-----------------------------

```cpp
int main()
{
  ...

  glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &success );

  if ( !success )
  {
      glGetShaderInfoLog( fragmentShader, 512, NULL, infoLog );
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  ...
}
```

Link shaders
------------

```cpp
int main()
{
  ...

  GLuint shaderProgram = glCreateProgram( );
  glAttachShader( shaderProgram, vertexShader );
  glAttachShader( shaderProgram, fragmentShader );
  glLinkProgram( shaderProgram );

  ...
}

```

Check for linking errors
------------------------

```cpp
int main()
{
  ...

  glGetProgramiv( shaderProgram, GL_LINK_STATUS, &success );

  if ( !success )
  {
      glGetProgramInfoLog( shaderProgram, 512, NULL, infoLog );
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }

  glDeleteShader( vertexShader );
  glDeleteShader( fragmentShader );

  ...
}
```

**[Shader Compilation](https://www.khronos.org/opengl/wiki/Shader_Compilation)** is the term used to describe the process by which OpenGL Shading Language scripts are loaded into OpenGL to be used as shaders. OpenGL has three ways to compile shader text into usable OpenGL objects. All of these forms of compilation produce a Program Object.

*Note*: This article contains references to OpenGL 4.x features, such as **tessellation shaders** and **Compute Shaders**. If you are using OpenGL 3.x, you can ignore such references.

A Program Object can contain the executable code for all of the Shader stages, such that all that is needed to render is to bind one program object. Building programs that contain multiple shader stages requires a two-stage compilation process.

This two-stage compilation process mirrors the standard compile/link setup for C and C++ source code. C/C++ text is first fed through a compiler, thus producing an object file. To get the executable code, one or more object files must be linked together.

With this method of program creation, shader text is first fed through a compiler, thus producing a shader object. To get the executable program object, one or more shader objects must be linked together.

STEP 3. Set up vertex data (and buffer(s)) and attribute pointers
=================================================================

```cpp
int main()
{
  ...

  GLfloat vertices[] =
  {
      -0.5f, -0.5f, 0.0f, // Left
      0.5f, -0.5f, 0.0f, // Right
      0.0f,  0.5f, 0.0f  // Top
  };

  GLuint VBO, VAO;
  glGenVertexArrays( 1, &VAO );
  glGenBuffers( 1, &VBO );

  ...
}

```

STEP 4. Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
========================================================================================================

```cpp
int main()
{
  ...

  glBindVertexArray( VAO );

  glBindBuffer( GL_ARRAY_BUFFER, VBO );
  glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid * ) 0 );
  glEnableVertexAttribArray( 0 );

  // Note that this is allowed, the call to glVertexAttribPointer registered VBO
  // as the currently bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer( GL_ARRAY_BUFFER, 0 );

  // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
  glBindVertexArray( 0 );

  ...
}
```

STEP 5. Draw triangle
=====================

```cpp
int main()
{
  ...
  while (!glfwWindowShouldClose(window))
  {
    ...

    glUseProgram( shaderProgram );
    glBindVertexArray( VAO );
    glDrawArrays( GL_TRIANGLES, 0, 3 );
    glBindVertexArray( 0 );

    ...
  }
  ...
}

```

STEP 6. Properly de-allocate all resources once they've outlived their purpose
==============================================================================

```cpp
int main()
{
  ...

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  ...
}
```

---

Source code
===========

```cpp
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

// The MAIN function, from here we start the application and run the game loop
int main()
{
    // Init GLFW
    glfwInit( );

    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );

    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow * window = glfwCreateWindow( WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr );

    int screenWidth, screenHeight;
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );

    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent( window );

    // Set this to true so GLEW knows to use a modern approach to
    // retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport( 0, 0, screenWidth, screenHeight );


    // Build and compile our shader program
    // Vertex shader
    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertexShader, 1, &vertexShaderSource, NULL );
    glCompileShader( vertexShader );

    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];

    glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( vertexShader, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Fragment shader
    GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragmentShader, 1, &fragmentShaderSource, NULL );
    glCompileShader( fragmentShader );

    // Check for compile time errors
    glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &success );

    if ( !success )
    {
        glGetShaderInfoLog( fragmentShader, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link shaders
    GLuint shaderProgram = glCreateProgram( );
    glAttachShader( shaderProgram, vertexShader );
    glAttachShader( shaderProgram, fragmentShader );
    glLinkProgram( shaderProgram );

    // Check for linking errors
    glGetProgramiv( shaderProgram, GL_LINK_STATUS, &success );

    if ( !success )
    {
        glGetProgramInfoLog( shaderProgram, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );


    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] =
    {
        -0.5f, -0.5f, 0.0f, // Left
        0.5f, -0.5f, 0.0f, // Right
        0.0f,  0.5f, 0.0f  // Top
    };

    GLuint VBO, VAO;
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray( VAO );

    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    glEnableVertexAttribArray( 0 );

    // Note that this is allowed, the call to glVertexAttribPointer registered VBO
    // as the currently bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
    glBindVertexArray( 0 );

    // Game loop
    while ( !glfwWindowShouldClose( window ) )
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and
        // call corresponding response functions
        glfwPollEvents( );

        // Render
        // Clear the colorbuffer
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );

        // Draw our first triangle
        glUseProgram( shaderProgram );
        glBindVertexArray( VAO );
        glDrawArrays( GL_TRIANGLES, 0, 3 );
        glBindVertexArray( 0 );

        // Swap the screen buffers
        glfwSwapBuffers( window );
    }

    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate( );

    return EXIT_SUCCESS;
}
```

---

References
==========

[Modern OpenGL 3.0+ Tutorials, [Draw Triangle]](https://www.youtube.com/watch?v=EIpxcNl2WJU&index=13&list=PLRtjMdoYXLf6zUMDJVRZYV-6g6n62vet8)
