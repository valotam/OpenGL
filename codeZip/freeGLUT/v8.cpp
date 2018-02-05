// A polygon

#include "Angel.h"

//----------------------------------------------------------------------------
// Version 8: That smear of color is sorta cool, but what if we actually
// want different-colored polygons?  The answer used to be 
// 	glShadeModel (GL_FLAT);
// which told OpenGL to pick a single color for each polygon, and shade
// the whole polygon uniformly rather than interpolating colors.
// The glShadeModel function is now deprecated, so we need to find
// another way to do it.  At present we have an array of 5 vertex
// positions and a corresponding array of 5 colors, so each vertex
// has a color of its own, rather than being on a sharp border between
// two color regions.  To avoid this, we could go back to the version-4
// approach, with the duplicated positions...

vec2 vertices[] = {vec2(-0.7, -1.0),
		   vec2(-1.0, 0.3),
		   vec2(0.0, 1.0),

		   vec2(-0.7, -1.0),
		   vec2(0.0, 1.0),
		   vec2(1.0, 0.3),

		   vec2(-0.7, -1.0),
		   vec2(1.0, 0.3),
		   vec2(0.7, -1.0)};

vec4 red = vec4(1.0, 0.0, 0.0, 1.0);
vec4 green = vec4(0.0, 1.0, 0.0, 1.0);
vec4 blue = vec4(0.0, 0.0, 1.0, 1.0);

vec4 colors[] = {red, red, red,
		 green, green, green,
		 blue, blue, blue};

void init( void )
{
    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vpassthru.glsl", "fpassthru.glsl" );
    glUseProgram( program );

    GLuint vao;			// this will hold the "name" of the vertex array
    glGenVertexArraysAPPLE(1,	// only need one vertex array
    		      &vao);	// where to put its name
    glBindVertexArrayAPPLE(vao); // actually allocate that array

    GLuint buffers[2];		// this will hold the "names" of the buffers on the GPU
    glGenBuffers (2, buffers);	// get two unused buffer names
    GLuint position_buffer = buffers[0];	// just a mnemonic
    GLuint color_buffer = buffers[1];
    glBindBuffer (GL_ARRAY_BUFFER, position_buffer); // say what we're using it for
    glBufferData (GL_ARRAY_BUFFER, 	// which buffer are we filling?
		  sizeof(vertices),	// how many bytes of data to send to GPU?
		  vertices,		// where are they?
		  GL_STATIC_DRAW);	// tell GPU that we're not planning to change
    					// the data at run-time, and the
					// information will go one-way from
					// application to GPU
			    		// (which affects how it stores the data)


    // Agree on what GLSL attribute(s) should receive the data we're going to
    // send with glDrawElements()

    GLuint vp = glGetAttribLocation (program, "vPosition");
    glEnableVertexAttribArray (vp);	// use this info in glDrawElements() et al
    // Next, tell the vertex array object that values of the "vPosition"
    // variable should come from the current ARRAY_BUFFER, and how this
    // array is organized.  Note that "current" means "whichever buffer
    // was most recently bound to GL_ARRAY_BUFFER as of the time
    // glVertexAttribPointer is called."
    glVertexAttribPointer (vp,		// which shared variable are we using?
		    	   2,		// dimensions in vPosition
			   GL_FLOAT,	// element type in vPosition
			   GL_FALSE,	// don't normalize to [-1,1] range
			   0,		// distance between elements of vPosition
			   BUFFER_OFFSET(0));
    		// how far to start from beginning of the current
    		// ARRAY_BUFFER, which is position_buffer.

    
    glBindBuffer (GL_ARRAY_BUFFER, color_buffer); // let's do it again with the color buffer
    glBufferData (GL_ARRAY_BUFFER,
		  sizeof(colors),
		  colors,	
		  GL_STATIC_DRAW);
    GLuint vc = glGetAttribLocation (program, "vColor");
    glEnableVertexAttribArray (vc); // pay attention to color info too
    glVertexAttribPointer (vc,
			   4,		// dimensions
			   GL_FLOAT,	// element type
			   GL_FALSE,	// don't normalize
			   0,		// stride
			   BUFFER_OFFSET(0));

    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window
    glDrawArrays(GL_TRIANGLES, // how should OpenGL interpret the vertices?
		 0, // start at the beginning of the arrays
		 9 // how many elements in each array?
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
