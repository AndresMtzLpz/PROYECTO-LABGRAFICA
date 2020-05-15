#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getHombroD() { return mueveHombroD; }
	GLfloat getHombroI() { return mueveHombroI; }
	GLfloat getPieD() { return muevePieD; }
	GLfloat getPieI() { return muevePieI; }
	GLfloat getMovBatmanZ() { return mueveBatmanZ; }
	GLfloat getMovBatmanX() { return mueveBatmanX; }
	GLfloat getGiroCaderaX() { return giraCaderaX; }
	GLfloat getGiroCaderaY() { return giraCaderaY; }
	GLfloat getGiroQuiosco() { return giroQuiosco; }
	GLfloat getGiroCabeza() { return giroCabeza; }
	GLfloat getAbrirPuerta() { return abrirPuerta; }
	GLfloat getmuevex() { return muevex; }
	bool getbutonKi() { return botonKi; }
	bool getAudio() { return audio; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	GLfloat mueveBatmanZ;
	GLfloat mueveBatmanX;
	GLfloat giraCaderaX;
	GLfloat giraCaderaY;
	GLfloat mueveHombroD;
	GLfloat mueveHombroI;
	GLfloat muevePieD;
	GLfloat muevePieI;
	GLfloat giroCabeza;
	GLfloat giroQuiosco;
	GLfloat abrirPuerta;

	bool botonKi;
	bool audio;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

