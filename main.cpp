#pragma region Library Imports

#include <cstdlib> // Import the C standard libraries.
#include <iostream> // Import the IO stream libraries.

// Define and import GLEW, the extension management system.
#define GLEW_STATIC // Use GLEW statically.
#include <GL/glew.h> // Import the GLEW library.

// Import GLFW, the modern window management system.
#include <GLFW/glfw3.h> // Import the GLFW library.

using namespace std; // Use the standard namespace, so I don't have to reference a std::string every time.

#pragma endregion

#pragma region Global Variables and Constants

// Window dimensions
GLuint WIDTH = 640, HEIGHT = 480;

// Shaders
const GLchar* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";
const GLchar* fragmentShaderSource =
"#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"}\n\0";

#pragma endregion

#pragma region Callbacks

// Key Callback: Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

// Window Callback: Is called whenever the window changes size.
void window_size_callback(GLFWwindow* window, int width, int height) {
	WIDTH = width;
	HEIGHT = height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
}

#pragma endregion

int main()
{
	#pragma region Initialise GLFW and GLEW

	// Initialise GLFW, the windowing system.
	glfwInit();

	// Set all the required options for GLFW.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Set the major version (3).
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Set the minor version (3).
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Set the OpenGL profile (core profile).
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); // Set the resizable option (true).

	// Create a GLFWwindow object that we can use for GLFW's functions.
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Alphascape", nullptr, nullptr); // Create the window.
	glfwMakeContextCurrent(window); // Make this window the current context.

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback); // Set the key_callback.
	glfwSetWindowSizeCallback(window, window_size_callback); // Set the window_size_callback.

	// Tell GLEW to use a modern approach to retrieving function pointers and extensions.
	glewExperimental = GL_TRUE;

	// Initialize GLEW, to set up the OpenGL function pointers.
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	#pragma endregion

	#pragma region Compile Shaders

	// Build and compile the shader program.
	// The vertex shader:
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // Create the shader.
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Pass the shader source.
	glCompileShader(vertexShader); // Compile the shader.
	// Check for errors at compile time from OpenGL:
	GLint success; // Declare the success variable.
	GLchar infoLog[512]; // Declare the information log.
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // Get the success of the shader compilation.
	if (!success) // If the shader compilation was not a success:
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); // Get the information log.
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl; // Print the information log.
	}

	// The fragment shader:
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Create the shader.
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // Pass the shader source.
	glCompileShader(fragmentShader); // Compile the shader.
	// Check for errors at compile time from OpenGL:
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); // Get the success of the shader compilation.
	if (!success) // If the shader compilation was not a success:
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog); // Get the information log.
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl; // Print the information log.
	}

	// Link the shaders.
	GLuint shaderProgram = glCreateProgram(); // Create the shader program.
	glAttachShader(shaderProgram, vertexShader); // Attach the vertex shader.
	glAttachShader(shaderProgram, fragmentShader); // Attach the fragment shader.
	glLinkProgram(shaderProgram); // Link the shader program to the OpenGL context.

	// Check for errors at compile time from OpenGL:
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); // Get the success of the shader linking.
	if (!success) { // If the shader linking was not a success:
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog); // Get the information log.
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl; // Print the information log.
	}

	// Delete the shaders to avoid a memory leak.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	#pragma endregion

	#pragma region VBO, VAO, Attribute Pointers

	// Set up vertex data, buffers, and attribute pointers.
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f, // The left vertex;
		0.5f, -0.5f, 0.0f, // The right vertex;
		0.0f,  0.5f, 0.0f  // The top vertex.
	};
	GLuint VBO, VAO; // Declare the vertex buffer object and vertex array object.
	glGenVertexArrays(1, &VAO); // Generate 1 vertex array object.
	glGenBuffers(1, &VBO); // Generate 1 vertex buffer object.

	// Bind the VAO, then bind and set the vertex buffer and attribute pointer.
	glBindVertexArray(VAO); // Bind the vertex array object.

	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the vertex array buffer and vertex buffer object.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Load the vertices as static vertices.

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); // Tell OpenGL how to interpret the vertices.
	glEnableVertexAttribArray(0); // Enable the vertex attribute array, size 0.

	// Call the attribute pointer with the previously registered VBO, so the buffer object can be unbound later.
	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	glBindVertexArray(0); // Unbind the vertex array object (response to bug #2, project Deltashot).

	#pragma endregion

	#pragma region Main Loop
	while (!glfwWindowShouldClose(window)) // While the game window should remain open
	{
		glfwPollEvents(); // Check if any events have been called.

		// Render everything:

		// Set the clear colour, and clear the buffers.
		glClearColor(0.529f, 0.808f, 0.980f, 1.0f); // Set the clear colour.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear the buffers.

		// Draw a triangle.
		glUseProgram(shaderProgram); // Use the shader program.
		glBindVertexArray(VAO); // Bind the vertex array object.
		glDrawArrays(GL_TRIANGLES, 0, 3); // Draw three vertices, starting from vertex 0.
		glBindVertexArray(0); // Bind to the (only) vertex array.

		glfwSwapBuffers(window); // Swap the buffers.
	}
	#pragma endregion

	#pragma region Clean Up
	// Properly de-allocate all resources.
	glDeleteVertexArrays(1, &VAO); // Delete the vertex array object.
	glDeleteBuffers(1, &VBO); // Delete the vertex buffer object.

	// Terminate the game window. Return success!
	glfwTerminate(); // Terminate the GLFW context.
	return 0; // Return success.
	#pragma endregion
}