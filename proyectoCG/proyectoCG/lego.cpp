/*--------------------- PROYECTO: ESCENARIO LEGO ------------------------------------*/
/*--------------------Integrantes: ----------------------------------------*/
/*------------------ Amezaga Campos Salvador ---------------------------------*/
/*------------------ Colohua Carvajal Daniela ---------------------------------*/

//#define STB_IMAGE_IMPLEMENTATION
#include <glew.h>
#include <glfw3.h>
#include <stb_image.h>

#include "camera.h"
#include "Model.h"
#include "Texture.h"
#include <windows.h>
#include <iostream>
#pragma comment(lib,"winmm.lib")

// Other Libs
#include "SOIL2/SOIL2.h"

void resize(GLFWwindow* window, int width, int height);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
// Window size
int SCR_WIDTH = 380000;
int SCR_HEIGHT = 760000;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;
GLuint skyboxVBO, skyboxVAO;

//Camera
Camera camera(glm::vec3(0.0f, 10.0f, 0.0f));
double	lastX = 0.0f,
lastY = 0.0f;																						//Aqui esta la camara
bool firstMouse = true;

//Timing
double	deltaTime = 0.0f,
lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, 3.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

void myData(void);
void display(Shader, Model, Model);
void getResolution(void);
void animate(void);
void LoadTextures(void);
unsigned int generateTextures(char*, bool);

//Texture
/* VARIABLES PARA EL MOVIMIENTO DEL AVION */

float	movZ = -50.0f,
movY = 0.0f,
movX = -10.0f,
orienta = 0.0,
luz = 0.0032,
brazoi = 0.0f,
brazod = 0.0f,
cabeza = 0.0f,
pied = 0.0f,
piei = 0.0f,
movXt = 0.0f,
movZt = 0.0f,
movYt = -1.75f,
movXbi = 0.0f,
movZbi = 0.0f,
movYbi = -1.75f,
movXpi = 0.0f,
movZpi = 0.0f,
movYpi = -1.75f,
movXpd = 0.0f,
movZpd = 0.0f,
movYpd = -1.75f,
movXbd = 0.0f,
movZbd = 0.0f,
movYbd = -1.75f,
movXc = 0.0f,
movZc = 0.0f,
movYc = -1.75f;

/* VARIABLES DE CONTROL */

bool	recorrido1 = false,			// MOVIMIENTO HORIZONTAL
sube = true,			// MOVIMIENTO DIAGONAL
recorrido2 = false,				// 
recorrido3 = false,				// 
recorrido4 = false,				// 
animacion = false,
recorrido5 = false,
recorrido6 = false,
recorrido7 = false,
animacion2 = false,
animacion3 = false,
recorridop1 = true,
recorridop2 = false,
recorridop3 = false,
recorridop4 = false,
recorridop5 = false,
recorridop6 = false,
recorridop7 = false,
recorridop8 = false;

unsigned int generateTextures(const char* filename, bool alfa)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 100;
	}

	stbi_image_free(data);
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;

	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;

}

void LoadTextures()
{

}

void myData()
{
	float vertices[] = {
		// positions            // texture coords
		-0.5f, -0.5f, -0.5f,  0.9f,  0.0f,  0.0f, //TRASERA
		 0.5f, -0.5f, -0.5f,  1.0f,  0.25f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.9f,  0.25f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.9f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f, //FRONTAL
		 0.5f, -0.5f,  0.5f,   0.5f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,   0.5f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,  0.9f,  0.0f,  0.0f, //IZQUIERDA
		-0.5f,  0.5f, -0.5f, 1.0f,  0.25f,  0.0f,
		-0.5f, -0.5f, -0.5f, 0.9f,  0.25f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.9f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  0.9f,  0.0f,  0.0f, //DERECHA
		 0.5f,  0.5f, -0.5f,  1.0f,  0.25f,  0.0f,
		 0.5f, -0.5f, -0.5f, 0.9f,  0.25f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.9f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,   0.9f,  0.0f,  0.0f, //INFERIOR
		 0.5f, -0.5f, -0.5f,  1.0f,  0.25f,  0.0f,
		 0.5f, -0.5f,  0.5f, 0.9f,  0.25f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.9f,  0.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.9f,  0.0f,  0.0f, //SUPERIOR
		 0.5f,  0.5f, -0.5f,  1.0f,  0.25f,  0.0f,
		 0.5f,  0.5f,  0.5f, 0.9f,  0.25f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,

	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}

void myData2()
{
	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);

	glBindVertexArray(skyboxVAO);

	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	// SkyBox
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

}

