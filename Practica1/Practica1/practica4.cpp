/*
Importante: En este código no está declarado las variable uniform color, tampoco en todos los archivos que modificamos en clase la sesión pasada, por lo que 
si quieren usar la variable uniform para color debend e agregar todas las declaraciones de la sesión pasada en los archivos Shader.h, Shader.cpp, shader.vert.
El archivo Window.h y Window.cpp tiene los cambios para la rotación alrededor del eje Y.
EL archivo Shader.h y el archivo Shader.cpp tienen las declaraciones de la variable uniform para la matriz de vista 
*/
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include "Sphere.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
Sphere sp = Sphere(1.0, 20, 20);

void CreateObject()
{
	unsigned int indices[] = { 
		0,3,1,
		1,3,2,
		2,3,0,
		0,1,2
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.5f,
		0.0f,-0.5f,0.5f,
		0.5f,-0.5f, 0.0f,
		0.0f,0.5f,0.0f
	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}
//ejercicio 1 para hacer en clase, el cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);

}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//CreateObject();//triangulo
	CrearCubo();
	CrearCubo();
	CrearCubo();
	CrearCubo();
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::vec3 direccion = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));// ya está indicado, pero todavía no lo modificaremos
	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		glm::mat4 model(1.0); 
		//model = glm::mat4(1.0);
		//Traslación
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		//////////////// ROTACIÓN //////////////////
		//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		////////////////  ESCALA ////////////////

		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		///////////////////// T+R////////////////
		/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		*/
		/////////////R+T//////////
		/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
		*/
		glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 4.5f));
		model = glm::scale(model, glm::vec3(0.9f, 0.85f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay() * toRadians), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla Y se rota sobre el eje y
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();
		//sp.render(); //Renderiza esfera

		
		meshList[0]->RenderMesh();

		//ejercicio : instanciar primitivas para crear su propio objeto
		model = glm::mat4(1.0f);
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.5f));
		model = glm::scale(model, glm::vec3(1.0f, 0.6f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay() * toRadians), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla Y se rota sobre el eje y
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();



		///
		model = glm::mat4(1.0f);
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.35f, -3.5f));
		model = glm::scale(model, glm::vec3(0.5f, 0.2f, 0.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay() * toRadians), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla Y se rota sobre el eje y
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMesh();

		///
		model = glm::mat4(1.0f);
		color = glm::vec3(1.0f, 1.0f, 0.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay() * toRadians), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla Y se rota sobre el eje y
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//meshList[3]->RenderMesh();
		sp.render(); //Renderiza esfera

				///
		model = glm::mat4(1.0f);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(-0.4f, 0.35f, -3.5f));
		model = glm::scale(model, glm::vec3(0.15f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay() * toRadians), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla Y se rota sobre el eje y
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[3]->RenderMesh();
		

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}