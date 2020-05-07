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

// Other Libs
#include "SOIL2/SOIL2.h"

void resize(GLFWwindow* window, int width, int height);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
// Window size
int SCR_WIDTH = 38000;
int SCR_HEIGHT = 76000;

GLFWmonitor *monitors;
GLuint skyboxVBO, skyboxVAO, VAO, EBO,VBO;

//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
double	lastX = 0.0f,
		lastY = 0.0f;
bool firstMouse = true;

//Timing
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, 3.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

void myData(void);
void myData2(void);
void display(Shader, Model, Model);
void getResolution(void);
void animate(void);
void LoadTextures(void);
unsigned int generateTextures(char*, bool);

//Texture
unsigned int	t_smile,
				t_toalla,
				t_unam,
				t_white,
				t_panda,
				t_cubo,
				t_caja,
				t_caja_brillo;

//For model
bool animacion = false;

/* VARIABLES PARA EL MOVIMIENTO DEL AUTO */

float	movAuto_z = 0.0f,
		movAuto_y = -1.75f;

/* VARIABLES DE CONTROL */

bool	avanza = false,			// MOVIMIENTO HORIZONTAL
		sube = true,			// MOVIMIENTO VERTICAL
		EA = true,				// ENABLE HORIZONTAL
		ES = false,				// ENABLE VERTICAL
		AV = false;				// ENABLE FINAL (PARA EL ULTIMO DESPLAZAMIENTO)
	


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
	t_smile = generateTextures("Textures/ficha.jpg", 0);
}

