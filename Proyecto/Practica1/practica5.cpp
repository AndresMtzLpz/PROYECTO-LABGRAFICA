/*
Semestre 2020-2
Práctica 5 Modelado Jerárquico
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
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
float codo = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
Sphere sp = Sphere(1, 20, 20);

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
	//CreateObject();
	CrearCubo();
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//Loop mientras no se cierra la ventana
	sp.init();
	sp.load();

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
		glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);
		//Creando un brazo formado por hombro, brazo,codo, antebrazo, muñeca,palma, dedos.
		//hombro, codo y muñeca sólo son puntos de rotación o articulación, en este caso no dibujaremos esferas que los representen
	
		//Creando el hombro 
		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);//Guardar la traslacion y rotacion
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -3.0f));
		model = glm::rotate(model, glm::radians(mainWindow.gethombro()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;//Aqui se guarda
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();
	
		//brazo
		//model = glm::mat4(1.0); //omitir estos resets y cambiarlos por matriz auxiliar que nos ayude a mantener jerarquía
		//Se inicializa a modelaux
		model = modelaux;
		//LOQUITAMOS ----- model = glm::rotate(model, glm::radians(mainWindow.gethombro() * toRadians), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f)); //En Z ya no lo movemos
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[0]->RenderMesh();
		model = modelaux;

		//codo
		model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getcodo()), glm::vec3(0.0f, 0.0f, 1.0f));

		/*En caso de ser el codo una esfera se tiene que desplazar segun su centro
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.15f, 0.0f, -1.7f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		*/
		
		//antebrazo
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();
		/*En caso de ser la muñeca una esfera
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.25f, 0.0f, -1.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); */

		//muñeca

		model = modelaux; //Para desacerse de la escala
		model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getmuneca()), glm::vec3(0.0f, 0.0f, 1.0f));



		//palma
		color = glm::vec3(1.0f, 1.0f, 0.0f);
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.4f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();
		model = modelaux;
		glm::mat4 posicionpalma(1.0);
		posicionpalma = model;

		////////////DEDO 01

		//nudillo indice
		model = glm::translate(model,glm::vec3(0.15f,0.15f,0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalange_indice()), glm::vec3(0.0f, -1.0f, 0.0f));

		//falange dedo índice
		color = glm::vec3(1.0f, 0.9f, 0.9f);
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.1f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//articulacion falange falangina indice
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalangina_indice()), glm::vec3(0.0f, -1.0f, 0.0f));

		//falangina dedo índice
		//modelaux = model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.12f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.25f, 0.09f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//articulacion falangina falangeta indice
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.15f,0.0f,0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalangeta_indice()), glm::vec3(0.0f, -1.0f, 0.0f));

		//falangeta dedo índice
		model = glm::translate(model, glm::vec3(0.08f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.20f, 0.08f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		model = modelaux = posicionpalma;
		 //completar la mano y agregar colores diferentes a cada dedo.

		////////////////DEDO 02


		//nudillo anulas
		model = glm::translate(model, glm::vec3(0.15f, 0.05f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalange_indice()), glm::vec3(0.0f, -1.0f, 0.0f));

		//falange dedo anular
		color = glm::vec3(0.2f, 0.2f, 0.2f);
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.1f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//articulacion falange falangina anular
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalangina_indice()), glm::vec3(0.0f, -1.0f, 0.0f));

		//falangina dedo anular
		//modelaux = model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.09f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//articulacion falangina falangeta anular
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalangeta_indice()), glm::vec3(0.0f, -1.0f, 0.0f));

		//falangeta dedo anular
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.08f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		model = modelaux = posicionpalma;


		/////////////////////// DEDO 03


		//nudillo anulas
		model = glm::translate(model, glm::vec3(0.15f, -0.05f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalange_indice()), glm::vec3(0.0f, -1.0f, 0.0f));

		//falange dedo anular
		color = glm::vec3(1.0f, 0.5f, 0.5f);
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.1f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//articulacion falange falangina anular
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalangina_indice()), glm::vec3(0.0f, -1.0f, 0.0f));

		//falangina dedo anular
		//modelaux = model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.09f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//articulacion falangina falangeta anular
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalangeta_indice()), glm::vec3(0.0f, -1.0f, 0.0f));

		//falangeta dedo anular
		model = glm::translate(model, glm::vec3(0.11f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.22f, 0.08f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		model = modelaux = posicionpalma;


		//////////////////////// DEDO 04


		//nudillo anulas
		model = glm::translate(model, glm::vec3(0.15f, -0.15f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalange_indice()), glm::vec3(0.0f, -1.0f, 0.0f));

		//falange dedo anular
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(0.14f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.1f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//articulacion falange falangina anular
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalangina_indice()), glm::vec3(0.0f, -1.0f, 0.0f));

		//falangina dedo anular
		//modelaux = model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.10f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.25f, 0.09f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//articulacion falangina falangeta anular
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalangeta_indice()), glm::vec3(0.0f, -1.0f, 0.0f));

		//falangeta dedo anular
		model = glm::translate(model, glm::vec3(0.07f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.20f, 0.08f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		model = modelaux = posicionpalma;

		//////////// pulgar


		//nudillo anulas
		model = glm::translate(model, glm::vec3(0.15f, -0.15f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalange_indice()), glm::vec3(0.0f, -1.0f, 0.0f));

		//falange dedo anular
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(-0.1f, 0.4f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.1f, 0.25f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//articulacion falange falangina anular
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getfalangina_indice()), glm::vec3(1.0f, 0.0f, 0.0f));

		//falangina dedo anular
		//modelaux = model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.15f, 0.25f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.09f, 0.25f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();


		model = modelaux = posicionpalma;


		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}