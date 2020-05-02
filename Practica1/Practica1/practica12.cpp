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

const float toRadians = 3.14159265f / 180.0f;
float movCoche;
float movOffset;
float rotapato;

float movBlackHawkX;
float movBlackHawkY;
float movBlackHawkZ;
bool bandera;
bool escenario = false;
int paso;
float girarBlackHawk;
float giroHelice;

bool avanza, patoadelante;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;
float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;
Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture dadoTexture;
Texture pisoTexture;
Texture Tagave;
//materiales
Material Material_brillante;
Material Material_opaco;
//luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Model Kitt_M;
Model Llanta_M;
Model Camino_M;
Model Blackhawk_M;
Model Helices_M;
Model Helicoptero_M;

//////VARIABLES BATMAN LEGO///////
float posXBatman = 20.0, posYBatman = -1.3f, posZBatman = 0.0;
float	movBatman_x = 0.0f, movBatman_y = 0.0f, movBatman_z = 0.0f;
float giroBatman = 0;
glm::vec3 posBatman = glm::vec3(posXBatman, posYBatman, posZBatman);


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

/////// VARIABLES NAVE ANAKIN ////////////
float movNaveX;
float movNaveY;
float movNaveZ;
float girarNave;
glm::vec3 posNave = glm::vec3(0.0f, 0.0f, 0.0f);

///////MODELO NAVE ANAKIN ////////////////
Model Nave_M;



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




void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	Mesh *obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.27f,  0.35f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.48f,	0.35f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.48f,	0.64f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.27f,	0.64f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.52f,  0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.73f,	0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.73f,	0.64f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.52f,	0.64f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.77f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.98f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.98f,	0.64f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.77f,	0.64f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,	0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.23f,  0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.23f,	0.64f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	0.64f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.27f,	0.02f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.48f,  0.02f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.48f,	0.31f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.27f,	0.31f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.27f,	0.68f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.48f,  0.68f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.48f,	0.98f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.27f,	0.98f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh *cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(cubo);

}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


///////////////////////////////KEYFRAMES/////////////////////


bool animacion = false;



//NEW// Keyframes
float posXavion = 2.0, posYavion = 2.0, posZavion = 0;
float	movAvion_x = 0.0f, movAvion_y = 0.0f, movAvion_z = 0.0f;
float giroAvion = 0;

#define MAX_FRAMES 30
int i_max_steps = 90;
int i_curr_steps = 29;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movAvion_x;		//Variable para PosicionX
	float movAvion_y;		//Variable para PosicionY
	float movAvion_z;		//Variable para PosicionZ
	float movAvion_xInc;		//Variable para IncrementoX
	float movAvion_yInc;		//Variable para IncrementoY
	float movAvion_zInc;		//Variable para IncrementoZ
	float giroAvion;
	float giroAvionInc;

	
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 29;			//introducir datos
bool play = false;
int playIndex = 0;

bool playDespegue = false;

void saveFrame(void)
{

	printf("frameindex %d: ", FrameIndex);
	


	KeyFrame[FrameIndex].movAvion_x = movAvion_x;
	KeyFrame[FrameIndex].movAvion_y = movAvion_y;
	KeyFrame[FrameIndex].movAvion_z = movAvion_z;
	KeyFrame[FrameIndex].giroAvion;

	FrameIndex++;
}

void resetElements(void)
{

	movAvion_x = KeyFrame[0].movAvion_x;
	movAvion_y = KeyFrame[0].movAvion_y;
	movAvion_z = KeyFrame[0].movAvion_z;
	giroAvion = KeyFrame[0].giroAvion;
}

void interpolation(void)
{
	KeyFrame[playIndex].movAvion_xInc = (KeyFrame[playIndex + 1].movAvion_x - KeyFrame[playIndex].movAvion_x) / i_max_steps;
	KeyFrame[playIndex].movAvion_yInc = (KeyFrame[playIndex + 1].movAvion_y - KeyFrame[playIndex].movAvion_y) / i_max_steps;
	KeyFrame[playIndex].movAvion_zInc = (KeyFrame[playIndex + 1].movAvion_z - KeyFrame[playIndex].movAvion_z) / i_max_steps;
	KeyFrame[playIndex].giroAvionInc = (KeyFrame[playIndex + 1].giroAvion - KeyFrame[playIndex].giroAvion) / i_max_steps;

}