void myData()
{	
	

	/* VERTICES DE UN CUBO */

	float vertices[] = {

		-0.5f, -0.5f, 0.5f,		0.0f,0.0f,0.0f,	//V0 - Frontal
		0.5f, -0.5f, 0.5f,		1.0f,0.0f,0.0f,	//V1
		0.5f, 0.5f, 0.5f,		1.0f,1.0f,0.0f,	//V5
		-0.5f, 0.5f, 0.5f,		0.0f,1.0f,0.0f,   //V4

		0.5f, -0.5f, -0.5f,		0.0f,0.0f,0.0f,	//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	1.0f,0.0f,0.0f,   //V3
		-0.5f, 0.5f, -0.5f,		1.0f,1.0f,0.0f,	//V7
		0.5f, 0.5f, -0.5f,		0.0f,1.0f,0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		0.0f,0.0f,0.0f,	//V4 - Izq
		-0.5f, 0.5f, -0.5f,		1.0f,0.0f,0.0f,	//V7
		-0.5f, -0.5f, -0.5f,	1.0f,1.0f,0.0f,	//V3
		-0.5f, -0.5f, 0.5f,		0.0f,1.0f,0.0f,	//V0

		0.5f, 0.5f, 0.5f,		0.0f,0.0f,0.0f,	//V5 - Der
		0.5f, -0.5f, 0.5f,		1.0f,0.0f,0.0f,	//V1
		0.5f, -0.5f, -0.5f,		1.0f,1.0f,0.0f,	//V2
		0.5f, 0.5f, -0.5f,		0.0f,1.0f,0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		0.0f,0.0f,0.0f,	//V4 - Sup
		0.5f, 0.5f, 0.5f,		1.0f,0.0f,0.0f,	//V5
		0.5f, 0.5f, -0.5f,		1.0f,1.0f,0.0f,	//V6
		-0.5f, 0.5f, -0.5f,		0.0f,1.0f,0.0f,//V7

		-0.5f, -0.5f, 0.5f,		0.0f,0.0f,0.0f,	//V0 - Inf
		-0.5f, -0.5f, -0.5f,	1.0f,0.0f,0.0f,	//V3
		0.5f, -0.5f, -0.5f,		1.0f,1.0f,0.0f,	//V2
		0.5f, -0.5f, 0.5f,		0.0f,1.0f,0.0f,
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

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
}

void display(Shader shader, Shader skyboxShader, GLuint skybox,Model piso, Model PisoB, 
	Model pirata, Model CamionetaSD, Model PlaneSD, Model CastilloSD, Model CamionLego, 
	Model Pizzeria, Model cuboG, Model cuboB, Model cuboC, Model trailer)
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
	glBindVertexArray(VAO);
	float i =0.0f,
		  j =0.0f;
	//CARRETERA

	for ( i = 0; i <= 15.5; i=i+1.55f)  //PARTE VERTICAL GRIS 1
	{
		for (j = 0; j < 62; j=j+1.55f)
		{
			model = glm::translate(glm::mat4(1.0f), glm::vec3(i, -1.5,j));
			//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
			shader.setMat4("model", model);
			cuboG.Draw(shader);
		}
	}

	for ( i = -1.55f; i >= -31.0f; i = i - 1.55f)  //PARTE HORIZONTAL GRIS 1
	{
		for (j = -1.55f; j >= -15.5f; j = j - 1.55f)
		{
			model = glm::translate(glm::mat4(1.0f), glm::vec3(i, -1.5f, j));
			//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			shader.setMat4("model", model);
			cuboG.Draw(shader);
		}
	}

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

	for (i = 0.0f; i <= 77.5f; i = i + 1.55f) //PARTE HORIZONTAL GRIS ARCO
	{
		for (j = 63.55; j <= 79.05; j = j + 1.55f)
		{
			model = glm::translate(glm::mat4(1.0f), glm::vec3(i, -1.5f, j));
			//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			shader.setMat4("model", model);
			cuboG.Draw(shader);
		}
	}

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.5f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 10.0f,10.0f));
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_smile);
	shader.setMat4("model", model);
	glDrawArrays(GL_QUADS,0, 24);

	//PIRATA
	model = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, -1.5f, 0.0f));
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
	tmp = model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 10.0f));

	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
	//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	shader.setMat4("model", model);
	PlaneSD.Draw(shader);
	

	
	/**/
	//Castillo SD

	model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(glm::mat4(1.0f), glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(0.0f, 0.0f, 50.0f));

	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
	//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	shader.setMat4("model", model);
	CastilloSD.Draw(shader);


	//Camion Lego

	model = glm::rotate(glm::mat4(1.0f), glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//model = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//model = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 10.0f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
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
	/*
	//Pizzeria 

	model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(-15.0f, 0.0f, 20.0f));

	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	shader.setMat4("model", model);
	Pizzeria.Draw(shader);
	*/

	// Draw skybox as last
	glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
	skyboxShader.use();
	view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix

	skyboxShader.setMat4("view", view);
	skyboxShader.setMat4("projection", projection);

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
	myData2();
	myData();
	
	glEnable(GL_DEPTH_TEST);
	
	//For Models
	Shader modelShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	//For Primitives
	//Shader primitivasShader("shaders/shader_texture_color.vs", "shaders/shader_texture_color.fs");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");

	/* CARGA DE MODELOS */

	// Load model
	//Model ourModel = ((char *)"Models/Lambo/carroseria.obj");
	//Model llantasModel = ((char *)"Models/Lambo/Wheel.obj");
	Model pisoModel = ((char *)"Models/piso/piso.obj");
	Model pisoBlanco = ((char *)"Models/pisoB/piso.obj");
	Model pirata = ((char *)"Modelos/pirata/pirata.obj");
	Model CamionetaSD = ((char *)"Modelos/SD_C/CamionetaSD.fbx");
	Model PlaneSD = ((char *)"Modelos/SD_P/PlaneSD.fbx");
	Model CastilloSD = ((char *)"Modelos/Castillo/ultimatecastillo.fbx");
	Model CamionLego = ((char *)"Modelos/OtrosModelos/CamionLego.fbx");
	Model Pizzeria = ((char *)"Modelos/OtrosModelos/Lego_Pizza.fbx");
	Model cuboG = ((char *)"Modelos/cuboGris/cuboG.obj");
	Model cuboB = ((char *)"Modelos/cuboBlanco/cuboB.obj");
	Model cuboC = ((char *)"Modelos/cuboCafec/cuboCC.obj");
	Model trailer = ((char *)"Modelos/trailer/trailer.fbx");


	/* TEXTURAS DEL SKY BOX*/

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right.tga");
	faces.push_back("SkyBox/left.tga");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/top.tga");
	faces.push_back("SkyBox/back.tga");
	faces.push_back("SkyBox/front.tga");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);
    
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
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
		display(modelShader, SkyBoxshader, cubemapTexture,pisoModel,pisoBlanco, 
			pirata, CamionetaSD, PlaneSD, CastilloSD, CamionLego, Pizzeria, cuboG, cuboB, cuboC,trailer);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
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
		lightPosition.z -=0.5f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		lightPosition.z += 0.5f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		lightPosition.x += 0.5f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		lightPosition.x -= 0.5f;

	/* MOVIMIENTO DEL AUTO */

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		animacion = true;
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
