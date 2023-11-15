#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"ShaderClass.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(
	GLFWwindow* window,
	glm::mat4& mvp,
	glm::mat4& strafe_up,
	glm::mat4& strafe_down,
	glm::mat4& strafe_right,
	glm::mat4& strafe_left,
	glm::mat4& rotationMatrix1,
	glm::mat4& rotationMatrix2);


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const float SENSIVITY = 0.01;
const float NORM_COLOR = 255.0f;
const float NORM_VERT = 25.0f;


GLfloat vertices[] = {
	-22.0f / NORM_VERT, 3.0f / NORM_VERT,   //0 -21 4
	-22.0f / NORM_VERT, 13.0f / NORM_VERT,   //1 -21 14   
	-17.0f / NORM_VERT, 8.0f / NORM_VERT,    //2 -16 9  
	-15.0f / NORM_VERT, 10.0f / NORM_VERT,   //3 -14 11 
	-15.0f / NORM_VERT, 3.0f / NORM_VERT,   //4 -14 4
	-22.0f / NORM_VERT, -4.0f / NORM_VERT,  //5 -21 -3
	-15.0f / NORM_VERT, -4.0f / NORM_VERT,   //6 -14 -3
	-15.0f / NORM_VERT, -15.0f / NORM_VERT,  //7 -14 -14
	-15.0f / NORM_VERT, 0.0f / NORM_VERT,    //8 -14 0.5
	0.0f / NORM_VERT, 0.0f / NORM_VERT,     //9  0.5 0.5
	15.0f / NORM_VERT, 0.0f / NORM_VERT,    //10 15 0.5 
	15.0f / NORM_VERT, -15.0f / NORM_VERT,   //11 15 -14
	-7.5f / NORM_VERT, -7.5f / NORM_VERT, //12 -6.75 -6.75
	7.5f / NORM_VERT, -7.5f / NORM_VERT,   //13 7.75 -6.75
	17.0f / NORM_VERT, 6.0f / NORM_VERT,     //14 17 6.5
	22.0f / NORM_VERT, 5.0f / NORM_VERT,     //15 23 6
	24.0f / NORM_VERT, 11.0f / NORM_VERT,    //16 25 12
};

GLuint indices[] = {
	0, 1, 2,    //1 triangle
	0, 3, 4,    //2 triangle
	0, 4, 5,    //3 triangle
	4, 5, 6,    //4 triangle
	7, 8, 9,    //5 triangle
	9, 10, 11,  //6 triangle
	9, 12, 13,  //7 triangle
	10, 14, 16, //8 triangle
	10, 15, 16  //9 triangle
};

GLuint lines[] = {
	//head
	0, 1,
	1, 2,
	0, 2,
	2, 3,
	3, 4,

	//head2
	0, 4,
	0, 5,
	4, 6,
	5, 6,

	//legs and body
	6, 7,
	7, 9,
	9, 11, 
	12, 13,
	8, 10,
	10, 11,

	//tail
	10, 14,
	10, 15, 
	14, 16, 
	15, 16
};


void setupVertexArray(
	unsigned int VAO, 
	unsigned int VBO, 
	unsigned int EBO, 
	const float *vertices,
	const unsigned int *indices, 
	int verticesSize, 
	int indicesSize) 
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
int main() 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lab1", NULL, NULL);
	
	if (window == NULL) {
		std::cout << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//mvp matrix
	glm::mat4 mvp = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

	//glm tranformations
	glm::mat4 strafe_left = glm::translate(glm::mat4(1.0f), glm::vec3(-0.1 * SENSIVITY, 0, 0));
	glm::mat4 strafe_right = glm::translate(glm::mat4(1.0f), glm::vec3(0.1 * SENSIVITY, 0, 0));
	glm::mat4 strafe_up = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.1 * SENSIVITY, 0));
	glm::mat4 strafe_down = glm::translate(glm::mat4(1.0f), glm::vec3(0, -0.1 * SENSIVITY, 0));
	glm::mat4 rotationMatrix1 = glm::rotate(glm::mat4(1.0f), 0.2f * SENSIVITY, glm::vec3(0, 0, 1.0));
	glm::mat4 rotationMatrix2 = glm::rotate(glm::mat4(1.0f), -0.2f * SENSIVITY, glm::vec3(0, 0, 1.0));
	

	Shader shaderProgram("default.vert", "default.frag");
	unsigned int VBO, VAOs[2], EBOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(1, &VBO);
	glGenBuffers(2, EBOs);
	
	//Setting up VAOs and EBOs
	setupVertexArray(VAOs[0], VBO, EBOs[0], vertices, indices, sizeof(vertices), sizeof(indices));
	setupVertexArray(VAOs[1], VBO, EBOs[1], vertices, lines, sizeof(vertices), sizeof(lines));

	glLineWidth(4);
	while (!glfwWindowShouldClose(window)) 
	{
		processInput(
			window,
			mvp,
			strafe_up,
			strafe_down,
			strafe_right,
			strafe_left,
			rotationMatrix1,
			rotationMatrix2);

		//Background color
		glClearColor(1.0f, (float)127 / NORM_COLOR, (float)56 / NORM_COLOR, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//Calling shader program
		glUseProgram(shaderProgram.ID);

		glBindVertexArray(VAOs[0]);//Binding VAO1
		int location = glGetUniformLocation(shaderProgram.ID, "u_Color");

		//Entity color
		glUniform4f(location, 0 , (float)153 / NORM_COLOR, (float)29 / NORM_COLOR, 1.0f);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(VAOs[1]);//Binding VAO2
		location = glGetUniformLocation(shaderProgram.ID, "u_Color");
		glUniform4f(location, 0.0f, 0.0f, 0.0f, 0.0f);
		glDrawElements(GL_LINES, sizeof(lines) / sizeof(lines[0]), GL_UNSIGNED_INT, nullptr);

		//Setting final uniform maxrix
		shaderProgram.SetUniformMat4f("u_MVP", mvp);

		//Swapping between buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Deleting buffers
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(2, EBOs);
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}


//Function for input
void processInput(
	GLFWwindow* window, 
	glm::mat4 &mvp, 
	glm::mat4 &strafe_up,
	glm::mat4 &strafe_down,
	glm::mat4 &strafe_right,
	glm::mat4 &strafe_left,
	glm::mat4 &rotationMatrix1,
	glm::mat4 &rotationMatrix2 )
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		mvp *= strafe_up;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		mvp *= strafe_down;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		mvp *= strafe_right;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		mvp *= strafe_left;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		mvp *= rotationMatrix1;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		mvp *= rotationMatrix2;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}