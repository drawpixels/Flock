/*
	Template.cpp

	Starting point template for creating a 2D graphics application
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <common/shader.hpp>

GLFWwindow* g_MainWindow;

char g_WindowTitle[] = "** TEMPLATE **";
float g_WindowWidth = 800.0f;
float g_WindowHeight = 800.0f;

void CallbackWindowSize (GLFWwindow*, int, int);
void CallbackKey (GLFWwindow*, int, int, int, int);
void CallbackCursonPos (GLFWwindow*, double, double);
void CallbackButton (GLFWwindow*, int, int, int);
GLFWwindow* CreateWindow (const char* title, const int width, const int height);
GLuint CreateObject ();

int main (int argc, char** argv)
{
	// Initialise GLFW
	if (!glfwInit()) {
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}
	// This is requied for OS X
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// A window must be created first before init GLEW
	g_MainWindow = glfwCreateWindow( (int)g_WindowWidth, (int)g_WindowHeight, g_WindowTitle, NULL, NULL);
	if( g_MainWindow == NULL ){
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		return -1;
	}

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Create and compile our GLSL program from the shaders for Affine transform and calculate SSD
	GLuint shader = LoadShaders("Instancing.vert", "Instancing.frag");
    glUseProgram (shader);

	// Set call back functions
	glfwMakeContextCurrent(g_MainWindow);
	glfwSetWindowSizeCallback(g_MainWindow,CallbackWindowSize);
	glfwSetKeyCallback(g_MainWindow,CallbackKey);
	glfwSetCursorPosCallback(g_MainWindow,CallbackCursonPos);
	glfwSetMouseButtonCallback(g_MainWindow,CallbackButton);
	// Ensure we can capture the escape key being pressed below
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Create frame VAO to display image
	GLuint vaoObject = CreateObject();
	
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(vaoObject);
	glDrawArraysInstanced(GL_LINES,0,4,5);

	// Swap buffers
	glfwSwapBuffers(g_MainWindow);

	while (!glfwWindowShouldClose(g_MainWindow)) {
		glfwPollEvents();
	} 

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

//---------------------------------
// Supporting call-back functions
//---------------------------------
void CallbackWindowSize (GLFWwindow* window, int width, int height) {
	g_WindowWidth = (double)width;
	g_WindowHeight = (double)height;
}


void CallbackKey (GLFWwindow* window, int key, int scancode, int action, int mods) {
	//printf("KEY - %d %d %d %d\n",key,scancode,action,mods);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void CallbackCursonPos (GLFWwindow* window, double x, double y) {
	bool bMB1 = (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_1)==GLFW_PRESS);
	bool bMB2 = (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_2)==GLFW_PRESS);
	bool bMB3 = (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_3)==GLFW_PRESS);
	bool bCtrl = (glfwGetKey(window,GLFW_KEY_LEFT_CONTROL)==GLFW_PRESS) || (glfwGetKey(window,GLFW_KEY_RIGHT_CONTROL)==GLFW_PRESS);
}

void CallbackButton (GLFWwindow* window, int button, int action, int mods) {
	double x,y;
	glfwGetCursorPos(window,&x,&y);
	//printf("BUTTON - %6.1f %6.1f %d %d %d\n",x,y,button,action,mods);
	if (button==GLFW_MOUSE_BUTTON_1 && action==GLFW_PRESS) {

	} 
}

GLuint CreateObject () {
	GLfloat fVertex[] = {
		-0.1f,  0.0f,
		 0.1f,  0.0f,
		 0.0f, -0.1f,
		 0.0f,  0.1f
	};
	GLfloat fColor[] = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
	};
	GLfloat fPosition[] = {
		 0.0f,  0.0f, 
		-0.5f,  0.0f,
		 0.5f,  0.0f,
		 0.0f, -0.5f, 
		 0.0f,  0.5f
	};

	GLuint Vao;
	glGenVertexArrays(1, &Vao);
	glBindVertexArray(Vao);

	GLuint Vbo[3];
	glGenBuffers(3, Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, Vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fVertex), fVertex, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,(void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, Vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fColor), fColor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, Vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fPosition), fPosition, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,0,(void*)0);
	glVertexAttribDivisor(2,1);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	return Vao;
}
