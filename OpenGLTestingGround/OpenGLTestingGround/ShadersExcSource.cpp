#include <iostream>

//includes the static GLEW library
#define GLEW_STATIC
#include <glew.h>

//includes the precompiled GLFW 3 library
#include <glfw3.h>

#include <SHADER_H.h>

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

	GLFWwindow* window = glfwCreateWindow(HEIGHT, WIDTH, "MyfirstOpenGLwindow", NULL, NULL); //creates our window with predefined height and with and title
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

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);//GLFW stores the window's height and width in the variables
	glViewport(0, 0, width, height);//initializes OpenGL viewport in the window's height and windth (could be less)
	//this method ensures we use the maximum amount of resolution available (for example MaxOS retina displays)

	//2.Create and compile the shaders and link them in a shader program

	Shader myShader("VertexShader.vs", "FragmentShader.frag");
	//3.Create buffer objects and a vertex array obejct
	//define verticies for our figure, and create order for the indices of it's triangles
	//GLfloat firstTriangle[] = {
	//	-1.0f,  -0.5f, 0.0f,
	//	-0.5f, 0.5f, 0.0f,
	//	0.0f, -0.5f, 0.0f 
	//};

	GLfloat secondTriangle[] = {
		0.0f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
		1.0f, -0.5f, 0.0,   0.0f, 0.0f, 1.0f
	};
	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3,   // First Triangle
		1, 2, 3    // Second Triangle
	};

	GLuint VAO1, VAO2, VBO1, VBO2;//initialize a vertex buffer object, a vertex array object and a element buffer object
	//glGenVertexArrays(1, &VAO1);//create the vertex array object(s)
	glGenVertexArrays(1, &VAO2);
	//glGenBuffers(1, &VBO1);//create the vertex buffer object, it stores the data for the vertices
	glGenBuffers(1, &VBO2);
	//glGenBuffers(1, &EBO);//create the element buffer object, it stores the data for the indicies

	//4.Encode the necessary vertices and indices buffer information into the VAO
	//glBindVertexArray(VAO1);//the vertex array object stores the buffer configurations so they can be easily accessed
	////everything from this point on will be stored in the VAO

	//	glBindBuffer(GL_ARRAY_BUFFER, VBO1);//binds our VBO to the array buffer
	//	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//binds our EBO to the element array buffer
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);//copies the verticies data to the buffer so it can be quickly accessed by the GPU
	//	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//copies the indices data to the buffer

	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);//defines how OpenGL should read the vertices data
	//	glEnableVertexAttribArray(0);//gives the location of the vertex array

	//glBindVertexArray(0);//unbinds the VAO

	//glBindBuffer(GL_ARRAY_BUFFER, 0);//unbinds the VBO
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);//unbinds the EBO

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	while (!glfwWindowShouldClose(window))//this is the "game loop" it keeps the program running and resolves events
	{
		glfwPollEvents();//resolves events

		//5.Draw the object
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//makes the background
		glClear(GL_COLOR_BUFFER_BIT);

		GLfloat timeValue = glfwGetTime();
		GLfloat offsetValue = (cos(timeValue) / 2) -0.5;
		GLint offsetLocation = glGetUniformLocation(myShader.Program, "x_offset");

		//glUseProgram(shaderProgramOrn);//using the shader program
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.5f, 1.0f);
		//glBindVertexArray(VAO1);//and the buffersetting encoded in the vertex array object
		////glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//we draw the elemnt
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		myShader.Use();
		glUniform1f(offsetLocation, offsetValue);
		glBindVertexArray(VAO2);//and unbind the object
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);//puts forward everything drawn on the back buffer
	}

	glDeleteVertexArrays(1, &VAO2);//when the program stops the memory is freed
	glDeleteBuffers(1, &VBO2);
	//glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)//a keypress triggers an event, if the pressed key is Escape, the window closes, the while condition is met
		glfwSetWindowShouldClose(window, GL_TRUE);
}