// A polygon

#include "Angel.h"

//----------------------------------------------------------------------------
// Version 5: An annoying feature of version 4 was that we had to repeat vertices
// that appeared in more than one triangle.  In this version, we refer to them
// by their indices instead.

vec2 vertices[] = {vec2(-0.7, -1.0),
		   vec2(-1.0, 0.3),
		   vec2(0.0, 1.0),
		   vec2(1.0, 0.3),
		   vec2(0.7, -1.0)};

GLubyte indices[] = { 0, 1, 2, // that's one triangle
		      0, 2, 3, // that's another
		      0, 3, 4  // that's the third
		    };
// Choose a data type as small as possible to hold all the relevant indices.
// GLubyte will hold a number from 0..255, so this will work as long as we
// have no more than 256 vertices.  If we do, GLushort will allow indices
// from 0..65535; if you have more than that many distinct vertices, you
// can use GLuint. 

void
init( void )
{
    // OpenGL allows you to maintain various standard pieces of information
    // on the client side, but visible to OpenGL: a color array, an edge flag
    // array, an index array, a normal array, a texture array, a vertex array, etc.
    // For this program, we'll use only the vertex array feature.
    // First we tell OpenGL which array(s) we're using...
    glEnableClientState (GL_VERTEX_ARRAY);
    // ... then tell it where that array is and how to interpret it.
    glVertexPointer(2 /* dimensions */,
		    GL_FLOAT /* type of things in array */,
		    0 /* no gaps between data */,
		    vertices);
    // If we had wanted to maintain the color array on the client side, we
    // would have said glEnableClientState (GL_COLOR_ARRAY); and
    // glColorPointer(...), etc.



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
    glDrawElements(GL_TRIANGLES,	// how should OpenGL interpret the vertices?
		   9, 			// how many indices are in the array?
		   GL_UNSIGNED_BYTE, 	// what type are the indices?
		   indices);
	// Note that glDrawElements() knows to treat these indices as indexing
	// into the vertex array we told it about with glVertexPointer().
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
