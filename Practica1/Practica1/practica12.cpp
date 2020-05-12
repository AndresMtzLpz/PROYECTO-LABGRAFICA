/*
Semestre 2020-2
Animaci�n por keyframes
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"
#include"SpotLight.h"
#include "Luminaria.h"

#include "Movimiento.h"
#include "AnimacionKF.h"




const float toRadians = 3.14159265f / 180.0f;
bool bandera;
bool banderaB;

//////VARIABLES BATMAN LEGO///////
float posXBatman = 20.0, posYBatman = 2.0f, posZBatman = 0.0;
float	movBatman_x = 0.0f, movBatman_y = 0.0f, movBatman_z = 0.0f;
float giroBatman = 0;
glm::vec3 posBatman = glm::vec3(posXBatman, posYBatman, posZBatman);

/////VARIABLE QUIOSCO////////
glm::vec3 posQuiosco;



Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;


/*Texruras*/
Texture brickTexture;
Texture Tagave;


//materiales
Material Material_brillante;
Material Material_opaco;

//luz direccional
DirectionalLight sunLight;
DirectionalLight moonLight;

//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];




/*Declaraci�n de modelos a usar*/
Model Blackhawk_M;
Model Helices_M;
Model Helicoptero_M;
Model Bloque_plano_M;
Model Bloque_plano_V_M;

///////MODELOS DE BATMAN LEGO//////////////
Model BatmanCabeza_M;  //1
Model BatmanMascara_M; //2
Model BatmanTorax_M;   //3
Model BatmanCinturon_M;//4
Model BatmanCadera_M;  //5
Model BatmanPieD_M;    //6
Model BatmanPieI_M;    //7
Model BatmanBrazoD_M;  //8
Model BatmanBrazoI_M;  //9
Model BatmanManoD_M;   //10
Model BatmanManoI_M;   //11
Model BatmanCapa_M;    //12


///////MODELOS QUIOSCO//////////////
Model Base01_M;  //1
Model Base02_M; //2
Model Piso_M;   //3
Model Barda01_M;//4
Model Barda02_M;  //5
Model Barda03_M;    //6
Model Poste01_M;    //7
Model Poste02_M;    //8
Model Poste03_M;    //9
Model Poste04_M;    //10
Model Poste05_M;    //11
Model Poste06_M;    //12
Model Poste07_M;    //13
Model Poste08_M;    //14
Model Poste09_M;    //15
Model Poste10_M;    //16
Model Poste11_M;    //17
Model Poste12_M;    //18
Model Poste13_M;    //19
Model Poste14_M;    //20
Model Poste15_M;    //21
Model Poste16_M;    //22
Model Techo01_M;  //23
Model Techo02_M;  //24
Model Techo03_M;  //25
Model Techo04_M;  //26
Model Techo05_M;  //27
Model Techo06_M;  //28
Model Techo07_M;  //29
Model Escalon01_M;//30
Model Escalon02_M;//31
Model Escalon03_M;//32
Model Escalon04_M;//33
////////COSAS NUEVAS//////////////////
Model Fuente_M;

Model Arbol_M;
Model Reja_M;
Model Luminaria_M;

Model Banca_M;
Model Bano_M;
Model Arbusto_M;
Model PuertaBano_M;
Model Pasto_M;
Model Pavimento_M;
Model PavimentoCirculo_M;
Model PavimentoCirculito_M;
Model Basura_M;
Model HelicopteroLego_M;
Model HelicesLego_M;
Model HelicesPLego_M;
Model Bicicleta_M;
Model LlantaD_M;
Model LlantaT_M;
Model Avion_Lego_M;
Model Jardinera_M;


//-.-.-.-.-.- -.-.-.-.-.- Animaciones -.-.-.-.-.- -.-.-.-.-.- /
AnimacionKF animacion;
Movimiento movimiento;

Skybox skyboxA;
Skybox skyboxB;
Skybox skybox;


Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

