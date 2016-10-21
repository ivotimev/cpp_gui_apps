#include <iostream>

//includes the static GLEW library
#define GLEW_STATIC
#include <glew.h>

//includes the precompiled GLFW 3 library
#include <glfw3.h>
//include our shader class
#include <SHADER_H.h>
//include the image processing library
#include <FreeImage.h>

const GLint HEIGHT = 800, WIDTH = 600; //specify the window height and width for easy access

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode); //a prototype of the key callback function, the function is below the main function

int main()
{
	//1.Initialize the required technologies and create a window, set it as the current context
	glfwInit(); //initialize the GLFW library

	//Sends window configurations to the first GLFW window that is opened, errors will be registered during the window initialization
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //set a minimal OpenGL version of 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //makes the window nonresizable

	GLFWwindow* window = glfwCreateWindow(HEIGHT, WIDTH, "Textures", NULL, NULL); //creates our window with predefined height and width and title
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}//checks if window was successfuly created, and displays error if not
	glfwMakeContextCurrent(window);//makes the window the current drawing context

	glfwSetKeyCallback(window, key_callback);//registers the callback function

	glewExperimental = GL_TRUE;//forces us to use modern core OpenGL
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	//initializes GLEW, and displays errors

	GLenum glError = glGetError();
	if (glError)
	{
		std::cout << "There was an error with somethign else" << std::endl;

		switch (glError)
		{
		case GL_INVALID_ENUM:
			std::cout << "Invalid enum." << std::endl;
			break;

		case GL_INVALID_VALUE:
			std::cout << "Invalid value." << std::endl;
			break;

		case GL_INVALID_OPERATION:
			std::cout << "Invalid operation." << std::endl;

		default:
			std::cout << "Unrecognised GLenum." << std::endl;
			break;
		}
	}

	glViewport(0, 0, WIDTH, HEIGHT);//creates the OpenGL viewport in which we will draw

	Shader basicShader("basic.vs", "basic.frag");

	glError = glGetError();
	if (glError)
	{
		std::cout << "There was an error with shader" << std::endl;

		switch (glError)
		{
		case GL_INVALID_ENUM:
			std::cout << "Invalid enum." << std::endl;
			break;

		case GL_INVALID_VALUE:
			std::cout << "Invalid value." << std::endl;
			break;

		case GL_INVALID_OPERATION:
			std::cout << "Invalid operation." << std::endl;

		default:
			std::cout << "Unrecognised GLenum." << std::endl;
			break;
		}
	}

	//triangleVertices
	GLfloat triangle[] = {
		// Positions          // Colors           // Texture Coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Top Left 
	};
	GLfloat elements[] = {
		0, 1, 3,
		1, 2, 3
	};

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glError = glGetError();
	if (glError)
	{
		std::cout << "There was an error with buffers: " << std::endl;

		switch (glError)
		{
		case GL_INVALID_ENUM:
			std::cout << "Invalid enum." << std::endl;
			break;

		case GL_INVALID_VALUE:
			std::cout << "Invalid value." << std::endl;
			break;

		case GL_INVALID_OPERATION:
			std::cout << "Invalid operation." << std::endl;

		default:
			std::cout << "Unrecognised GLenum." << std::endl;
			break;
		}
	}
	FreeImage_Initialise();
	// Load and create a texture 

	const char* filename = "C:\\container.jpg";
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);

	if (format == -1)
	{
		std::cout << "Could not find image: " << filename << " - Aborting." << std::endl;
	}

	FIBITMAP *bitmap = FreeImage_Load(format, filename);
	if (!bitmap)
	{
		std::cout << "Failed to load image ok" << std::endl;
	}

	int imageWidth = FreeImage_GetWidth(bitmap);
	int imageHeight = FreeImage_GetHeight(bitmap);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // Set the texture wrapping parameter
	glTexImage2D(GL_TEXTURE_2D,    // Type of texture
		0,                // Mipmap level (0 being the top level i.e. full size)
		GL_RGB,          // Internal format
		imageWidth,       // Width of the texture
		imageHeight,      // Height of the texture,
		0,                // Border in pixels
		GL_RGB,          // Data format
		GL_UNSIGNED_BYTE, // Type of texture data
		(void*)FreeImage_GetBits(bitmap));

	glError = glGetError();
	if (glError)
	{
		std::cout << "There was an error loading the texture: " << filename << std::endl;

		switch (glError)
		{
		case GL_INVALID_ENUM:
			std::cout << "Invalid enum." << std::endl;
			break;

		case GL_INVALID_VALUE:
			std::cout << "Invalid value." << std::endl;
			break;

		case GL_INVALID_OPERATION:
			std::cout << "Invalid operation." << std::endl;

		default:
			std::cout << "Unrecognised GLenum." << std::endl;
			break;
		}

		std::cout << "See https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml for further details." << std::endl;
	}

	FreeImage_Unload(bitmap);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//makes the background
		glClear(GL_COLOR_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture);

		basicShader.Use();

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)//a keypress triggers an event, if the pressed key is Escape, the window closes, the while condition is met
		glfwSetWindowShouldClose(window, GL_TRUE);
}

