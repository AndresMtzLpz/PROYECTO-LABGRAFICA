/*
Semestre 2020-2
Animación por keyframes
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
//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"
#include"SpotLight.h"

#include "Movimiento.h"
#include "AnimacionKF.h"

const float toRadians = 3.14159265f / 180.0f;
bool bandera;
/*Variables para movimiento*/
/*float movBlackHawkX;
float movBlackHawkY;
float movBlackHawkZ;


bool escenario = false;
int paso;
float girarBlackHawk;
float giroHelice;*/


//////VARIABLES BATMAN LEGO///////
float posXBatman = 20.0, posYBatman = -1.3f, posZBatman = 0.0;
float	movBatman_x = 0.0f, movBatman_y = 0.0f, movBatman_z = 0.0f;
float giroBatman = 0;
glm::vec3 posBatman = glm::vec3(posXBatman, posYBatman, posZBatman);





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
DirectionalLight mainLight;

//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];




/*Declaración de modelos a usar*/
Model Blackhawk_M;
Model Helices_M;
Model Helicoptero_M;

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


///////MODELO NAVE ANAKIN ////////////////
Model Nave_M;



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
//cálculo del promedio de las normales para sombreado de Phong
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
	CreaPiso();

	/*Inicialización para los objetos de animación*/
	animacion = AnimacionKF();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	/*Carga de texturas*/
	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	Tagave = Texture("Textures/Agave.tga");
	Tagave.LoadTextureA();

	/*Materiales*/
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	/*Craga de modelos*/
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Helices_M = Model();
	Helices_M.LoadModel("Models/heliceprincipal.obj");
	Helicoptero_M = Model();
	Helicoptero_M.LoadModel("Models/Helicoptero.fbx");

	///////BATMAN LEGO /////
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

	////////////////
	
	Nave_M = Model();
	Nave_M.LoadModel("Models/Anakin.obj");


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
								 0.3f, 0.3f, //Intensidad luminosa
								 0.0f, 0.0f, -1.0f);



	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, //Intensidades 
		2.0f, 1.5f, 1.5f, //Posición de la luz
		0.3f, 0.2f, 0.1f);	//coeficientes de una ecuación de segundo grado
						//que no den valores complejos
	pointLightCount++;


	/*Declaración de luces tipo spot*/
	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f, 
		0.0f, 0.0f, 0.0f, //Posición de la luz
		0.0f, -1.0f, 0.0f, //Dirección de la luz
		1.0f, 0.0f, 0.0f,//Atenuación
		20.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		0.0f, 2.0f,
		10.0f, 0.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;
	//luz de faro
	spotLights[2] = SpotLight(0.0f, 1.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, -1.5f, 0.0f,
		-4.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;


	glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);

	///////// ESCENRAIO DIA Y NOCHE /////////////
	////Dia
	std::vector<std::string> skyboxFacesA;
	skyboxFacesA.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFacesA.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFacesA.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFacesA.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFacesA.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFacesA.push_back("Textures/Skybox/cupertin-lake_ft.tga");
	skyboxA = Skybox(skyboxFacesA);

	////Noche
	std::vector<std::string> skyboxFacesB;
	skyboxFacesB.push_back("Textures/Skybox/cupertin-lake-night_rt.tga");
	skyboxFacesB.push_back("Textures/Skybox/cupertin-lake-night_lf.tga");
	skyboxFacesB.push_back("Textures/Skybox/cupertin-lake-night_dn.tga");
	skyboxFacesB.push_back("Textures/Skybox/cupertin-lake-night_up.tga");
	skyboxFacesB.push_back("Textures/Skybox/cupertin-lake-night_bk.tga");
	skyboxFacesB.push_back("Textures/Skybox/cupertin-lake-night_ft.tga");
	skyboxB = Skybox(skyboxFacesB);

	
	


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);

	

	///////// DESPEGUE /////////////
	/*
	movBlackHawkZ = 0.0f;
	movBlackHawkY = 0.0f;
	movBlackHawkX = 0.0f;
	bandera = false;
	paso = 1;
	girarBlackHawk = 0.0f;*/

	movimiento = Movimiento();
	movimiento.time();

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Se Ejecuta una unica vez por cada ciclo
		movimiento.time();
		deltaTime = movimiento.getDeltaTime();
/*
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		//deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//////////*/
		
		
		/*Animación para dia y noche*/

		skybox = movimiento.horaDia() >= 6 && movimiento.horaDia() < 18 ? 
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
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0);

		/*Piso provisinal para probar iluminaciones*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		brickTexture.UseTexture();
		meshList[0]->RenderMesh();



		/*Helicoptero por keyframes*/
		model = glm::mat4(1.0);
		posblackhawk = animacion.movAvion();
		model = glm::translate(model, posblackhawk);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, animacion.rGiroAvion() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Blackhawk_M.RenderModel(); 
		spotLights[3].SetPos(posblackhawk);

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
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
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

		////MuñecaD
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

		////MuñecaI
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
		model = glm::rotate(model, glm::radians(mainWindow.getPieD()), glm::vec3(0.0f, 1.0f, 0.0f));
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

		////////////////////////////////////////////////




				//////HELICES ///////////////////////////
		model = glm::mat4(1.0);
		//posblackhawk = glm::vec3(movBlackHawkX, -1.85f + movBlackHawkY + 0.2f, movBlackHawkZ + movBlackHawkZ + 2.0f);
		posblackhawk = movimiento.movBlackHawk(0.0, bandera) + glm::vec3(0.0, 0.2, 0.0);
		model = glm::translate(model, posblackhawk);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, movimiento.getGiroHelice() * 100, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Helices_M.RenderModel();

		/////HELICOPTERO 02 ////////////////////
		model = glm::mat4(1.0);
		//posblackhawk = glm::vec3(movBlackHawkX, -1.85 + movBlackHawkY, movBlackHawkZ + movBlackHawkZ + 2.0f);
		posblackhawk = movimiento.movBlackHawk(0.0, bandera);
		model = glm::translate(model, posblackhawk);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, movimiento.giroBlackHawk(), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Helicoptero_M.RenderModel();
		
		/* Se deja para recordar lo de traspaencia
		//Agave ¿qué sucede si lo renderizan antes del coche y de la pista?
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



}