//void my_input(GLFWwindow *window);
void inputKeyframes(bool* keys);

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";
//c�lculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreaPiso() {
	unsigned int floorIndices[] = {
	0, 2, 1,
	1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -2.0f,	0.0f,	0.56f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -2.0f,		1.0f,	0.56f,		0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 2.0f,		0.0f,	0.36f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 2.0f,		1.0f,	0.36f,		0.0f, -1.0f, 0.0f

	};

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateShaders();
	//CreaPiso();

	/*Inicializaci�n para los objetos de animaci�n*/
	animacion = AnimacionKF();
	camera = Camera(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 10.0f, 1.2f);

	/*Carga de texturas*/
	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	Tagave = Texture("Textures/Agave.tga");
	Tagave.LoadTextureA();

	/*Materiales*/
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.1f, 2);

	/*Craga de modelos*/
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Helices_M = Model();
	Helices_M.LoadModel("Models/heliceprincipal.obj");
	Helicoptero_M = Model();
	Helicoptero_M.LoadModel("Models/Helicoptero.fbx");
	Bloque_plano_M = Model();
	Bloque_plano_M.LoadModel("Models/Bloques/Legos.obj");
	Bloque_plano_V_M = Model();
	Bloque_plano_V_M.LoadModel("Models/Bloques/b_plano_v.obj");
	Avion_Lego_M = Model();
	Avion_Lego_M.LoadModel("Models/Bloques/Avion.obj");
	Jardinera_M = Model();
	Jardinera_M.LoadModel("Models/Bloques/jardinera.obj");



	
	//////////////////BATMAN LEGO ////////////////////////7//
	
	BatmanCabeza_M = Model();  //1
	BatmanCabeza_M.LoadModel("Models/BatmanCabeza.obj");
	BatmanMascara_M = Model(); //2
	BatmanMascara_M.LoadModel("Models/BatmanMascara.obj");
	BatmanTorax_M = Model();  //3
	BatmanTorax_M.LoadModel("Models/Torax.obj");
	BatmanCinturon_M = Model();//4
	BatmanCinturon_M.LoadModel("Models/BatmanCinturon.obj");
	BatmanCadera_M = Model();  //5
	BatmanCadera_M.LoadModel("Models/BatmanCadera.obj");
	BatmanPieD_M = Model();    //6
	BatmanPieD_M.LoadModel("Models/BatmanPieD.obj");
	BatmanPieI_M = Model();    //7
	BatmanPieI_M.LoadModel("Models/BatmanPieI.obj");
	BatmanBrazoD_M = Model();  //8
	BatmanBrazoD_M.LoadModel("Models/BatmanBrazoD.obj");
	BatmanBrazoI_M = Model();  //9
	BatmanBrazoI_M.LoadModel("Models/BatmanBrazoI.obj");
	BatmanManoD_M = Model();   //10
	BatmanManoD_M.LoadModel("Models/BatmanManoD.obj");
	BatmanManoI_M = Model();   //11
	BatmanManoI_M.LoadModel("Models/BatmanManoI.obj");
	BatmanCapa_M = Model();    //12
	BatmanCapa_M.LoadModel("Models/BatmanCapa.obj");
	
	
	
	///////////////////////QUIOSCO LEGO /////////////////////
	Base01_M = Model();
	Base01_M.LoadModel("Models/Quiosco/Base01.obj");
	Base02_M = Model();
	Base02_M.LoadModel("Models/Quiosco/Base02.obj");
	Escalon01_M = Model();
	Escalon01_M.LoadModel("Models/Quiosco/Escalon01.obj");
	Escalon02_M = Model();
	Escalon02_M.LoadModel("Models/Quiosco/Escalon02.obj");
	Escalon03_M = Model();
	Escalon03_M.LoadModel("Models/Quiosco/Escalon03.obj");
	Escalon04_M = Model();
	Escalon04_M.LoadModel("Models/Quiosco/Escalon04.obj");
	Piso_M = Model();
	Piso_M.LoadModel("Models/Quiosco/Piso.obj");
	Barda01_M = Model();
	Barda01_M.LoadModel("Models/Quiosco/Barda01.obj");
	Barda02_M = Model();
	Barda02_M.LoadModel("Models/Quiosco/Barda02.obj");
	Barda03_M = Model();
	Barda03_M.LoadModel("Models/Quiosco/Barda03.obj");
	Poste01_M = Model();
	Poste01_M.LoadModel("Models/Quiosco/Poste01.obj");
	Poste02_M = Model();
	Poste02_M.LoadModel("Models/Quiosco/Poste02.obj");
	Poste03_M = Model();
	Poste03_M.LoadModel("Models/Quiosco/Poste03.obj");
	Poste04_M = Model();
	Poste04_M.LoadModel("Models/Quiosco/Poste04.obj");
	Poste05_M = Model();
	Poste05_M.LoadModel("Models/Quiosco/Poste05.obj");
	Poste06_M = Model();
	Poste06_M.LoadModel("Models/Quiosco/Poste06.obj");
	Poste07_M = Model();
	Poste07_M.LoadModel("Models/Quiosco/Poste07.obj");
	Poste08_M = Model();
	Poste08_M.LoadModel("Models/Quiosco/Poste08.obj");
	Poste09_M = Model();
	Poste09_M.LoadModel("Models/Quiosco/Poste09.obj");
	Poste10_M = Model();
	Poste10_M.LoadModel("Models/Quiosco/Poste10.obj");
	Poste11_M = Model();
	Poste11_M.LoadModel("Models/Quiosco/Poste11.obj");
	Poste12_M = Model();
	Poste12_M.LoadModel("Models/Quiosco/Poste12.obj");
	Poste13_M = Model();
	Poste13_M.LoadModel("Models/Quiosco/Poste13.obj");
	Poste14_M = Model();
	Poste14_M.LoadModel("Models/Quiosco/Poste14.obj");
	Poste15_M = Model();
	Poste15_M.LoadModel("Models/Quiosco/Poste15.obj");
	Poste16_M = Model();
	Poste16_M.LoadModel("Models/Quiosco/Poste16.obj");
	Techo01_M = Model();
	Techo01_M.LoadModel("Models/Quiosco/Techo01.obj");
	Techo02_M = Model();
	Techo02_M.LoadModel("Models/Quiosco/Techo02.obj");
	Techo03_M = Model();
	Techo03_M.LoadModel("Models/Quiosco/Techo03.obj");
	Techo04_M = Model();
	Techo04_M.LoadModel("Models/Quiosco/Techo04.obj");
	Techo05_M = Model();
	Techo05_M.LoadModel("Models/Quiosco/Techo05.obj");
	Techo06_M = Model();
	Techo06_M.LoadModel("Models/Quiosco/Techo06.obj");
	Techo07_M = Model();
	Techo07_M.LoadModel("Models/Quiosco/Techo07.obj");
	


	///// COSAS NUEVAS////////////
	Fuente_M = Model();
	Fuente_M.LoadModel("Models/Nuevo/Fuente.obj");
	Arbol_M = Model();
	Arbol_M.LoadModel("Models/Nuevo/Arbol.fbx");
	Reja_M = Model();
	Reja_M.LoadModel("Models/Nuevo/Reja.obj");
	Luminaria_M = Model();
	Luminaria_M.LoadModel("Models/Nuevo/Lampara.fbx");
	Banca_M = Model();
	Banca_M.LoadModel("Models/Nuevo/banca.obj");
	Bano_M = Model();
	Bano_M.LoadModel("Models/Nuevo/bano.obj");
	Arbusto_M = Model();
	Arbusto_M.LoadModel("Models/Nuevo/Arbusto.obj");
	PuertaBano_M = Model();
	PuertaBano_M.LoadModel("Models/Nuevo/PuertaBano.fbx");
	Pasto_M = Model();
	Pasto_M.LoadModel("Models/Nuevo/pasto.obj");
	Pavimento_M = Model();
	Pavimento_M.LoadModel("Models/Nuevo/pavimento.obj");
	PavimentoCirculo_M = Model();
	PavimentoCirculo_M.LoadModel("Models/Nuevo/pavimentoCirculo.obj");
	PavimentoCirculito_M = Model();
	PavimentoCirculito_M.LoadModel("Models/Nuevo/pavimentoCirculito.obj");
	Basura_M = Model();
	Basura_M.LoadModel("Models/Nuevo/basura.obj");
	HelicopteroLego_M = Model();
	HelicopteroLego_M.LoadModel("Models/Nuevo/Helicoptero.obj");
	HelicesLego_M = Model();
	HelicesLego_M.LoadModel("Models/Nuevo/Helices.obj");
	HelicesPLego_M = Model();
	HelicesPLego_M.LoadModel("Models/Nuevo/HelicePequena.obj");
	Bicicleta_M = Model();
	Bicicleta_M.LoadModel("Models/Nuevo/Bicicleta.obj");
	LlantaD_M = Model();
	LlantaD_M.LoadModel("Models/Nuevo/LlantaD.obj");
	LlantaT_M = Model();
	LlantaT_M.LoadModel("Models/Nuevo/LlantaT.obj");


	glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);
	glm::vec3 posbici = glm::vec3(2.0f, 0.0f, 0.0f);

	///////// ESCENRAIO DIA Y NOCHE /////////////
	////Dia
	std::vector<std::string> skyboxFacesA;
	skyboxFacesA.push_back("Textures/Skybox/IceRiver/posx.jpg");
	skyboxFacesA.push_back("Textures/Skybox/IceRiver/negx.jpg");
	skyboxFacesA.push_back("Textures/Skybox/IceRiver/negy.jpg");
	skyboxFacesA.push_back("Textures/Skybox/IceRiver/posy.jpg");
	skyboxFacesA.push_back("Textures/Skybox/IceRiver/posz.jpg");
	skyboxFacesA.push_back("Textures/Skybox/IceRiver/negz.jpg");
	skyboxA = Skybox(skyboxFacesA);

	////Noche
	std::vector<std::string> skyboxFacesB;
	skyboxFacesB.push_back("Textures/Skybox/posx.jpg");
	skyboxFacesB.push_back("Textures/Skybox/negx.jpg");
	skyboxFacesB.push_back("Textures/Skybox/negy.jpg");
	skyboxFacesB.push_back("Textures/Skybox/posy.jpg");
	skyboxFacesB.push_back("Textures/Skybox/posz.jpg");
	skyboxFacesB.push_back("Textures/Skybox/negz.jpg");
	skyboxB = Skybox(skyboxFacesB);





	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);



	movimiento = Movimiento();
	movimiento.time();
	Luminaria lum;
	lum = Luminaria(pointLights, spotLights);
	int itBloc, itBloc2;
	glm::mat4 modelblo(1.0);

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Se Ejecuta una unica vez por cada ciclo
		movimiento.time();
		deltaTime = movimiento.getDeltaTime();


		/*Animaci�n para dia y noche*/
		skybox = movimiento.horaDia() >= 600 && movimiento.horaDia() < 1800 ?
			skyboxA : skyboxB;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());


		//para keyframes
		inputKeyframes(mainWindow.getsKeys());
		animacion.animate();
		animacion.inputKeyframes(mainWindow.getsKeys());


		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;


		/* Luces */

		lum.setLuminaria(movimiento.horaDia(), mainWindow.getbutonKi());
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		/*Luces del kiosco*/
		if (mainWindow.getbutonKi()) {
			spotLights[1].SetFlash(glm::vec3(0.0f, 3.0f, 0.0f), movimiento.getMovCir());
			spotLights[2].SetFlash(movimiento.getMovLin(), glm::vec3(0.0f, -1.0f, 0.0f));
			spotLights[3].SetFlash(movimiento.getMovLin_2(), glm::vec3(0.0f, -1.0f, 0.0f));
		}

		shaderList[0].SetDirectionalLight(lum.getDirectional());
		shaderList[0].SetPointLights(pointLights, lum.getPointCount());
		shaderList[0].SetSpotLights(spotLights, lum.getSpotCount());



		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		/*Pasto */
		glm::mat4 model(1.0);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(72.0f, -0.75f, 72.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.10, 0.10, 0.10));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		modelblo = model;
		for (itBloc2 = 0; itBloc2 < 47; itBloc2++)
		{
			model = modelblo;
			modelblo = glm::translate(modelblo, glm::vec3(0.0f, 0.0f, -31));
			for (itBloc = 0; itBloc < 89; itBloc++)
			{
				model = glm::translate(model, glm::vec3(15.80f, 0.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Bloque_plano_V_M.RenderModel();
			}
		}

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 72.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.10, 0.10, 0.10));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		modelblo = model;
		for (itBloc2 = 0; itBloc2 < 7; itBloc2++)
		{
			model = modelblo;
			modelblo = glm::translate(modelblo, glm::vec3(0.0f, 0.0f, -31));
			for (itBloc = 0; itBloc < 89; itBloc++)
			{
				model = glm::translate(model, glm::vec3(15.80f, 0.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Bloque_plano_M.RenderModel();
			}
		}

		//Bloques cruzados

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-72.0f, 0.0f, 9.0f));
		model = glm::scale(model, glm::vec3(0.10, 0.10, 0.10));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		modelblo = model;
		for (itBloc2 = 0; itBloc2 < 6; itBloc2++)
		{
			model = modelblo;
			modelblo = glm::translate(modelblo, glm::vec3(0.0f, 0.0f, -31)); //Desplazamiento de linea
			for (itBloc = 0; itBloc < 39; itBloc++)
			{
				model = glm::translate(model, glm::vec3(15.8f, 0.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Bloque_plano_M.RenderModel();
			}
		}

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(8.5f, 0.0f, 9.0f));
		model = glm::scale(model, glm::vec3(0.10, 0.10, 0.10));
		modelblo = model;

		for (itBloc2 = 0; itBloc2 < 6; itBloc2++)
		{
			model = modelblo;
			modelblo = glm::translate(modelblo, glm::vec3(0.0f, 0.0f, -31));
			for (itBloc = 0; itBloc < 40; itBloc++)
			{
				model = glm::translate(model, glm::vec3(15.8f, 0.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Bloque_plano_M.RenderModel();
			}
		}

		model = glm::mat4(1.0);
		posblackhawk = animacion.movAvion();
		model = glm::translate(model, posblackhawk);
		//model = glm::translate(model, glm::vec3(28.0f, -1.0f, 35.0f));
		model = glm::scale(model, glm::vec3(0.025, 0.025, 0.025));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, animacion.rGiroAvionZ() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, animacion.rGiroAvionX() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, animacion.rVueltaAvion() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Avion_Lego_M.RenderModel();
	
		
		/*
		///Helicoptero por keyframes
		model = glm::mat4(1.0);
		posblackhawk = animacion.movAvion();
		model = glm::translate(model, posblackhawk);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, animacion.rGiroAvion() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Avion_Lego_M.RenderModel(); */

		////////////BATMAN LEGO DIBUJO//////////////////
		
		////Cadera
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);


		model = glm::mat4(1.0);
		glm::mat4 modelaux(1.0);
		model = glm::mat4(1.0);
		posBatman = glm::vec3(mainWindow.getMovBatmanX() + 5.0, posYBatman + movBatman_y , mainWindow.getMovBatmanZ() + 1.0);
		model = glm::translate(model, posBatman);
		model = glm::rotate(model, glm::radians(mainWindow.getGiroCaderaX()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getGiroCaderaY()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.07f, 0.07f, 0.07f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BatmanCadera_M.RenderModel();
		glm::mat4 posicionCadera(1.0);
		posicionCadera = model;
	
		////Cinturon
		posBatman = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posBatman);
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BatmanCinturon_M.RenderModel();
		model = modelaux = posicionCadera;

		////Torax
		posBatman = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posBatman);
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BatmanTorax_M.RenderModel();
		glm::mat4 posicionTorax(1.0);
		posicionTorax = model;

		////HombroD
		model = glm::translate(model, glm::vec3(0.0f, 15.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getHombroD()), glm::vec3(-1.0f, 0.0f, 0.0f));


		////BrazoD
		posBatman = glm::vec3(0.0f, - 15.0f, 0.0f);
		model = glm::translate(model, posBatman);
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BatmanBrazoD_M.RenderModel();

		////Mu�ecaD
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));

		////ManoD
		posBatman = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posBatman);
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BatmanManoD_M.RenderModel();
		model = modelaux = posicionTorax;

		////HombroI
		model = glm::translate(model, glm::vec3(0.0f,  15.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getHombroI()), glm::vec3(1.0f, 0.0f, 0.0f));

		////BrazoI
		posBatman = glm::vec3(0.0f, - 15.0f, 0.0f);
		model = glm::translate(model, posBatman);
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BatmanBrazoI_M.RenderModel();

		////Mu�ecaI
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));

		////ManoI
		posBatman = glm::vec3(0.0f, 0.0f , 0.0f);
		model = glm::translate(model, posBatman);
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BatmanManoI_M.RenderModel();
		model = modelaux = posicionTorax;


		////Cuello
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getGiroCabeza()), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 posicionCuello(1.0);
		posicionCuello = model;

		////Cabeza
		posBatman = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posBatman);
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BatmanCabeza_M.RenderModel();
		model = modelaux = posicionCuello;

		////Mascara
		posBatman = glm::vec3(0.0f,0.0f, 0.0f);
		model = glm::translate(model, posBatman);
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BatmanMascara_M.RenderModel();
		model = modelaux = posicionCuello;

		////CentroPieD
		modelaux = posicionCadera;
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getPieD()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		glm::mat4 posicionPieD(1.0);
		posicionPieD = model;

		////PieD
		posBatman = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posBatman);
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BatmanPieD_M.RenderModel();
		model = modelaux = posicionPieD;

		////CentroPieI
		modelaux = posicionCadera;
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getPieI()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		glm::mat4 posicionPieI(1.0);
		posicionPieI = model;

		////PieI
		model = modelaux;
		posBatman = glm::vec3(0.0f,0.0f, 0.0f);
		model = glm::translate(model, posBatman);
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BatmanPieI_M.RenderModel();
		model = modelaux = posicionTorax;


		////Capa
		posBatman = glm::vec3(0.0, 0.0, 0.0);
		model = glm::translate(model, posBatman);
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BatmanCapa_M.RenderModel();
		model = modelaux = posicionTorax;
		





		///////////////////QUIOSCO DIBUJO///////////////////////

		////Base01
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);


		model = glm::mat4(1.0);
		glm::mat4 modelauxQ(1.0);
		model = glm::mat4(1.0);
		posQuiosco = glm::vec3(0.0, 1.0, 0.0);
		model = glm::translate(model, posQuiosco);
		model = glm::rotate(model, glm::radians(mainWindow.getGiroQuiosco()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Base01_M.RenderModel();
		glm::mat4 posicionBase(1.0);
		posicionBase = model;

		////Base02
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Base02_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Escalon01
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Escalon01_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Escalon02
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Escalon02_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Escalon03
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Escalon03_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Escalon04
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Escalon04_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Piso
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Piso_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Barda01
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Barda01_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Barda02
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Barda02_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Barda03
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Barda03_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Poste01
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Poste01_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Poste02
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Poste02_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Poste03
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Poste03_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Poste04
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Poste04_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Poste05
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Poste05_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Poste06
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Poste06_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Poste07
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Poste07_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Poste08
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Poste08_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Poste09
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Poste09_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Poste10
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Poste10_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Poste11
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Poste11_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Poste12
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Poste12_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Poste13
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Poste13_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Poste14
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Poste14_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Poste15
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Poste15_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Poste16
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Poste16_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Techo01
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Techo01_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Techo02
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Techo02_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Techo03
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Techo03_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Techo04
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Techo04_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Techo05
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Techo05_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Techo06
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Techo06_M.RenderModel();
		model = modelauxQ = posicionBase;

		////Techo07
		posQuiosco = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posQuiosco);
		modelauxQ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Techo07_M.RenderModel();
		model = modelauxQ = posicionBase;

		
		//////HELICES ///////////////////////////
		model = glm::mat4(1.0);
		posblackhawk = movimiento.movBlackHawk(0.0, bandera) + glm::vec3(0.0, -0.2, 0.0);
		model = glm::translate(model, posblackhawk);
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, movimiento.getGiroHelice() * 100, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		HelicesLego_M.RenderModel();


		/////HELICOPTERO 02 ////////////////////
		model = glm::mat4(1.0);
		glm::mat4 modelauxH(1.0);
		model = glm::mat4(1.0);
		posblackhawk = movimiento.movBlackHawk(0.0, bandera);
		model = glm::translate(model, posblackhawk);
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, movimiento.giroBlackHawk(), glm::vec3(0.0f, -1.0f, 0.0f));
		modelauxH = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		HelicopteroLego_M.RenderModel();
		glm::mat4 posicionHelicoptero(1.0);
		posicionHelicoptero = model;

		//////HELICES TRASERA///////////////////////////
		posblackhawk =  glm::vec3(1.3, -1.4, -12.0);
		model = glm::translate(model, posblackhawk);
		model = glm::rotate(model, movimiento.getGiroHelice() * 100, glm::vec3(1.0f, 0.0f, 0.0f));
		modelauxH = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		HelicesPLego_M.RenderModel();
		model = modelauxH = posicionHelicoptero;

		/* Se deja para recordar lo de traspaencia
		//Agave �qu� sucede si lo renderizan antes del coche y de la pista?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.7f, -2.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Tagave.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();
		glDisable(GL_BLEND);*/





		///////////////////////////COSAS NUEVAS//////////////////////////7

		/////Fuente
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Fuente_M.RenderModel();

		/////Arbol 01
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, -1.0f, -30.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, 0.20f, -30.0f));
		model = glm::scale(model, glm::vec3(12.0, 12.00, 12.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Jardinera_M.RenderModel();

		/////Arbol 02
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, -1.0f, -50.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, 0.20f, -50.0f));
		model = glm::scale(model, glm::vec3(12.0, 12.00, 12.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Jardinera_M.RenderModel();

		/////Arbol 03
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, -1.0f, -20.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, 0.20f, -20.0f));
		model = glm::scale(model, glm::vec3(12.0, 12.00, 12.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Jardinera_M.RenderModel();

		/////Arbol 04
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, -1.0f, 30.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 0.20f, 30.0f));
		model = glm::scale(model, glm::vec3(12.0, 12.00, 12.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Jardinera_M.RenderModel();

		/////Arbol 05
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, -1.0f, 50.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 50.0f));
		model = glm::scale(model, glm::vec3(12.0, 12.00, 12.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Jardinera_M.RenderModel();

		/////Arbol 06
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, -1.0f, 20.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 20.0f));
		model = glm::scale(model, glm::vec3(12.0, 12.00, 12.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Jardinera_M.RenderModel();

		/////Reja
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();


		/////Luminaria
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.07f, 0.07f, 0.07f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Luminaria_M.RenderModel();


		/////Banca
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.9f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Banca_M.RenderModel();

		/////Basura
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Basura_M.RenderModel();

		/////Bicicleta
		glm::mat4 modelauxB(1.0);
		model = glm::mat4(1.0);
		posbici = movimiento.movBici(0.0,banderaB);
		model = glm::translate(model, posbici);
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		modelauxB = model;
		model = glm::rotate(model, movimiento.giroBici(), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bicicleta_M.RenderModel();
		glm::mat4 posicionBicicleta(1.0);
		posicionBicicleta = model;

		/////Llanta Delantera
		model = glm::translate(model, glm::vec3(-1.6f, -0.6f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, movimiento.getGiroLlantas() * 100, glm::vec3(-1.0f, 0.0f, 0.0f));
		modelauxB = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		LlantaD_M.RenderModel();
		model = modelauxB = posicionBicicleta;

		/////Llanta Trasera
		model = glm::translate(model, glm::vec3(1.5f, -0.6f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, movimiento.getGiroLlantas() * 100, glm::vec3(-1.0f, 0.0f, 0.0f));
		modelauxB = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		LlantaT_M.RenderModel();
		model = modelauxB = posicionBicicleta;

		/////Bano
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bano_M.RenderModel();

		/////Puerta Bano 01
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(65.0f, 2.2f, -43.7f));
		model = glm::scale(model, glm::vec3(0.65f, 0.65f, 0.65f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		////////*************** Para girar la puerta ¨***************************
		model = glm::rotate(model, -mainWindow.getAbrirPuerta() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PuertaBano_M.RenderModel();


		/////Puerta Bano 02
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(65.0f, 2.2f, -38.8f));
		model = glm::scale(model, glm::vec3(0.65f, 0.65f, 0.65f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		////////*************** Para girar la puerta ¨***************************
		model = glm::rotate(model, -mainWindow.getAbrirPuerta() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PuertaBano_M.RenderModel();

		/////Puerta Bano 03
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-64.67f, 2.2f, 48.4f));
		model = glm::scale(model, glm::vec3(0.65f, 0.65f, 0.65f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		////////*************** Para girar la puerta ¨***************************
		model = glm::rotate(model, -mainWindow.getAbrirPuerta() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PuertaBano_M.RenderModel();


		/////Puerta Bano 04
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-64.67f, 2.2f, 43.5f));
		model = glm::scale(model, glm::vec3(0.65f, 0.65f, 0.65f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		////////*************** Para girar la puerta ¨***************************
		model = glm::rotate(model, -mainWindow.getAbrirPuerta() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PuertaBano_M.RenderModel();


	

		/////Pasto
		/*
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pasto_M.RenderModel();

		
		/////Pavimento
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pavimento_M.RenderModel();

		/////Pavimento Circulo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.45f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PavimentoCirculo_M.RenderModel();

		/////Pavimento Circulito
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.45f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PavimentoCirculito_M.RenderModel();
		*/

		/////Arbusto ** Debe ir al final para menor perdida de transparencia
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbusto_M.RenderModel();

		glDisable(GL_BLEND);
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

void inputKeyframes(bool* keys)
{
	////////Tecla despegue //////////////
	if (keys[GLFW_KEY_K])
	{
		bandera = true;
	}

	if (keys[GLFW_KEY_6])
	{
		banderaB = true;
	}


}
