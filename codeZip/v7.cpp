// A polygon

#include "Angel.h"

//----------------------------------------------------------------------------
// Version 7: So far we've been coloring everything red, using a
// constant-valued fragment shader.  That's pretty silly: more often,
// one wants to specify colors, normals, etc. somehow in the application.
// So for this example, we use different shaders: "vpassthru.glsl",
// which takes in "vPosition" and "vColor" input attributes and passes
// them unaltered to the "gl_Position" and "color" varying attributes.
// Note that the "color" attribute received by the fragment shader is
// not quite the same value produced by the vertex shader; instead, the
// rasterizer automatically interpolates colors across each triangle
// and sends the interpolated colors to the fragment shader.
// We'll use two separate buffers: one of 2d vectors for the "vPosition"
// variable, and one of 4d vectors for the "vColor" variable.  A single
// vertex array object refers to both of them, through two separate
// calls to glVertexAttribPointer.
// A vertex array can refer to several different buffers, any or all of
// which may actually be used when drawing.  glEnableVertexAttribArray
// tells OpenGL which of them to pay attention to.

vec2 vertices[] = {vec2(-0.7, -1.0),
		   vec2(-1.0, 0.3),
		   vec2(0.0, 1.0),
		   vec2(1.0, 0.3),
		   vec2(0.7, -1.0)};

vec4 colors[] = {vec4(0.0,0.0,0.0,1.0), // black
		 vec4(1.0,0.0,0.0,1.0), // red
		 vec4(1.0,1.0,0.0,1.0), // yellow
		 vec4(0.0,1.0,1.0,1.0), // cyan
		 vec4(0.0,0.0,1.0,1.0)}; // blue

GLubyte indices[] = { 0, 1, 2, // that's one triangle
		      0, 2, 3, // that's another
		      0, 3, 4  // that's the third
		    };

void
init( void )
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
