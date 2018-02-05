// A polygon

#include "Angel.h"

//----------------------------------------------------------------------------
// Version 2: in "modern" OpenGL, the GL_POLYGON primitive is deprecated;
// they really prefer that you build everything from triangles.  So this
// version breaks the pentagon down into three triangles.

void
init( void )
{
    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader21.glsl", "fshader21.glsl" );
    glUseProgram( program );
    // I don't think this is necessary, since InitShader has already called
    // glUseProgram(program)


    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window
    glBegin(GL_TRIANGLES);

    glVertex2f(-0.7, -1.0);
    glVertex2f(-1.0, 0.3);
    glVertex2f(0.0, 1.0); // that's one triangle

    glVertex2f(-0.7, -1.0);
    glVertex2f(0.0, 1.0);
    glVertex2f(1.0, 0.3); // that's another

    glVertex2f(-0.7, -1.0);
    glVertex2f(1.0, 0.3);
    glVertex2f(0.7,-1.0); // and a third
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