/**/
void animate(void)
{
	if (animacion) {
		if (sube) {
			movY += 0.5f;
			movZ += 0.3f;
			orienta = 0.0f;
			if (movY >= 50.0f) {
				sube = false;
				recorrido1 = true;
			}
		}
		if (recorrido1) {
			movX -= 0.3f;
			orienta = -90.0f;
			if (movX <= -15.0f) {
				recorrido1 = false;
				recorrido2 = true;
			}
		}
		if (recorrido2) {
			movZ -= 0.4f;
			orienta = -180.0f;
			printf("Posicion en z: %f \n", movZ);
			if (movZ <= -90.0f) {
				recorrido2 = false;
				recorrido3 = true;
			}
		}

		if (recorrido3) {
			movX += 0.5f;
			orienta = -90.0f;
			if (movX >= -20.0f) {
				recorrido3 = false;
				recorrido4 = true;
			}
		}
		if (recorrido4) {
			movY -= 0.5f;
			movZ -= 0.3f;
			orienta = 180.0f;
			if (movY <= 0.0f) {
				recorrido4 = false;
				recorrido5 = true;
			}
		}
		if (recorrido5) {
			animacion = false;
			sube = true;
		}

	}

	//Animacion luz
	if (animacion2) {
		luz += 0.001;
		if (luz >= 0.0102) {
			animacion2 = false;
		}
	}

	//ANIMACION PERSONAJE
	if (animacion3) {
		if (recorridop1) {
			movXt -= 0.2;
			if (movXt <= -5.0f) {
				movZt += 0.2f;
				if (movZt >= 20.0f) {
					recorridop1 = false;
					recorridop2 = true;
				}
			}

		}
		if (recorridop2) {
			movXbi -= 0.2;
			if (movXbi <= -5.0f) {
				movZbi += 0.2f;
				if (movZbi >= 20.0f) {
					recorridop2 = false;
					recorridop3 = true;
				}
			}

		}
		if (recorridop3) {
			movXbd -= 0.2;
			if (movXbd <= -5.0f) {
				movZbd += 0.2f;
				if (movZbd >= 20.0f) {
					recorridop3 = false;
					recorridop4 = true;
				}
			}

		}
		if (recorridop4) {
			movXpi -= 0.2;
			if (movXpi <= -5.0f) {
				movZpi += 0.2f;
				if (movZpi >= 20.0f) {
					recorridop4 = false;
					recorridop5 = true;
				}
			}

		}

		if (recorridop5) {
			movXpd -= 0.2;
			if (movXpd <= -5.0f) {
				movZpd += 0.2f;
				if (movZpd >= 20.0f) {
					recorridop5 = false;
					recorridop6 = true;
				}
			}

		}
		if (recorridop6) {
			movXc -= 0.2;
			if (movXc <= -5.0f) {
				movZc += 0.2f;
				if (movZc >= 20.0f) {
					recorridop6 = false;
					animacion = false;
				}
			}

		}

	}
}


