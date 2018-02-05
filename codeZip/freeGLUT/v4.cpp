// A polygon

#include "Angel.h"

//----------------------------------------------------------------------------
// Version 4: This version uses the OpenGL "vertex arrays" feature:
// rather than extracting individual vertices from the array ourselves,
// we tell OpenGL where they are, then tell it to go through the array itself.

vec2 distinctVertices[] = {	vec2(-0.7, -1.0),
				vec2(-1.0, 0.3),
				vec2(0.0, 1.0),
				vec2(1.0, 0.3),
				vec2(0.7, -1.0)};

vec2 vertices[] = {	distinctVertices[0],
			distinctVertices[1],
			distinctVertices[2], // that's one triangle

			distinctVertices[0],
			distinctVertices[2],
			distinctVertices[3], // that's another

			distinctVertices[0],
			distinctVertices[3],
			distinctVertices[4]
		};

void
init( void )
{
    glEnableClientState (GL_VERTEX_ARRAY); // tell OpenGL we want to
					// use the vertex-array feature
    glVertexPointer(2 /* dimensions */,
		    GL_FLOAT /* type of things in array */,
		    0 /* no gaps between data */,
		    vertices);
    // tell OpenGL where to find the data

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
    glDrawArrays(GL_TRIANGLES, // how should OpenGL interpret the vertices?
		 0, // start at the beginning of the array
		 9 // how many elements in the array?
		 );
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
