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
#include "Bird.hpp"

#define NUM_OF_BIRDS 20

GLFWwindow* g_MainWindow;

char g_WindowTitle[] = "FLOCK";
float g_WindowWidth = 800.0f;
float g_WindowHeight = 800.0f;
bool g_running = true;
bool g_disp_range = false;
bool g_color = false;

void CallbackWindowSize (GLFWwindow*, int, int);
void CallbackKey (GLFWwindow*, int, int, int, int);
void CallbackCursonPos (GLFWwindow*, double, double);
void CallbackButton (GLFWwindow*, int, int, int);
GLFWwindow* CreateWindow (const char* title, const int width, const int height);
GLuint CreateObject (GLuint& vboPos, GLuint& vboOri, GLuint& vboColor);
void InitBirds (Bird *b, int num);
void DisplayBirds (Bird *b, int num, GLuint vboPos, GLuint vboOri, GLuint vboColor);
float Random (float upper, float lower);

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

	// Load shaders
	GLuint shader = LoadShaders("Instancing.vert", "Instancing.frag");
    glUseProgram (shader);
	GLuint ColorID = glGetUniformLocation(shader, "gColor");

	// Set call back functions
	glfwMakeContextCurrent(g_MainWindow);
	glfwSetWindowSizeCallback(g_MainWindow,CallbackWindowSize);
	glfwSetKeyCallback(g_MainWindow,CallbackKey);
	glfwSetCursorPosCallback(g_MainWindow,CallbackCursonPos);
	glfwSetMouseButtonCallback(g_MainWindow,CallbackButton);
	// Ensure we can capture the escape key being pressed below
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Create frame VAO to display image
	GLuint vboPos, vboOri, vboColor;
	GLuint vaoObject = CreateObject(vboPos,vboOri,vboColor);

	Bird birds[NUM_OF_BIRDS];
	InitBirds(birds,NUM_OF_BIRDS);
	
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);

	while (!glfwWindowShouldClose(g_MainWindow)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (g_running) {
			for (int i=0; i<NUM_OF_BIRDS; i++) {
				birds[i].Align(birds,NUM_OF_BIRDS,i);
				birds[i].Move(0.05);
			}
		}

		DisplayBirds(birds,NUM_OF_BIRDS,vboPos,vboOri,vboColor);
		glBindVertexArray(vaoObject);
		if (g_color)
			glUniform3f(ColorID,0.0f,0.0f,0.0f);
		else
			glUniform3f(ColorID,1.0f,1.0f,1.0f);
		glDrawArraysInstanced(GL_TRIANGLES,0,9,NUM_OF_BIRDS);
		if (g_disp_range)
			glDrawArraysInstanced(GL_LINE_STRIP,9,11,NUM_OF_BIRDS);

		// Swap buffers
		glfwSwapBuffers(g_MainWindow);

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
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		g_running = !g_running;
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
		g_disp_range = !g_disp_range;
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
		g_color = !g_color;
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

GLuint CreateObject (GLuint& vboPos, GLuint& vboOri, GLuint& vboColor) {
	GLfloat fVertex[] = {
		 0.00f,  0.05f,
		-0.01f, -0.03f,
		 0.01f, -0.03f,
		 0.04f,  0.01f,
		-0.04f,  0.01f,
		-0.05f,  0.00f,
		 0.04f,  0.01f,
 		-0.05f,  0.00f,
		 0.05f,  0.00f,
		-0.2394, -0.3205,
		-0.3637, -0.1665,
		-0.3990,  0.0283,
		-0.3366,  0.2161,
		-0.1918,  0.3510,
		 0.0000,  0.4000,
		 0.1918,  0.3510,
		 0.3366,  0.2161,
		 0.3990,  0.0283,
		 0.3637, -0.1665,
		 0.2394, -0.3205
	};
	GLuint Vao;
	glGenVertexArrays(1, &Vao);
	glBindVertexArray(Vao);

	GLuint Vbo[4];
	glGenBuffers(4, Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, Vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fVertex), fVertex, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,(void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, Vbo[1]);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,(void*)0);
	glVertexAttribDivisor(1,1);

	glBindBuffer(GL_ARRAY_BUFFER, Vbo[2]);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,1,GL_FLOAT,GL_FALSE,0,(void*)0);
	glVertexAttribDivisor(2,1);

	glBindBuffer(GL_ARRAY_BUFFER, Vbo[3]);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,0,(void*)0);
	glVertexAttribDivisor(3,1);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	vboPos = Vbo[1];
	vboOri = Vbo[2];
	vboColor = Vbo[3];
	return Vao;
}

void InitBirds(Bird *b, int num) {
	for (int i=0; i<num; i++) {
		b[i].SetPosition (glm::vec2(Random(1.0f,-1.0f),Random(1.0f,-1.0f)));
		b[i].SetBearing (Random(M_PI,-M_PI));
		b[i].SetSpeed (Random(0.1f,0.05f));
	}
}

void DisplayBirds(Bird *b, int num, GLuint vboPos, GLuint vboOri, GLuint vboColor) {
	glm::vec2* pos = (glm::vec2*)malloc(sizeof(glm::vec2)*num);
	float* ori = (float*)malloc(sizeof(float)*num);
	glm::vec3* color = (glm::vec3*)malloc(sizeof(glm::vec3)*num);
	
	for (int i=0; i<num; i++) {
		pos[i] = b[i].GetPosition();
		ori[i] = b[i].GetBearing();
		color[i] = b[i].GetColor();
	}

	glBindBuffer(GL_ARRAY_BUFFER, vboPos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*num, pos, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboOri);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num, ori, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*num, color, GL_DYNAMIC_DRAW);
	
	free(ori);
	free(pos);
	free(color);
}

float Random (float upper, float lower) {
	return (float(rand())/float(RAND_MAX)*(upper-lower)+lower);
}

