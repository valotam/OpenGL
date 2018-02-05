// A polygon

#include "Angel.h"

//----------------------------------------------------------------------------
// Version 6: So far all of the arrays have been stored on the "client side"
// in regular RAM.  Things will go faster -- and be legal in OpenGL 3.x -- 
// if we give the data to OpenGL in a "buffer" (which is likely to be stored
// on the GPU).

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
	/* Do I need the following four lines?  NO, at least not on my 2007 Mac
	 * running OpenGL 2.1, which doesn't know about vertex array objects.
	 * However, these lines compile and run harmlessly on that
	 * platform. */
    GLuint vao;			// this will hold the "name" of the vertex array
    glGenVertexArraysAPPLE(1,	// only need one vertex array
    		      &vao);	// where to put its name
    glBindVertexArrayAPPLE(vao); // actually allocate that array

    GLuint buffer;		// this will hold the "name" of the buffer on the GPU
    glGenBuffers (1, &buffer);	// get an unused name
    glBindBuffer (GL_ARRAY_BUFFER, buffer); // say what we're using it for
    glBufferData (GL_ARRAY_BUFFER, 	// which buffer are we filling?
		  sizeof(vertices),	// how many bytes of data to send to GPU?
		  vertices,		// where are they?
		  GL_STATIC_DRAW);	// tell GPU that we're not planning to change
    					// the data at run-time, and the
					// information will go one-way from
					// application to GPU
			    		// (which affects how it stores the data)


    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader21.glsl", "fshader21.glsl" );
    glUseProgram( program );

    // Agree on what GLSL attribute(s) should receive the data we're going to
    // send with glDrawElements()

    GLuint vp = glGetAttribLocation (program, "vPosition");
    glEnableVertexAttribArray (vp);	// use this info in glDrawElements() et al
    glVertexAttribPointer (vp,		// which shared variable are we using?
		    	   2,		// dimensions in vPosition
			   GL_FLOAT,	// element type in vPosition
			   GL_FALSE,	// normalize to [-1,1] range?
			   0,		// distance between elements of vPosition
			   BUFFER_OFFSET(0));
    		// how far to start from beginning of aforementioned buffer


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
	// into the buffer we told it about with glBufferData().
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