void animate(void)
{
	//Movimiento del objeto
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			printf("movAvion_x=%f ,movAvion_y=%f,,movAvion_z=%f ,giroAvion= %f\n", KeyFrame[playIndex].movAvion_x, KeyFrame[playIndex].movAvion_y, KeyFrame[playIndex].movAvion_z, KeyFrame[playIndex].giroAvion);
			printf("\n");
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				//printf("entro aquí\n");
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//printf("se quedó aqui\n");
			//printf("max steps: %f", i_max_steps);
			//Draw animation
			movAvion_x += KeyFrame[playIndex].movAvion_xInc;
			movAvion_y += KeyFrame[playIndex].movAvion_yInc;
			movAvion_z += KeyFrame[playIndex].movAvion_zInc;
			giroAvion += KeyFrame[playIndex].giroAvionInc;
			i_curr_steps++;
		}

	}

	//Movimiento del Helicoptero Despegue
	if (playDespegue)
	{
		

	}
}

/* FIN KEYFRAMES*/



	
		






int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearCubo();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado.tga");
	dadoTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	Tagave = Texture("Textures/Agave.tga");
	Tagave.LoadTextureA();
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/k_rueda.3ds");
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
	Camino_M = Model();
	Camino_M.LoadModel("Models/railroad track.obj");
	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		2.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
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
	//luz de helicoptero
	spotLights[3] = SpotLight(0.0f, 0.0f, 1.0f,
		0.0f, 1.0f,
		2.0 - movCoche, 2.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
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
	
	////Noche
	std::vector<std::string> skyboxFacesB;
	skyboxFacesB.push_back("Textures/Skybox/cupertin-lake-night_rt.tga");
	skyboxFacesB.push_back("Textures/Skybox/cupertin-lake-night_lf.tga");
	skyboxFacesB.push_back("Textures/Skybox/cupertin-lake-night_dn.tga");
	skyboxFacesB.push_back("Textures/Skybox/cupertin-lake-night_up.tga");
	skyboxFacesB.push_back("Textures/Skybox/cupertin-lake-night_bk.tga");
	skyboxFacesB.push_back("Textures/Skybox/cupertin-lake-night_ft.tga");
	skybox = Skybox(skyboxFacesB);



	///////
	
	


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);

	movCoche = 0.0f;
	movOffset = 1.0f;
	avanza = 1;

	//KEYFRAMES DECLARADOS INICIALES

	KeyFrame[0].movAvion_x = 0.0f;
	KeyFrame[0].movAvion_y = 0.0f;
	KeyFrame[0].movAvion_z = 0.0f;
	KeyFrame[0].giroAvion = 0;


	KeyFrame[1].movAvion_x = 1.0f;
	KeyFrame[1].movAvion_y = 2.0f;
	KeyFrame[1].movAvion_z = 0.0f;
	KeyFrame[1].giroAvion = 0;


	KeyFrame[2].movAvion_x = 2.0f;
	KeyFrame[2].movAvion_y = 0.0f;
	KeyFrame[2].movAvion_z = 0.0f;
	KeyFrame[2].giroAvion = 0;


	KeyFrame[3].movAvion_x = 3.0f;
	KeyFrame[3].movAvion_y = -2.0f;
	KeyFrame[3].movAvion_z = 0.0f;
	KeyFrame[3].giroAvion = 0;

	KeyFrame[4].movAvion_x = 4.0f;
	KeyFrame[4].movAvion_y = 0.0f;
	KeyFrame[4].movAvion_z = 0.0f;
	KeyFrame[4].giroAvion = 0.0f;

	KeyFrame[5].movAvion_x = 5.0f;
	KeyFrame[5].movAvion_y = 2.0f;
	KeyFrame[5].movAvion_z = 0.0f;
	KeyFrame[5].giroAvion = 0;

	KeyFrame[6].movAvion_x = 6.0f;
	KeyFrame[6].movAvion_y = 0.0f;
	KeyFrame[6].movAvion_z = 0.0f;
	KeyFrame[6].giroAvion = 0;

	KeyFrame[7].movAvion_x = 7.0f;
	KeyFrame[7].movAvion_y = -2.0f;
	KeyFrame[7].movAvion_z = 0.0f;
	KeyFrame[7].giroAvion = 0;

	KeyFrame[8].movAvion_x = 8.0f;
	KeyFrame[8].movAvion_y = 0.0f;
	KeyFrame[8].movAvion_z = 0.0f;
	KeyFrame[8].giroAvion = 0;

	KeyFrame[9].movAvion_x = 9.0f;
	KeyFrame[9].movAvion_y = 2.0f;
	KeyFrame[9].movAvion_z = 0.0f;
	KeyFrame[9].giroAvion = 0.0f;

	KeyFrame[10].movAvion_x = 10.0f;
	KeyFrame[10].movAvion_y = 0.0f;
	KeyFrame[10].movAvion_z = 0.0f;
	KeyFrame[10].giroAvion = 0;

	KeyFrame[11].movAvion_x = 10.0f;
	KeyFrame[11].movAvion_y = 0.0f;
	KeyFrame[11].movAvion_z = -1.0f;
	KeyFrame[11].giroAvion = 45;

	KeyFrame[12].movAvion_x = 10.0f;
	KeyFrame[12].movAvion_y = 0.0f;
	KeyFrame[12].movAvion_z = -2.0f;
	KeyFrame[12].giroAvion = 90;

	KeyFrame[13].movAvion_x = 10.0f;
	KeyFrame[13].movAvion_y = 0.0f;
	KeyFrame[13].movAvion_z = -3.0f;
	KeyFrame[13].giroAvion = 135;

	KeyFrame[14].movAvion_x = 10.0f;
	KeyFrame[14].movAvion_y = 0.0f;
	KeyFrame[14].movAvion_z = -4.0f;
	KeyFrame[14].giroAvion = 180.0;

	KeyFrame[15].movAvion_x = 9.0f;
	KeyFrame[15].movAvion_y = 2.0f;
	KeyFrame[15].movAvion_z = -4.0f;
	KeyFrame[15].giroAvion = 180.0;

	KeyFrame[16].movAvion_x = 8.0f;
	KeyFrame[16].movAvion_y = 0.0f;
	KeyFrame[16].movAvion_z = -4.0f;
	KeyFrame[16].giroAvion = 180;

	KeyFrame[17].movAvion_x = 7.0f;
	KeyFrame[17].movAvion_y = -2.0f;
	KeyFrame[17].movAvion_z = -4.0f;
	KeyFrame[17].giroAvion = 180;

	KeyFrame[18].movAvion_x = 6.0f;
	KeyFrame[18].movAvion_y = 0.0f;
	KeyFrame[18].movAvion_z = -4.0f;
	KeyFrame[18].giroAvion = 180;

	KeyFrame[19].movAvion_x = 5.0f;
	KeyFrame[19].movAvion_y = 2.0f;
	KeyFrame[19].movAvion_z = -4.0f;
	KeyFrame[19].giroAvion = 180.0f;

	KeyFrame[20].movAvion_x = 4.0f;
	KeyFrame[20].movAvion_y = 0.0f;
	KeyFrame[20].movAvion_z = -4.0f;
	KeyFrame[20].giroAvion = 180;

	KeyFrame[21].movAvion_x = 3.0f;
	KeyFrame[21].movAvion_y = -2.0f;
	KeyFrame[21].movAvion_z = -4.0f;
	KeyFrame[21].giroAvion = 180;

	KeyFrame[22].movAvion_x = 2.0f;
	KeyFrame[22].movAvion_y = 0.0f;
	KeyFrame[22].movAvion_z = -4.0f;
	KeyFrame[22].giroAvion = 180.0;

	KeyFrame[23].movAvion_x = 1.0f;
	KeyFrame[23].movAvion_y = 2.0f;
	KeyFrame[23].movAvion_z = -4.0f;
	KeyFrame[23].giroAvion = 180.0;

	KeyFrame[24].movAvion_x = 0.0f;
	KeyFrame[24].movAvion_y = 0.0f;
	KeyFrame[24].movAvion_z = -4.0f;
	KeyFrame[24].giroAvion = 180;

	KeyFrame[25].movAvion_x = 0.0f;
	KeyFrame[25].movAvion_y = 0.0f;
	KeyFrame[25].movAvion_z = -3.0f;
	KeyFrame[25].giroAvion = 225;

	KeyFrame[26].movAvion_x = 0.0f;
	KeyFrame[26].movAvion_y = 0.0f;
	KeyFrame[26].movAvion_z = -2.0f;
	KeyFrame[26].giroAvion = 270;

	KeyFrame[27].movAvion_x = 0.0f;
	KeyFrame[27].movAvion_y = 0.0f;
	KeyFrame[27].movAvion_z = -1.0f;
	KeyFrame[27].giroAvion = 315.0f;

	KeyFrame[28].movAvion_x = 0.0f;
	KeyFrame[28].movAvion_y = 0.0f;
	KeyFrame[28].movAvion_z = 0.0f;
	KeyFrame[28].giroAvion = 360.0f;
	//Agregar Kefyrame[5] para que el avión regrese al inicio

	///////// DESPEGUE /////////////
	movCoche = 0.0f;
	movNaveZ = 0.0f;
	movNaveY = 0.0f;
	movNaveX = 0.0f;
	bandera = false;
	paso = 1;
	girarNave = 0.0f;


	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		//deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//////////
		if (bandera) {
			giroHelice += 15.0f * deltaTime;
			

			if (giroHelice == 200.0f || movNaveY < 4.5f) {
				movNaveY += 0.6f * deltaTime;
			}
			else {

				switch (paso)
				{
				case 1:
					if (movNaveZ < 5 ) {
						movNaveZ += sin(0.3) * 2.5 * deltaTime;
						movNaveY += sin(giroHelice) * deltaTime;
					}
					else if (movNaveZ >= 5 && girarNave < 90) {
						girarNave += 1.0 ;
					}
					else
						paso += 1;
					break;

				case 2:
					if (movNaveX > -5) {
						movNaveX -= sin(0.7) * 2.5 * deltaTime;
						movNaveY += sin(giroHelice) * deltaTime;
					}
					else if (movNaveX <= -5 && girarNave < 180) {
						girarNave += 1.0 ;
					}
					else {
						///////CAMBIA A DIA EL ESCENARIO ///////
						paso += 1;
						skybox = Skybox(skyboxFacesA);
						
					}
						
					break;
				case 3:
					if (movNaveZ > -5) {
						movNaveZ -= sin(0.4) * 2.5* deltaTime;
						movNaveY += sin(giroHelice) * deltaTime;
					}
					else if (movNaveZ <= 5 && girarNave < 270) {
						girarNave += 1.0;
					}
					else
						paso += 1;
					break;
				case 4:
					if (movNaveX < 5) {
						movNaveX += sin(0.3) * 2.5 * deltaTime;
						movNaveY += sin(giroHelice) * deltaTime;
					}
					else if (movNaveX >= 5 && girarNave < 360) {
						girarNave += 1.0 ;
					}
					else
						paso += 1;
					break;
				default:
					/////SE INICIALIZAN VALORES //// VUELVE A NOCHE /////
					paso = 1;
					girarNave = 0;
					skybox = Skybox(skyboxFacesB);
					
					
				}


				
				
			}
			
		}
		

		
	



		////////////
		if (avanza)
		{
			if (movCoche < 10.0f)
			{
				//				movCoche += movOffset*deltaTime;
				movCoche += movOffset;
				avanza = 1;
			}
			else
			{
				avanza = 0;
			}
		}
		else
		{
			if (movCoche > -10.0f)
			{
				movCoche -= movOffset * deltaTime;
			}
			else
			{
				avanza = 1;
			}


		}


		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		//para keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();
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


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//pisoTexture.UseTexture();
		plainTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche, -1.4f, 0.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kitt_M.RenderModel();
		spotLights[2].SetPos(glm::vec3(movCoche, -1.5f, 0.0f));

		model = glm::mat4(1.0);
		posblackhawk = glm::vec3(posXavion + movAvion_x, posYavion + movAvion_y, posZavion + movAvion_z);
		model = glm::translate(model, posblackhawk);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, giroAvion * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Blackhawk_M.RenderModel(); 
		spotLights[3].SetPos(posblackhawk);

		////////////BATMAN LEGO DIBUJO//////////////////


		////Cadera
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
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BatmanCadera_M.RenderModel();
		//model = modelaux;
		glm::mat4 posicionCadera(1.0);
		posicionCadera = model;

		////Cinturon
		//model = glm::mat4(1.0);
		posBatman = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posBatman);
		modelaux = model;
		//model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BatmanCinturon_M.RenderModel();
		model = modelaux = posicionCadera;

		////Torax
		//model = glm::mat4(1.0);
		//glm::mat4 modelaux(1.0);
		posBatman = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posBatman);
		//model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BatmanTorax_M.RenderModel();
		//model = modelaux = posicionCadera;
		glm::mat4 posicionTorax(1.0);
		posicionTorax = model;

		////HombroD
		//model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 15.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getHombroD()), glm::vec3(-1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//meshList[0]->RenderMesh();


		////BrazoD
		//model = glm::mat4(1.0);
		//modelaux = model;
		posBatman = glm::vec3(0.0f, - 15.0f, 0.0f);
		model = glm::translate(model, posBatman);
		modelaux = model;
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

		//modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BatmanBrazoD_M.RenderModel();

		////MuñecaD
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));

		////ManoD
		//model = glm::mat4(1.0);
		posBatman = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posBatman);
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BatmanManoD_M.RenderModel();
		model = modelaux = posicionTorax;

		////HombroI
		//model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f,  15.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getHombroI()), glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//meshList[0]->RenderMesh();

		////BrazoI
		//model = glm::mat4(1.0);
		posBatman = glm::vec3(0.0f, - 15.0f, 0.0f);
		model = glm::translate(model, posBatman);
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		modelaux = model;
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BatmanBrazoI_M.RenderModel();

		////MuñecaI
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));

		////ManoI
		//model = glm::mat4(1.0);
		posBatman = glm::vec3(0.0f, 0.0f , 0.0f);
		model = glm::translate(model, posBatman);
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BatmanManoI_M.RenderModel();
		model = modelaux = posicionTorax;


		////Cuello
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getPieD()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();
		//model = modelaux = posicionCadera;
		//model = modelaux = posicionCadera;
		glm::mat4 posicionCuello(1.0);
		posicionCuello = model;

		////Cabeza
		posBatman = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posBatman);
		//model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BatmanCabeza_M.RenderModel();
		model = modelaux = posicionCuello;

		////Mascara
		posBatman = glm::vec3(0.0f,0.0f, 0.0f);
		model = glm::translate(model, posBatman);
		//model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BatmanMascara_M.RenderModel();
		model = modelaux = posicionCuello;

		////CentroPieD
		modelaux = posicionCadera;
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getPieD()), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		modelaux = model;
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//meshList[0]->RenderMesh();
		glm::mat4 posicionPieD(1.0);
		posicionPieD = model;
	
		////PieD
		//model = glm::mat4(1.0);
		//model = modelaux;
		posBatman = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, posBatman);
		//model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
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
		//model = glm::mat4(1.0);
		model = modelaux;
		posBatman = glm::vec3(0.0f,0.0f, 0.0f);
		model = glm::translate(model, posBatman);
		//model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BatmanPieI_M.RenderModel();
		model = modelaux = posicionTorax;

		
	
		////Capa
		posBatman = glm::vec3(0.0, 0.0, 0.0);
		model = glm::translate(model, posBatman);
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BatmanCapa_M.RenderModel();
		model = modelaux = posicionTorax;

		////////////////////////////////////////////////


		/////NAVE ANAKIN ////////////////////
		model = glm::mat4(1.0);
		posNave = glm::vec3(movNaveX , movNaveY - 1.5, movNaveZ + 2.0f);
		model = glm::translate(model, posNave);
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, girarNave  * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Nave_M.RenderModel();


		/*
		model = glm::mat4(1.0);
		posblackhawk = glm::vec3(posXavion + movAvion_x, posYavion + movAvion_y, posZavion);
		model = glm::translate(model, posblackhawk);
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, giroAvion * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Nave_M.RenderModel();
		*/

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.53f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Camino_M.RenderModel();

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
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("presiona 0 para habilitar reproducir de nuevo la animación'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;
			}
		}
	}

	////////Tecla despegue //////////////
	if (keys[GLFW_KEY_K])
	{
		if (reproduciranimacion < 1)
		{
			if (playDespegue == false )
			{
				resetElements();
				bandera = true;		

			}
			else
			{
				bandera = false;
			}
		}
	}

	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1)
		{
			reproduciranimacion = 0;
		}
	}

	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			printf("movAvion_x es: %f\n", movAvion_x);
			//printf("movAvion_y es: %f\n", movAvion_y);
			printf("presiona P para habilitar guardar otro frame'\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}
	if (keys[GLFW_KEY_P])
	{
		if (reinicioFrame < 1)
		{
			guardoFrame = 0;
		}
	}


	if (keys[GLFW_KEY_1])
	{
		if (ciclo < 1)
		{
			//printf("movAvion_x es: %f\n", movAvion_x);
			movAvion_x += 1.0f;
			printf("movAvion_x es: %f\n", movAvion_x);
			ciclo++;
			ciclo2 = 0;
			printf("reinicia con 2\n");
		}

	}
	if (keys[GLFW_KEY_2])
	{
		if (ciclo2 < 1)
		{
			ciclo = 0;
		}
	}

}