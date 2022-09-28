#include <GL/glew.h> /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h> /* GLFW helper library */
#include <stdio.h>
#include <math.h>
#include <ostream>
#include <iostream>
#include "Mesh.h"
#include "Camera.h"
#include "Shader.h"
#include "Shader2.h"
#include "ObjReader.h"

GLFWwindow* window = NULL;
const GLubyte* renderer;
const GLubyte* version;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX;
float lastY;
float mouseX = 800 / 2.0f;
float mouseY = 600 / 2.0f;
bool firstMouse = true;
Camera* camera = new Camera(glm::vec3(4.5f, 2.0f, 9.0f));
Shader2* coreShader2;
//ObjReader* objReader = new ObjReader();
std::vector<Obj3D*> objects;

void reflectBullet(Obj3D* bullet) {
	if (bullet->reflected)
		return;

	glm::vec3 reflection = glm::cross(*bullet->direction, camera->calculateUp());
	bullet->direction = &reflection;
	bullet->reflected = true;
	printf("REFLETINDO O TIRO \n");
}

void processCollisions(Obj3D* bullet) {
	for (Obj3D* obj : objects) {
		if (!obj->isProjectile) {
			if (bullet->testCollision(obj)) {
				if (obj->canBeDestroyed) {
					obj->destroyed = true;
				} else {
					reflectBullet(bullet);
				}
			};
		}
	}
}

void shoot(Obj3D* modelObject) {
	Obj3D* bullet = new Obj3D();
	bullet->mesh = modelObject->mesh;
	bullet->min = modelObject->min;
	bullet->max = modelObject->max;
	bullet->isProjectile = true;
	bullet->destroyed = false;
	bullet->scale = 0.01f;
	bullet->direction = new glm::vec3(camera->front);
	bullet->position = new glm::vec3(camera->position + camera->front);
	objects.push_back(bullet);
}

void moveBullet(Obj3D* bullet, float deltaTime) {
	bullet->position->x = bullet->position->x + (deltaTime * 2 * bullet->direction->x);
	bullet->position->y = bullet->position->y + (deltaTime * 2 * bullet->direction->y);
	bullet->position->z = bullet->position->z + (deltaTime * 2 * bullet->direction->z);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera->processMouseScroll(yoffset);
}

void mouse_callback(GLFWwindow* window, double xPosition, double yPosition) {
	if (firstMouse) {
		mouseX = xPosition;
		mouseY = yPosition;
		firstMouse = false;
	}

	float offsetX = xPosition - mouseX;
	float offsetY = mouseY - yPosition;

	mouseX = xPosition;
	mouseY = yPosition;

	camera->processMouseMovement(offsetX, offsetY);
}

void initializeGraphics() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(800, 600, "Computação Gráfica", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();

	}
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwMakeContextCurrent(window);

	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, 800 / 2, 600 / 2);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed no init GLEW." << std::endl;
	}

	glViewport(0, 0, 800, 600);
	/* tell GL to only draw onto a pixel if the shape is closer to the viewer */
	glEnable(GL_DEPTH_TEST); /* enable depth-testing */
	glDepthFunc(GL_LESS);/*depth-testing interprets a smaller value as "closer"*/
}

//deletar esse cara
void initializeGraphics2() {
	/* start GL context and O/S window using the GLFW helper library */
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		std::cout << "ERROR: Could not start GLFW3" << std::endl;
	}

	/* change to 3.2 if on Apple OS X */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(
		800, 600, "CGR - Cube", NULL, NULL
	);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		std::cout << "ERROR: Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	/* start GLEW extension handler */
	glewExperimental = GL_TRUE;
	glewInit();

	glViewport(0, 0, 800, 600);

	/* get version info */
	renderer = glGetString(GL_RENDERER); /* get renderer string */
	version = glGetString(GL_VERSION); /* version as a string */
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	/* tell GL to only draw onto a pixel if the shape is closer to the viewer */
	glEnable(GL_DEPTH_TEST); /* enable depth-testing */
	glDepthFunc(GL_LESS);/*depth-testing interprets a smaller value as "closer"*/

}

void processInput(GLFWwindow* window, float deltaTime, Obj3D* obj3) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera->processKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera->processKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera->processKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera->processKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		shoot(obj3);
	}
}

Face* createFace(int v0, int v1, int v2, int v3, int normal) {
	Face* face = new Face();
	//face->vertices = { v0,v1,v3,v1,v2,v3 };

	face->addVerticeId(v0);
	face->addVerticeId(v1);
	face->addVerticeId(v3);

	face->addVerticeId(v1);
	face->addVerticeId(v2);
	face->addVerticeId(v3);
	return face;
}

