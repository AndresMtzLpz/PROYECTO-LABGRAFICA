#include <stdio.h>
#include <glew.h>
#include <glfw3.h>
//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 600;

int main()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //3.2 y sube, es de preferencia minimo 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Primer ventana", NULL, NULL); //4to monitor, 5to share ventana entre monitores

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer - alto*ancho*modelo de color, es para ver cuanta memoria de video voy a ocupar 800*600*24 bits, lo pasamos a megabites
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto - a que ventana vamos a manejar osea la ventana actual
	glfwMakeContextCurrent(mainWindow);

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}
	//glsl intel, cg nvidia
	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);
	printf("Version de Opengl: %s \n",glGetString(GL_VERSION));
	printf("Marca: %s \n", glGetString(GL_VENDOR));
	printf("Renderer: %s \n", glGetString(GL_RENDERER));
	printf("Shaders: %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	//Loop mientras no se cierra la ventana - se realiza cada ciclo de reloj
	float rojo = 0.0, azul = 0.0, verde = 0.0, transparencia = 0.0; //
	while (!glfwWindowShouldClose(mainWindow))
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//
		if (rojo >= 1.0) {
			rojo = 0.0;
		}
		else {
			rojo += 0.001;
		}
		if (azul >= 1.0) {
			azul = 1.0;
		}
		else {
			azul += 0.002;
		}
		if (verde >= 1.0) {
			verde = 0.0;
		}
		else {
			verde += 0.003;
		}

		//
		//Limpiar la ventana
		glClearColor(rojo, azul, verde, 1.0f); //rojo, verde, azul, transparencia
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(mainWindow); //para realizar cambio entre dos buffers

	}

	return 0;
}