// A polygon

#include "Angel.h"

//----------------------------------------------------------------------------
// Version 3: This version behaves exactly the same as version 2, but
// the vertex coordinates are stored in an array rather than being
// hard-coded into the display() function. This has several advantages:
// 1) we can easily refer to the same vertex as part of several
// different triangles;
// 2) we can easily use the vertices in a different order;
// 3) it's a nice segue into sharing the data with OpenGL,
// and eventually storing the data on the GPU.
// Note that glVertex2fv takes in a 2-element array or vector of floats,
// whereas glVertex2f took in two separate floats.

vec2 vertices[] = {vec2(-0.7, -1.0),
		   vec2(-1.0, 0.3),
		   vec2(0.0, 1.0),
		   vec2(1.0, 0.3),
		   vec2(0.7, -1.0)};

void
init( void )
{
    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader21.glsl", "fshader21.glsl" );
    glUseProgram( program );


    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window
    glBegin(GL_TRIANGLES);

    glVertex2fv(vertices[0]);
    glVertex2fv(vertices[1]);
    glVertex2fv(vertices[2]); // that's one triangle

    glVertex2fv(vertices[0]);
    glVertex2fv(vertices[2]);
    glVertex2fv(vertices[3]); // that's another

    glVertex2fv(vertices[0]);
    glVertex2fv(vertices[3]);
    glVertex2fv(vertices[4]); // that's the third

    glEnd();
    glFlush();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 033:                   // ESC key
        exit( EXIT_SUCCESS );
        break;
    }
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA );
    glutInitWindowSize( 512, 512 );

    glutCreateWindow( "Just a pentagon" );

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );

    glutMainLoop();
    return 0;
}
