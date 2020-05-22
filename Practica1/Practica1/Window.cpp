#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 2.0f;
	mueveHombroD = 0.0f;
	mueveHombroI = 0.0f;
	muevePieD = 0.0f;
	muevePieI = 0.0f;
	mueveBatmanZ = 0.0f;
	mueveBatmanX = 0.0f;
	giraCaderaX = 0.0f;
	giraCaderaY = 0.0f;
	giroQuiosco = 0.0f;
	giroCabeza = 0.0f;
	abrirPuerta = 0.0f;

	botonKi = false;
	audio = true;
	digiEvolucion = false;
	UFO = false;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicializaci�n de GLFW
	if (!glfwInit())
	{
		printf("Fall� inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica 10", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tama�o de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fall� inicializaci�n de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se est� usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}



void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_Y)
	{
		theWindow->muevex += 1.0;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow->muevex -= 1.0;
	}
	////BATMAN LEGO
	if (key == GLFW_KEY_Y)
	{
		theWindow->mueveHombroD += 10.0;
	}
	if (key == GLFW_KEY_R)
	{
		theWindow->mueveHombroI += 10.0;
	}
	if (key == GLFW_KEY_C)
	{
		theWindow->muevePieD += 10.0;
	}
	if (key == GLFW_KEY_X)
	{
		theWindow->muevePieI += 10.0;
	}
	if (key == GLFW_KEY_T)
	{
		theWindow->mueveBatmanZ += 1.0;
	}
	if (key == GLFW_KEY_G)
	{
		theWindow->mueveBatmanZ -= 1.0;
	}
	if (key == GLFW_KEY_F)
	{
		theWindow->mueveBatmanX += 1.0;
	}
	if (key == GLFW_KEY_H)
	{
		theWindow->mueveBatmanX -= 1.0;
	}
	if (key == GLFW_KEY_V)
	{
		theWindow->giraCaderaX += 10.0;
	}
	if (key == GLFW_KEY_B)
	{
		theWindow->giraCaderaY += 10.0;
	}

	if (key == GLFW_KEY_Z)
	{
		theWindow->giroCabeza += 1.0;
	}
	/////////PUERTA BANO/////////////
	if (key == GLFW_KEY_0 && theWindow->abrirPuerta < 90.0)
	{
		theWindow->abrirPuerta += 10.0;
	}
	if (key == GLFW_KEY_9 && theWindow->abrirPuerta > 0.0)
	{
		theWindow->abrirPuerta -= 10.0;
	}
	/////////QUIOSCO///////////////////
	if (key == GLFW_KEY_N)
	{
		theWindow->giroQuiosco += 5.0;

	}
		if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		{
			theWindow->botonKi = !theWindow->botonKi;

		}

		if (key == GLFW_KEY_M && action == GLFW_PRESS)
		{
			theWindow->audio = !theWindow->audio;
		}

	/*Nuevas teclas*/

	if (key == GLFW_KEY_7 && action == GLFW_PRESS)
	{
		theWindow->digiEvolucion = !theWindow->digiEvolucion;

	}

	if (key == GLFW_KEY_8 && action == GLFW_PRESS)
	{
		theWindow->UFO = !theWindow->UFO;

	}
		if (key >= 0 && key < 1024)
		{
			if (action == GLFW_PRESS)
			{
				theWindow->keys[key] = true;
				printf("se presiono la tecla %d'\n", key);
			}
			else if (action == GLFW_RELEASE)
			{
				theWindow->keys[key] = false;
				printf("se solto la tecla %d'\n", key);
			}
		}
	
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{

	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