void display(Shader shader, Shader skyboxShader, Shader primitivasShader, GLuint skybox, Model pirata, Model CamionetaSD,
	Model PlaneSD, Model CastilloSD, Model CamionLego, Model cuboG, Model cuboB, Model cuboC, Model Pizzeria, Model faro,
	Model Casita, Model Carro, Model Casita2, Model Casita3, Model Estudio, Model Pandilla
	, Model Zombies, Model Dudes/*, Model Bandido*/ ,Model brazoder, Model brazoizq, Model cabeza, Model torzo, Model piernaizq, Model piernader)
{
	shader.use();

	//Setup Advanced Lights

	/* FUENTE DE LUZ DIRECCIONAL */

	shader.setVec3("viewPos", camera.Position);
	shader.setVec3("dirLight.direction", lightDirection);
	shader.setVec3("dirLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	shader.setVec3("pointLight[0].position", lightPosition);
	shader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setVec3("pointLight[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("pointLight[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setFloat("pointLight[0].constant", 1.0f);						/* DISTANCIA DE LOS RAYOS LUMINOSOS */
	shader.setFloat("pointLight[0].linear", 0.009f);						/*  */
	shader.setFloat("pointLight[0].quadratic", 0.00032f);					/* PARA MAYOR ILUMINACION (POTENCIA)*/

	shader.setVec3("pointLight[1].position", glm::vec3(0.0, 0.0f, 0.0f));
	shader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setFloat("pointLight[1].constant", 1.0f);
	shader.setFloat("pointLight[1].linear", 0.009f);
	shader.setFloat("pointLight[1].quadratic", 0.032f);

	shader.setFloat("material_shininess", 32.0f);

	// create transformations and Projection
	glm::mat4 tmp = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shader.setMat4("projection", projection);

	/* DIBUJAMOS EL PISO */

	/* EL MODELO DEL PISO ES UN CUBO (MEDIDAS DEL OBJ (1,1,0.025)[m])*/

	float i = 0.0f,
		j = 0.0f;
	//CARRETERA

	for (i = 0; i <= 15.5; i = i + 1.55f)  //PARTE VERTICAL GRIS 1
	{
		for (j = 0; j < 62; j = j + 1.55f)
		{
			model = glm::translate(glm::mat4(1.0f), glm::vec3(i, -1.5, j));
			//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
			shader.setMat4("model", model);
			cuboG.Draw(shader);
		}
	}

	for (i = 0.0f; i >= -31.0f; i = i - 1.55f)  //PARTE HORIZONTAL GRIS 1
	{
		for (j = -1.55f; j >= -17.05f; j = j - 1.55f)
		{
			model = glm::translate(glm::mat4(1.0f), glm::vec3(i, -1.5f, j));
			//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			shader.setMat4("model", model);
			cuboG.Draw(shader);
		}
	}

	for (i = 0.0f; i >= -31.0f; i = i - 1.55f)  //PARTE HORIZONTAL BLANCO 1
	{
		for (j = -17.05f; j >= -18.6f; j = j - 1.55f)
		{
			model = glm::translate(glm::mat4(1.0f), glm::vec3(i, -1.5f, j));
			//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			shader.setMat4("model", model);
			cuboB.Draw(shader);
		}
	}

	//PARTE HORIZONTAL GRIS 2
	for (i = 0.0f; i >= -31.0f; i = i - 1.55f)
	{
		model = glm::translate(glm::mat4(1.0f), glm::vec3(i, -1.5f, -20.15));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		shader.setMat4("model", model);
		cuboG.Draw(shader);
	}

	for (i = 0.0f; i >= -31.0f; i = i - 1.55f)  //PARTE HORIZONTAL BLANCO 2
	{
		for (j = -21.7f; j >= -23.25f; j = j - 1.55f)
		{
			model = glm::translate(glm::mat4(1.0f), glm::vec3(i, -1.5f, j));
			//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			shader.setMat4("model", model);
			cuboB.Draw(shader);
		}
	}

	for (i = 0.0f; i >= -31.0f; i = i - 1.55f)  //PARTE HORIZONTAL GRIS 2
	{
		for (j = -24.8f; j >= -38.75f; j = j - 1.55f)
		{
			model = glm::translate(glm::mat4(1.0f), glm::vec3(i, -1.5f, j));
			//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			shader.setMat4("model", model);
			cuboG.Draw(shader);
		}
	}

	//ESQUINA 1 DE CARRETERA

	for (i = 1.55f; i <= 38.75f; i = i + 1.55f)
	{
		for (j = -1.55f; j >= -38.75f; j = j - 1.55f)
		{
			model = glm::translate(glm::mat4(1.0f), glm::vec3(i, -1.5f, j));
			//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			shader.setMat4("model", model);
			cuboG.Draw(shader);
		}
	}

	//ESQUINA 2 DE CARRETERA

	for (i = -32.55f; i >= -69.75f; i = i - 1.55f)
	{
		for (j = -1.55f; j >= -38.75f; j = j - 1.55f)
		{
			model = glm::translate(glm::mat4(1.0f), glm::vec3(i, -1.5f, j));
			//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			shader.setMat4("model", model);
			cuboG.Draw(shader);
		}
	}

	//CARRETERA 2
	for (i = -32.55f; i >= -46.5f; i = i - 1.55f)  //PARTE VERTICAL GRIS 1
	{
		for (j = 0; j < 62; j = j + 1.55f)
		{
			model = glm::translate(glm::mat4(1.0f), glm::vec3(i, -1.5, j));
			//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
			shader.setMat4("model", model);
			cuboG.Draw(shader);
		}
	}

	for (i = -48.05f; i >= -49.6f; i = i - 1.55f) //PARTE VERTICAL BLANCO 1
	{
		for (j = 0; j < 62; j = j + 1.55f)
		{
			model = glm::translate(glm::mat4(1.0f), glm::vec3(i, -1.5f, j));
			//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			shader.setMat4("model", model);
			cuboB.Draw(shader);
		}
	}

	for (j = 0; j < 62; j = j + 1.55f) //PARTE GRIS VERTICAL DESPU�S DE BLANCO
	{
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-51.15, -1.5f, j));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		shader.setMat4("model", model);
		cuboG.Draw(shader);
	}

	for (i = -52.7f; i >= -54.25f; i = i - 1.55f) //PARTE VERTICAL BLANCO 2
	{
		for (j = 0; j < 62; j = j + 1.55f)
		{
			model = glm::translate(glm::mat4(1.0f), glm::vec3(i, -1.5f, j));
			//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			shader.setMat4("model", model);
			cuboB.Draw(shader);
		}
	}

	for (i = -55.8f; i >= -71.3f; i = i - 1.55f) //PARTE VERTICAL GRIS 2
	{
		for (j = 0; j < 62; j = j + 1.55f)
		{
			model = glm::translate(glm::mat4(1.0f), glm::vec3(i, -1.5f, j));
			//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			shader.setMat4("model", model);
			cuboG.Draw(shader);
		}
	}

	for (i = -31.0f; i <= -1.55f; i = i + 1.55f) //CENTRO PARTE CAFE
	{
		for (j = 0; j < 62; j = j + 1.55f)
		{
			model = glm::translate(glm::mat4(1.0f), glm::vec3(i, -1.5f, j));
			//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			shader.setMat4("model", model);
			cuboC.Draw(shader);
		}
	}

	//CARRETERA 1

	for (i = 15.5f; i <= 17.05f; i = i + 1.55f) //PARTE VERTICAL BLANCO 1
	{
		for (j = 0; j < 62; j = j + 1.55f)
		{
			model = glm::translate(glm::mat4(1.0f), glm::vec3(i, -1.5f, j));
			//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			shader.setMat4("model", model);
			cuboB.Draw(shader);
		}
	}

	for (j = 0; j < 62; j = j + 1.55f) //PARTE GRIS VERTICAL DESPU�S DE BLANCO
	{
		model = glm::translate(glm::mat4(1.0f), glm::vec3(18.6, -1.5f, j));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		shader.setMat4("model", model);
		cuboG.Draw(shader);
	}


	for (i = 20.15f; i <= 21.7f; i = i + 1.55f) //PARTE VERTICAL BLANCO 2
	{
		for (j = 0; j < 62; j = j + 1.55f)
		{
			model = glm::translate(glm::mat4(1.0f), glm::vec3(i, -1.5f, j));
			//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			shader.setMat4("model", model);
			cuboB.Draw(shader);
		}
	}

	for (i = 23.25f; i <= 38.75f; i = i + 1.55f) //PARTE VERTICAL GRIS 2
	{
		for (j = 0; j < 62; j = j + 1.55f)
		{
			model = glm::translate(glm::mat4(1.0f), glm::vec3(i, -1.5f, j));
			//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			shader.setMat4("model", model);
			cuboG.Draw(shader);
		}
	}

	for (i = 40.3f; i <= 52.7f; i = i + 1.55f) //PARTE VERTICAL CAFE
	{
		for (j = 0; j < 62; j = j + 1.55f)
		{
			model = glm::translate(glm::mat4(1.0f), glm::vec3(i, -1.5f, j));
			//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			shader.setMat4("model", model);
			cuboC.Draw(shader);
		}
	}

	//CARRETERA ARCO
	for (i = 0.0f; i <= 77.5f; i = i + 1.55f) //PARTE HORIZONTAL GRIS ARCO
	{
		for (j = 63.55f; j <= 79.05f; j = j + 1.55f)
		{
			model = glm::translate(glm::mat4(1.0f), glm::vec3(i, -1.5f, j));
			//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			shader.setMat4("model", model);
			cuboG.Draw(shader);
		}
	}

	for (i = 0.0f; i >= -71.3f; i = i - 1.55f) //PARTE HORIZONTAL GRIS ARCO
	{
		for (j = 63.55f; j <= 79.05f; j = j + 1.55f)
		{
			model = glm::translate(glm::mat4(1.0f), glm::vec3(i, -1.5f, j));
			//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			shader.setMat4("model", model);
			cuboG.Draw(shader);
		}
	}

	for (i = 54.25f; i <= 100.76f; i = i + 1.55f) //Estacionamiento
	{
		for (j = 0.0f; j <= 79.05f; j = j + 1.55f)
		{
			model = glm::translate(glm::mat4(1.0f), glm::vec3(i, -1.5f, j));
			//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			shader.setMat4("model", model);
			cuboC.Draw(shader);
		}
	}

	for (i = -71.3f; i >= -119.35f; i = i - 1.55f) //PISO CASTILLO
	{
		for (j = 79.05f; j >= -38.75f; j = j - 1.55f)
		{
			model = glm::translate(glm::mat4(1.0f), glm::vec3(i, -1.5f, j));
			//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			shader.setMat4("model", model);
			cuboC.Draw(shader);
		}
	}


	
	//PIRATA

	model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(35.0f, -1.0f, 21.0f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	shader.setMat4("model", model);
	pirata.Draw(shader);

	//CAMIONETA DEL MISTERIO
	model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(3.0f, -1.0f, 5.0f));
	//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
	//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	shader.setMat4("model", model);
	CamionetaSD.Draw(shader);


	//AVION SD

	model = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(movX, movY, movZ));
	model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
	//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	shader.setMat4("model", model);
	PlaneSD.Draw(shader);


	//Castillo SD

	model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(glm::mat4(1.0f), glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(30.0f, -4.0f, -70.0f));
	//tmp = model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

	model = glm::scale(model, glm::vec3(1.4f, 1.4f, 1.4f));
	//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	shader.setMat4("model", model);
	CastilloSD.Draw(shader);

	

	//FARO
	model = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-10.0f, -6.0f, 70.0f));
	model = glm::scale(model, glm::vec3(0.45f, 0.45f, 0.45f));
	//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	shader.setMat4("model", model);
	faro.Draw(shader);

	
	//Camion Lego

	//model = glm::rotate(glm::mat4(1.0f), glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//model = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(-20.0f, .0f, -25.0f));


	model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
	//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	shader.setMat4("model", model);
	CamionLego.Draw(shader);

	//TRAILER
	/*
	//model = glm::rotate(glm::mat4(1.0f), glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(30.0f, 0.0f, 10.0f));
	//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	shader.setMat4("model", model);
	trailer.Draw(shader);
	*/
	
	

	//Pizzeria 

	model = glm::rotate(glm::mat4(1.0f), glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//tmp = model = glm::translate(model, glm::vec3(30.0f, 20.0f, 45.0f));
	tmp = model = glm::translate(model, glm::vec3(20.0f, -2.0f, 25.0f));
	model = glm::scale(model, glm::vec3(1.19f, 1.19f, 1.19f));
	//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	shader.setMat4("model", model);
	Pizzeria.Draw(shader);
	
	//CASITA
	model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(15.0f, 6.0f, -52.0f));
	model = glm::scale(model, glm::vec3(30.5f, 30.5f, 30.5f));
	shader.setMat4("model", model);
	Casita.Draw(shader);


	//Carrito Nuevo
	model = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(43.0f, 0.0f, -45.0f));
	model = glm::scale(model, glm::vec3(93.5f, 93.5f, 93.5f));
	shader.setMat4("model", model);
	Carro.Draw(shader);

	/*
	//Casita 2
	model = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(15.0f, 0.0f, -75.0f));
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	shader.setMat4("model", model);
	Casita2.Draw(shader);*/
	
	//Casita 2
	model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(0.0f, -1.0f, -70.0f));
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	shader.setMat4("model", model);
	Casita2.Draw(shader);

	//Casita 3
	model = glm::rotate(glm::mat4(1.0f), glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(30.0f, 0.0f, 80.0f));
	model = glm::scale(model, glm::vec3(2.3f, 2.3f, 2.3f));
	shader.setMat4("model", model);
	Casita3.Draw(shader);


	//Estudio
	model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(-60.0f, -2.0f, -15.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	shader.setMat4("model", model);
	Estudio.Draw(shader);
	
	

	//Pandilla de SD Con Alopecia
	model = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(20.0f, -1.0f, -60.0f));
	model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
	shader.setMat4("model", model);
	Pandilla.Draw(shader);
	

	//Zombies
	model = glm::rotate(glm::mat4(1.0f), glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(20.0f, -1.0f, -30.0f));
	model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
	shader.setMat4("model", model);
	Zombies.Draw(shader);

	//Dudes Marineros
	model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(20.0f, -1.0f, -40.0f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	shader.setMat4("model", model);
	Dudes.Draw(shader);

	//PERSONAJE DESARMADO

	//TORSO
	model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(movXt, movYt, movZt));
	model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
	shader.setMat4("model", model);
	torzo.Draw(shader);

	//BRAZO DERECHO
	model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 5.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(movXbd, movYbd, movZbd));
	model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
	shader.setMat4("model", model);
	brazoder.Draw(shader);

	//BRAZO IZQUIERDO
	model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(movXbi, movYbi, movZbi));
	model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
	shader.setMat4("model", model);
	brazoizq.Draw(shader);

	//PIE IZQUIERDO
	model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(movXpi, movYpi, movZpi));
	model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
	shader.setMat4("model", model);
	piernaizq.Draw(shader);

	//PIE DERECHO
	model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(movXpd, movYpd, movZpd));
	model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
	shader.setMat4("model", model);
	piernader.Draw(shader);

	//CABEZA
	model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(movXc, movYc, movZc));
	model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
	shader.setMat4("model", model);
	cabeza.Draw(shader);

	/*
	

	

	
	
	
	

	
	//Bandido
	model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(-35.0f, -1.0f, -65.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
	shader.setMat4("model", model);
	Bandido.Draw(shader);
	//Bandido 2
	model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(-20.0f, -1.0f, -65.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
	shader.setMat4("model", model);
	Bandido.Draw(shader);
	//Bandido 3
	model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(-10.0f, -1.0f, -65.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
	shader.setMat4("model", model);
	Bandido.Draw(shader);
	*/


	// Draw skybox as last
	glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
	skyboxShader.use();
	view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
	model = glm::scale(glm::mat4(1.0f), glm::vec3(50, 50, 50));
	skyboxShader.setMat4("view", view);
	skyboxShader.setMat4("projection", projection);
	skyboxShader.setMat4("model", model);

	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // Set depth function back to default
}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lego", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, resize);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	//To Enable capture of our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	LoadTextures();
	myData();
	myData2();
	glEnable(GL_DEPTH_TEST);

	//For Models
	Shader modelShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	//For Primitives
	Shader primitivasShader("shaders/shader_texture_color.vs", "shaders/shader_texture_color.fs");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");

	/* CARGA DE MODELOS */

	// Load model

	// Load model
	Model pirata = ((char *)"Modelos/pirata/pirata.obj");
	Model CamionetaSD = ((char *)"Modelos/SD_C/CamionetaSD.fbx");
	Model PlaneSD = ((char *)"Modelos/SD_P/PlaneSD.fbx");
	Model CastilloSD = ((char *)"Modelos/Castillo/ultimatecastillo.obj");
	Model CamionLego = ((char *)"Modelos/VEHICULOSLEGO/CamionLego.obj");
	Model Pizzeria = ((char *)"Modelos/OtrosModelos/Lego_Pizza.obj");
	Model cuboG = ((char *)"Modelos/cuboGris/cuboG.obj");
	Model cuboB = ((char *)"Modelos/cuboBlanco/cuboB.obj");
	Model cuboC = ((char *)"Modelos/cuboCafec/cuboCC.obj");
	//Model trailer = ((char *)"Modelos/trailer/trailer.fbx");
	Model faro = ((char *)"Modelos/OtrosModelos/faro.obj");
	Model Casita = ((char *)"Modelos/OtrosModelos/casita.obj");
	Model Carro = ((char *)"Modelos/VEHICULOSLEGO/NEWCARRO.obj");
	Model Casita2 = ((char *)"Modelos/OtrosModelos/casita2.obj");
	Model Casita3 = ((char *)"Models/OtrosModelos/House.obj");
	Model Estudio = ((char *)"Modelos/OtrosModelos/Estudio.obj");
	Model Pandilla = ((char *)"Modelos/PERSONASLEGO/Pandilla.obj");
	Model Zombies = ((char *)"Modelos/PERSONASLEGO/LegoZombies.obj");
	Model Dudes = ((char *)"Modelos/PERSONASLEGO/LEGODUDES.fbx");
	Model brazoder = ((char *)"Modelos/LEGOMAN/BRAZODER.obj");
	Model brazoizq = ((char *)"Modelos/LEGOMAN/BRAZOIZQ.obj");
	Model torzo = ((char *)"Modelos/LEGOMAN/TORZO.obj");
	Model piernader = ((char *)"Modelos/LEGOMAN/PIERNADER.obj");
	Model piernaizq = ((char *)"Modelos/LEGOMAN/PIERNAIZQ.obj");
	Model cabeza = ((char *)"Modelos/LEGOMAN/CABEZA.obj");
	/*

	
	

	
	Model Bandido = ((char *)"Modelos/PERSONASLEGO/Bandido.obj");


	Model Dudes = ((char *)"Modelos/OtrosModelos/LEGODUDES.obj");
	
	
	Model Bandido = ((char *)"Modelos/OtrosModelos/Bandido.obj");
	*/

	/* TEXTURAS DEL SKY BOX*/

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right.tga");
	faces.push_back("SkyBox/left.tga");
	faces.push_back("SkyBox/top.tga");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/back.tga");
	faces.push_back("SkyBox/front.tga");
	/*
	faces.push_back("SkyBox/negx.jpg");
	faces.push_back("SkyBox/posx.jpg");
	faces.push_back("SkyBox/posy.jpg");
	faces.push_back("SkyBox/negy.jpg");
	faces.push_back("SkyBox/negz.jpg");
	faces.push_back("SkyBox/posz.jpg");
	*/

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);
	//GLuint cubemapTexture2 = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// render loop
	// While the windows is not closed
	while (!glfwWindowShouldClose(window))
	{
		PlaySound("file1.wav", NULL, SND_ASYNC);

		// per-frame time logic
		// --------------------
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// input
		// -----
		//my_input(window);
		animate();

		// render
		// Backgound color
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//display(modelShader, ourModel, llantasModel);
		display(modelShader, SkyBoxshader, primitivasShader, cubemapTexture, pirata,
			CamionetaSD, PlaneSD, CastilloSD, CamionLego, cuboG, cuboB, cuboC,
			Pizzeria, faro, Casita, Carro, Casita2, Casita3, Estudio, Pandilla, Zombies, Dudes/*, Bandido*/ ,cabeza, torzo, piernader,
			piernaizq, brazoder, brazoizq);


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);

	glfwTerminate();
	return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		lightPosition.z -= 0.5f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		lightPosition.z += 0.5f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		lightPosition.x += 0.5f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		lightPosition.x -= 0.5f;
	/* LUZ*/
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		animacion2 = true;

	/* PERSONAJE*/
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		animacion3 = true;
	}

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{

		vector<const GLchar*> faces;
		faces.push_back("SkyBox/grimmnight_rt.tga");
		faces.push_back("SkyBox/grimmnight_lf.tga");
		faces.push_back("SkyBox/grimmnight_up.tga");
		faces.push_back("SkyBox/grimmnight_dn.tga");
		faces.push_back("SkyBox/grimmnight_bk.tga");
		faces.push_back("SkyBox/grimmnight_ft.tga");
		
		GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);}
	
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		vector<const GLchar*> faces;
		faces.push_back("SkyBox/right.tga");
		faces.push_back("SkyBox/left.tga");
		faces.push_back("SkyBox/top.tga");
		faces.push_back("SkyBox/bottom.tga");
		faces.push_back("SkyBox/back.tga");
		faces.push_back("SkyBox/front.tga");
		GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);
	}

	/* MOVIMIENTO DEL AVION */

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		animacion  = true;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}