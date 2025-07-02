#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//vertex shader source code
const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

//fragment shader source code
const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

int main() 
{
	glfwInit();  //initialize glfw
	//specify version of opengl
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //using core profile

	GLfloat vertices[] =
	{
		-0.5f, -0.5f, 0.0f,  //left
		0.5f, -0.5f, 0.0f,  //right
		0.0f, 0.5f, 0.0f  //top
	};

	//new window object
	GLFWwindow* window = glfwCreateWindow(800, 800, "Openglcpp", NULL, NULL);  //800x800
	if (window == NULL)  //error check
	{  
		std::cout << "Failed to create GLFW window" << '\n';
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);  //introduce window to current context

	if (!gladLoadGL()) {  // load glad to configure opengl
	    std::cout << "Failed to initialize GLAD" << std::endl;
	    return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //called framebuffer_size_callback

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);  //vertex shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);  //attach vertex shader source to the object
	glCompileShader(vertexShader);  //compile into machine code

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);  //fragment shader object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);  //attach frag shader to object
	glCompileShader(fragmentShader);  //compile

	GLuint shaderProgram = glCreateProgram();  //create shader prograsm
	
	//attach vertex and frag shaders to shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//delete vertex and frag shader objects since it is now attached to the program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	GLuint VAO, VBO; //Vertex Array Object and Vertex Buffer Object

	//Generate the vao and vbo with one object each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);  //bind vao

	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //bind vbo as an array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  //introduce vertices[] into vbo

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  //configure vertex attribute
	glEnableVertexAttribArray(0);  //enable vertex attribute

	//bind vao and vbo to 0
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))  //main while loop
	{  
		processInput(window);  //input

		// glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  -- why is the framebuffercallback outside the loop if it is always being updated after a resize?
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);  //set color
		glClear(GL_COLOR_BUFFER_BIT);  //clean back buffer and assigned the new color to it

		glUseProgram(shaderProgram);  //use shader program
		glBindVertexArray(VAO);  //bind vao
		glDrawArrays(GL_TRIANGLES, 0, 3);  //draw triangle

		glfwSwapBuffers(window); //swap back buffer with front buffer
		glfwPollEvents(); //process all pulled events
	}

	//delete all objects created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

//adjusts frame buffer size to glfw window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) 
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}