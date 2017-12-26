#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"
#include "Camera.h"


// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void Draw(Shader& _lightingShader, Shader& _lampShader);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;
const GLuint FOILMAX = 10;
const GLfloat HUBRADIUS = 3.0f;

// Camera
Camera  camera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(-20.0f, 20.0f, 2.0f);

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// VAO, VBO, EBO
GLuint foilVAO, foilVBO, foilEBO;
GLuint hubVAO, hubVBO, hubEBO;
GLuint lampVAO, lampVBO;

// The MAIN function, from here we start the application and run the game loop
int main( )
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
    GLFWwindow* window = glfwCreateWindow( WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    // Build and compile shader programs
    Shader lightingShader("core.vertexshader", "core.fragmentshader");
    Shader lampShader( "lamp.vertexshader", "lamp.fragmentshader" );
    
	// Set up vertex data (and buffer(s)) and attribute pointers
	lightingShader.LoadOutFile("foil_spline.out");
	lightingShader.MakeFoil(FOILMAX);
	lightingShader.MakeHub(HUBRADIUS);

    // First, set the foil's VAO (and VBO)
    glGenVertexArrays( 1, &foilVAO );
    glGenBuffers( 1, &foilVBO);
    glBindBuffer( GL_ARRAY_BUFFER, foilVBO);
	glBufferData( GL_ARRAY_BUFFER, sizeof(VertexAttribute) * lightingShader.vFoilVertex.size(), &lightingShader.vFoilVertex.front(), GL_STATIC_DRAW);
    glBindVertexArray( foilVAO );
    // Position attribute
	GLuint vp = glGetAttribLocation(lightingShader.Program, "position");
	glEnableVertexAttribArray(vp);
	glVertexAttribPointer(vp, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (GLvoid*)(0));    
    // Normal attribute
	GLuint vn = glGetAttribLocation(lightingShader.Program, "normal");
	glEnableVertexAttribArray(vn);
	glVertexAttribPointer(vn, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (GLvoid*) (3*sizeof(GLfloat)));
	glGenBuffers(1, &foilEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, foilEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * lightingShader.vFoilIndices.size(), &lightingShader.vFoilIndices.front(), GL_STATIC_DRAW);
    glBindVertexArray( 0 );

	// Second, set the hub's VAO (and VBO)
	glGenVertexArrays(1, &hubVAO);
	glGenBuffers(1, &hubVBO);
	glBindBuffer(GL_ARRAY_BUFFER, hubVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexAttribute) * lightingShader.vHubVertex.size(), &lightingShader.vHubVertex.front(), GL_STATIC_DRAW);
	glBindVertexArray(hubVAO);
	// Position attribute
	glEnableVertexAttribArray(vp);
	glVertexAttribPointer(vp, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (GLvoid*)(0));
	// Normal attribute
	glEnableVertexAttribArray(vn);
	glVertexAttribPointer(vn, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (GLvoid*)(3 * sizeof(GLfloat)));
	glGenBuffers(1, &hubEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hubEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * lightingShader.vHubIndices.size(), &lightingShader.vHubIndices.front(), GL_STATIC_DRAW);
	glBindVertexArray(0);

    // Then, set the light's VAO (VBO stays at location fixed. Also the vertices are the same for the 3D cube object)
    glGenVertexArrays( 1, &lampVAO);
    glBindVertexArray(lampVAO);
    // Only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glGenBuffers(1, &lampVBO);
    glBindBuffer( GL_ARRAY_BUFFER, lampVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lampShader.vertices), lampShader.vertices, GL_STATIC_DRAW);
    // Set the vertex attributes (only position data for the lamp))
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), ( GLvoid * )0 ); // Note that we skip over the normal vectors
    glEnableVertexAttribArray( 0 );
    glBindVertexArray( 0 );
        
    // Game loop
    while ( !glfwWindowShouldClose( window ) )
    {
        // Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime( );
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents( );
        DoMovement( );
        
        // Clear the colorbuffer
        glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
		// Draw
		Draw(lightingShader, lampShader);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
    
    glDeleteVertexArrays( 1, &foilVAO );
	glDeleteBuffers(1, &foilVBO);
	glDeleteBuffers(1, &foilEBO);
	glDeleteVertexArrays(1, &hubVAO);
	glDeleteBuffers(1, &hubVBO);
	glDeleteBuffers(1, &hubEBO);
	glDeleteVertexArrays( 1, &lampVAO);
	glDeleteBuffers( 1, &lampVBO );
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate( );
    return EXIT_SUCCESS;
}

