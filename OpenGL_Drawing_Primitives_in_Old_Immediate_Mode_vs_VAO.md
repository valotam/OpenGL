[OpenGL Drawing Primitives in Immediate Mode vs in VAO](http://www.falloutsoftware.com/tutorials/gl/gl3.htm)
============================================================================================================

Method Development
------------------

from Past to Present

```wavedrom
{signal: [
  ['Development',
  {  name: ' ',  wave: '3......xxxxxxxx', data: ['glBegin and glEnd(Immediate Mode)']},
  {  name: ' ', wave: 'xxxxxx4...xxxxx', data: ['Display Lists']},
  {name: ' ', wave: 'xxxx5..........', data: ['VAO(Vertex Array Objects), VBO(Vertex Buffer Objects)']},
  ],
]}
```

The only thing you will not be able to do in **Immediate Mode** is to use **OpenGL Shading Language** (**GLSL**). Note, however, although different methods of rendering, it is possible to mix drawing in Immediate Mode and with GLSL by switching between the two within the same OpenGL program. Having said this, GLSL is a much more powerful rendering technique. And anything you can do in Immediate Mode can be done in GLSL with salt and pepper added.

However, drawing primitives in OpenGL via VBO has become a requirement as over the years, the OpenGL pipeline changed from outdated methods to new ones. And knowing glBegin and glEnd commands is not enough, as they are quickly becoming outdated and obsolete.

After that, let's take a look at different ways of drawing primitives in OpenGL. One, called **Immediate Mode** using glBegin and glEnd, has become outdated. But it still works, and it's Ok for practicing. But probably not a good idea to use in modern software.

> Because, for example, to draw two lines in one glBegin-glEnd sequence you need to specify FOUR vertices. Likewise, to draw 2 triangles in one shot, you would call glBegin(GL_TRIANGLES) and specify SIX vertices afterwards. glBegin and glEnd can be expensive calls in tight loops.
>
> If you have an object containing a polygon and, say a line (to indicate its normal?), it would be better to have two loops: first one for drawing all the polygons in the object and second one to draw all the lines in the objects, rather than calling glBegin and glEnd 4 times in one loop each iteration. This is an oversimplified example, don't take my word for it.

```cpp
glBegin(GL_TRIANGLES); // Draw a triangle:
  glVertex3f(-1.0f, -0.5f, -4.0f); // Lower left vertex
  glVertex3f( 1.0f, -0.5f, -4.0f); // Lower right vertex
  glVertex3f( 0.0f,  0.5f, -4.0f); // Upper vertex
glEnd();
```

The other is using **Display Lists**. Note, OpenGL ES(Embeded System) does not support Display Lists. But they still work on desktops. First, a list is created, and the 3D data representing vertices is placed into it. Later on, in the rendering loop, the vertex list is called using glCallList command, to render the polygons.

```cpp

GLuint list = glGenLists( 1 ); // Generate 1 display list

glNewList( list, GL_COMPILE );
  glBegin(GL_TRIANGLES); // Enter your vertices here
    glVertex3f(-1.0f, -0.5f, -4.0f); // Lower left vertex
    glVertex3f( 1.0f, -0.5f, -4.0f); // Lower right vertex
    glVertex3f( 0.0f,  0.5f, -4.0f); // Upper vertex
  glEnd();
glEndList();

// Then later, elsewhere in your code,
// Call the display list to render it
glCallList( list );
```

VAO and VBO
-----------

Inevitably on your journey to understand modern OpenGL rendering techniques, you will stumble across two principles. They are **VAO** or **Vertex Array Object** and **VBO** or **Vertex Buffer Object**. Let's get familiar with them.

VBO and VAO can cause a little confusion at first. Because they seem like they are used together or for the same or similar purpose. However, they have little to do with one another. First question is what is the difference between the two? This is probably a wrong question to ask, as they perform two unique actions.

**VBO** stores actual vertex data. The most important thing about a VBO is not that it stores data, though it is its primary function, but where it is stored. A VBO object resides on GPU, the graphics processing unit. This means it is very fast, it is stored in memory on the graphics card itself. How cool is that? Storing data on the computer processor or RAM is slow mostly because it needs to be transferred to the GPU, and this transfer can be costly.

**VAO** represents properties, rather than actual data. But these properties do describe the objects actually stored in the VBO. VAO can be thought of as an **advanced memory pointer** to objects. Similar to C-language pointers, they do a whole lot more tracking than just the address. They are very sophisticated.

VAOs are a lot like helpers, rather than actual data storage. That's what they're for. They also keep track of properties to be used in current rendering process. Finally, they describe properties of objects, rather than the raw data of those objects that is by all means already stored in a VBO.

VAOs are not directly related to VBOs, although it may seem that way at first. VAOs simply save time to enable a certain application state needed to be set. Without VAO, you would have to call a bunch of gl* commands to do the same thing.

VBO is used to render complex 3D geometry represented as a list stored in GPU memory. It's tough to abandon glBegin and glEnd methods in favor of VBO. But we are gaining so much more in terms of performance. And if we want to achieve a greater amount of realism we will use VBOs together with GLSL shaders. But that's a subject of another tutorial.

In the following examples we will discover how VBOs are used in an OpenGL program to actually draw a display list consisting of a set of arbitrary vertices.

```cpp
GLfloat vertices[] = {
    0,0,0,   // v1
    0,0,0,   // v2
    0,0,0,   // v3
    0,0,0,   // v4
    0,0,0,   // v5
    0,0,0    // v6
}

int size = 18 * sizeof(GLfloat);
int m_vboId = CreateVBO(&vertices[0], size);

int main(int argc, char const *argv[]) {
  ...
  glPushMatrix();

    // Bind VBO for drawing array data
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vboId);

    // Activate array-based data
    glEnableClientState(GL_VERTEX_ARRAY);

    // Stride of 3, floats
    glVertexPointer(3, GL_FLOAT, 0, 0);

    // Draw triangles
    glDrawArrays(GL_TRIANGLES, 0, 18);

    // Switch off vertex array data
    glDisableClientState(GL_VERTEX_ARRAY);

    // Bind with 0 to switch back to default pointer operation
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, NO_VBO_ID);

    glPopMatrix();  return 0;
    ...
}
```

[Tutorials](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle)
=============================================================================================

References
==========

Drawing OpenGL Primitives VBO, VAO, glBegin and glVertex (old Immediate Mode) <www.falloutsoftware.com/tutorials/gl/gl3.htm>

Opengl-tutorial <www.opengl-tutorial.org>