Mesh* createCubeMesh() {
	int A = 0, B = 1, C = 2, D = 3, E = 4, F = 5, G = 6, H = 7;
	float escale = 2;
	/*
	Mesh* mesh = new Mesh();
	mesh->vertices.push_back(new glm::vec3(0.5f, 0.5f, 0.5f));
	mesh->vertices.push_back(new glm::vec3(-0.5f, 0.5f, 0.5f));
	mesh->vertices.push_back(new glm::vec3(-0.5f, -0.5f, 0.5f));
	mesh->vertices.push_back(new glm::vec3(0.5f, -0.5f, 0.5f));
	mesh->vertices.push_back(new glm::vec3(0.5f, 0.5f, -0.5f));
	mesh->vertices.push_back(new glm::vec3(-0.5f, 0.5f, -0.5f));
	mesh->vertices.push_back(new glm::vec3(-0.5f, -0.5f, -0.5f));
	mesh->vertices.push_back(new glm::vec3(0.5f, -0.5f, -0.5f));
	*/

	Mesh* mesh = new Mesh();
	mesh->vertices.push_back(new glm::vec3(1.0f / escale, 1.0f / escale, 1.0f / escale)); //A
	mesh->vertices.push_back(new glm::vec3(-1.0f / escale, 1.0f / escale, 1.0f / escale)); //B
	mesh->vertices.push_back(new glm::vec3(-1.0f / escale, -1.0f / escale, 1.0f / escale)); //C
	mesh->vertices.push_back(new glm::vec3(1.0f / escale, -1.0f / escale, 1.0f / escale)); //D      
	mesh->vertices.push_back(new glm::vec3(1.0f / escale, 1.0f / escale, -1.0f / escale)); //E
	mesh->vertices.push_back(new glm::vec3(-1.0f / escale, 1.0f / escale, -1.0f / escale)); //F
	mesh->vertices.push_back(new glm::vec3(-1.0f / escale, -1.0f / escale, -1.0f / escale)); //G
	mesh->vertices.push_back(new glm::vec3(1.0f / escale, -1.0f / escale, -1.0f / escale)); //H

	Group* group = new Group();
	group->addFace(createFace(A, B, C, D, 4));
	group->addFace(createFace(A, D, H, E, 0));
	group->addFace(createFace(A, E, F, B, 2));
	group->addFace(createFace(C, B, F, G, 1));
	group->addFace(createFace(D, C, G, H, 3));
	group->addFace(createFace(H, G, F, E, 5));

	mesh->addGroup(group);

	return mesh;
}

int main() {

	initializeGraphics();

	//tranformar em metodo openGlSetup
	glEnable(GL_BLEND);	// Enables blending ( glBlendFunc )
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_DEPTH_TEST);
	
	//transformar no metodo systemSetup
	/*
	coreShader = new Shader("core.vert", "core.frag");
	coreShader->use();
	
	Shader* colorShader = new Shader("core.vert", "fragment_color.frag");
	*/
	//coreShader = new Shader("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");
	//coreShader->use();

	// Create and compile our GLSL program from the shaders
	coreShader2 = new Shader2("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	//GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	glm::ortho(0.0f, (float)800, 0.0f, (float)600, 0.1f, 100.0f);
	//std::vector<glm::vec3*> cubePositions;
	//cubePositions.push_back(new glm::vec3(3.0f, 2.0f, 0.0f));

	//run:
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	//coreShader.Use();
	//coreShader.LoadTexture("bricks.jpg", "texture1", "bricksTexture");

	//Obj3D* cube = objReader->readObjFromFile("Cubo.mtl");
	//cube->destroyed = true;
	//cube->position = new glm::vec3(-8.0f, -5.0f, -15.0f);
	//objects.push_back(cube);

	Mesh* mesh1 = createCubeMesh();

	//Mesh* cube2Mesh = createCubeMesh();

	Obj3D* cube1 = new Obj3D();
	cube1->position = new glm::vec3(3.0f, 2.0f, 0.0f);
	cube1->mesh = mesh1;
	cube1->scale = 0.7f;
	cube1->canBeDestroyed = false;

	Obj3D* cube2 = new Obj3D();
	cube2->position = new glm::vec3(2.0f, 2.0f, 0.0f);
	cube2->mesh = mesh1;
	cube2->scale = 0.3f;

	Obj3D* obj3 = new Obj3D();
	obj3->mesh = mesh1;
	obj3->min = glm::vec3(0.0f, 0.0f, 0.0f);
	obj3->max = glm::vec3(10.0f, 10.0f, 10.0f);
	obj3->scale = 0.2f;
	obj3->canBeDestroyed = false;
	obj3->position = new glm::vec3(16.0f, -5.0f, -15.0f);
	objects.push_back(obj3);

	objects.push_back(cube1);
	objects.push_back(cube2);

	for (Obj3D* obj : objects) {
		for (Group* group : obj->mesh->groups) {
			std::vector<float> finalVector;

			for (Face* face : group->faces) {
				//face vertices
				for (int verticeID : face->vertices) {
					glm::vec3* vertice = obj->mesh->vertices[verticeID];
					finalVector.push_back(vertice->x);
					finalVector.push_back(vertice->y);
					finalVector.push_back(vertice->z);
				}
			}

			GLuint VAO, VBOvert;

			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBOvert);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBOvert);

			glBufferData(GL_ARRAY_BUFFER, finalVector.size() * sizeof(GLfloat), &finalVector.front(), GL_STATIC_DRAW);

			// vertices
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
			glEnableVertexAttribArray(0);

			group->setVAO(&VAO);
			group->addVBO(&VBOvert);
		}

	}

	//game loop
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();

		processInput(window, deltaTime, obj3);

		//set backgroud collor
		glClearColor(0.2f, 0.7f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		coreShader2->use();

		for (Obj3D* obj : objects) {
			if (obj->isProjectile) {
				moveBullet(obj, deltaTime);
				processCollisions(obj);
			}

			obj->draw(camera, coreShader2);
		}

		glfwSwapBuffers(window);
	}
}