void Draw(Shader& _lightingShader, Shader& _lampShader)
{
	// Use cooresponding shader when setting uniforms/drawing objects
	glUseProgram(_lightingShader.Program);
	GLint lightPosLoc = glGetUniformLocation(_lightingShader.Program, "light.position");
	GLint viewPosLoc = glGetUniformLocation(_lightingShader.Program, "viewPos");
	glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

	// Set lights properties
	glm::vec3 lightColor;
	lightColor.r = 0.3f * sin(glfwGetTime() * 1.0f) + 0.7f;
	lightColor.g = 0.1f * sin(glfwGetTime() * 0.3f) + 0.9f;
	lightColor.b = 0.1f * sin(glfwGetTime() * 0.6f) + 0.9f;

	glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // Decrease the influence
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // Low influence
	glUniform3f(glGetUniformLocation(_lightingShader.Program, "light.ambient"), ambientColor.r, ambientColor.g, ambientColor.b);
	glUniform3f(glGetUniformLocation(_lightingShader.Program, "light.diffuse"), diffuseColor.r, diffuseColor.g, diffuseColor.b);
	glUniform3f(glGetUniformLocation(_lightingShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);

	// Set material properties
	glUniform3f(glGetUniformLocation(_lightingShader.Program, "material.ambient"), 1.0f, 0.5f, 0.31f);
	glUniform3f(glGetUniformLocation(_lightingShader.Program, "material.diffuse"), 1.0f, 0.5f, 0.31f);
	glUniform3f(glGetUniformLocation(_lightingShader.Program, "material.specular"), 0.5f, 0.5f, 0.5f); // Specular doesn't have full effect on this object's material
	glUniform1f(glGetUniformLocation(_lightingShader.Program, "material.shininess"), 32.0f);

	// Create camera and whole models transformations
	glm::mat4 view;
	view = camera.GetViewMatrix();
	view = glm::scale(view, glm::vec3(0.1f, 0.1f, 0.1f));
	glm::mat4 model_pure;
	model_pure = glm::translate(model_pure, glm::vec3(0.0f, 0.0f, -25.0f));
	model_pure = glm::rotate(model_pure, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 500.0f);

	// Get the uniform locations
	GLint modelLoc = glGetUniformLocation(_lightingShader.Program, "model");
	GLint viewLoc = glGetUniformLocation(_lightingShader.Program, "view");
	GLint projLoc = glGetUniformLocation(_lightingShader.Program, "projection");

	// Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Draw foils (using foil's vertex attributes)
	glBindVertexArray(foilVAO);
	// foil #1.
	glm::mat4 model;
	model = glm::translate(model_pure, glm::vec3(HUBRADIUS, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, _lightingShader.vFoilIndices.size(), GL_UNSIGNED_INT, 0);
	// foil #2.
	model = glm::rotate(model_pure, 120 * 3.14f / 180, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(HUBRADIUS, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, _lightingShader.vFoilIndices.size(), GL_UNSIGNED_INT, 0);
	// foil #3.
	model = glm::rotate(model_pure, 240 * 3.14f / 180, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(HUBRADIUS, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, _lightingShader.vFoilIndices.size(), GL_UNSIGNED_INT, 0);
	// foil #1's boundary line
	glUniform3f(glGetUniformLocation(_lightingShader.Program, "material.ambient"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(_lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(_lightingShader.Program, "material.specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(_lightingShader.Program, "material.shininess"), 32.0f);
	model = glm::translate(model_pure, glm::vec3(HUBRADIUS, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_POINTS, 0, _lightingShader.vFoilVertex.size());
	// foil #2's boundary line
	model = glm::rotate(model_pure, 120 * 3.14f / 180, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(HUBRADIUS, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_POINTS, 0, _lightingShader.vFoilVertex.size());
	// foil #3's boundary line
	model = glm::rotate(model_pure, 240 * 3.14f / 180, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(HUBRADIUS, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_POINTS, 0, _lightingShader.vFoilVertex.size());
	glUniform3f(glGetUniformLocation(_lightingShader.Program, "material.ambient"), 1.0f, 0.5f, 0.31f);
	glUniform3f(glGetUniformLocation(_lightingShader.Program, "material.diffuse"), 1.0f, 0.5f, 0.31f);
	glUniform3f(glGetUniformLocation(_lightingShader.Program, "material.specular"), 0.5f, 0.5f, 0.5f);
	glUniform1f(glGetUniformLocation(_lightingShader.Program, "material.shininess"), 32.0f);
	glBindVertexArray(0);

	// Draw hub (using hub's vertex attributes)
	glBindVertexArray(hubVAO);
	glUniform3f(glGetUniformLocation(_lightingShader.Program, "material.ambient"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(_lightingShader.Program, "material.diffuse"), 0.5f, 0.5f, 0.5f);
	glUniform1f(glGetUniformLocation(_lightingShader.Program, "material.shininess"), 25.0f);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_pure));
	glDrawElements(GL_TRIANGLES, _lightingShader.vHubIndices.size(), GL_UNSIGNED_INT, 0);
	glUniform3f(glGetUniformLocation(_lightingShader.Program, "material.ambient"), 1.0f, 0.5f, 0.31f);
	glUniform3f(glGetUniformLocation(_lightingShader.Program, "material.diffuse"), 1.0f, 0.5f, 0.31f);
	glUniform3f(glGetUniformLocation(_lightingShader.Program, "material.specular"), 0.5f, 0.5f, 0.5f);
	glUniform1f(glGetUniformLocation(_lightingShader.Program, "material.shininess"), 32.0f);
	glBindVertexArray(0);

	// Also set the lamp object, again binding the appropriate shader
	glUseProgram(_lampShader.Program);
	// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
	modelLoc = glGetUniformLocation(_lampShader.Program, "model");
	viewLoc = glGetUniformLocation(_lampShader.Program, "view");
	projLoc = glGetUniformLocation(_lampShader.Program, "projection");
	// Set matrices
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	model = glm::mat4();
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// Draw the lamp object (using lamp's vertex attributes)
	glBindVertexArray(lampVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

// Callback whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}
