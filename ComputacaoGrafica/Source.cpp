#include <GL/glew.h> /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h> /* GLFW helper library */
#include <stdio.h>
#include <math.h>
#include <ostream>
#include <iostream>
#include "Mesh.h"
#include "Camera.h"
#include "Shader.h"
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
Camera* camera = new Camera();
Shader coreShader;
ObjReader* objReader = new ObjReader();
std::vector<Obj3D*> objects;

void onScroll(GLFWwindow* window, double xPosition, double yPosition) {
	if (firstMouse) {
		mouseX = xPosition;
		mouseY = yPosition;
		firstMouse = false;
	}

	float offsetX = xPosition - mouseX;
	float offsetY = mouseY - yPosition;

	mouseX = xPosition;
	mouseY = yPosition;

	camera->processMouseInput(offsetX, offsetY);
}

void initializeGraphics() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(800, 600, "Call of Duty", nullptr, nullptr);

	//glfwGetFramebufferSize(window, 800, 600);
	glfwSetCursorPosCallback(window, onScroll);

	if (window == nullptr) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();

	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed no init GLEW." << std::endl;
	}

	glViewport(0, 0, 800, 600);
}

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

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		std::cout << "W pressed";
		camera->processKeyboardInput(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		std::cout << "S pressed";
		camera->processKeyboardInput(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		std::cout << "A pressed";
		camera->processKeyboardInput(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		std::cout << "D pressed";
		camera->processKeyboardInput(RIGHT, deltaTime);
	}
}

Face* createFace(int v0, int v1, int v2, int v3, int normal) {
	Face* face = new Face();
	face->vertices = { v0,v1,v3,v1,v2,v3 };
	return face;
}

Mesh* createCubeMesh() {
	int A = 0, B = 1, C = 2, D = 3, E = 4, F = 5, G = 6, H = 7;

	Mesh* mesh = new Mesh();
	mesh->vertices.push_back(new glm::vec3(0.5f, 0.5f, 0.5f));
	mesh->vertices.push_back(new glm::vec3(-0.5f, 0.5f, 0.5f));
	mesh->vertices.push_back(new glm::vec3(-0.5f, -0.5f, 0.5f));
	mesh->vertices.push_back(new glm::vec3(0.5f, -0.5f, 0.5f));
	mesh->vertices.push_back(new glm::vec3(0.5f, 0.5f, -0.5f));
	mesh->vertices.push_back(new glm::vec3(-0.5f, 0.5f, -0.5f));
	mesh->vertices.push_back(new glm::vec3(-0.5f, -0.5f, -0.5f));
	mesh->vertices.push_back(new glm::vec3(0.5f, -0.5f, -0.5f));

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
	coreShader = Shader("core.vert", "core.frag");
	coreShader.Use();

	//run:
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	coreShader.Use();
	coreShader.LoadTexture("bricks.jpg", "texture1", "bricksTexture");

	Obj3D* cube = objReader->readObjFromFile("Cubo.mtl");
	cube->destroyed = true;
	cube->position = new glm::vec3(-8.0f, -5.0f, -15.0f);
	objects.push_back(cube);

	std::vector<float> finalVector;

	for (Obj3D* obj : objects) {
		for (Group* group : obj->mesh->groups) {
			for (Face* face : group->faces) {
				//face vertices
				for (int verticeID : face->vertices) {
					glm::vec3* vertice = obj->mesh->vertices[verticeID];
					finalVector.push_back(vertice->x);
					finalVector.push_back(vertice->y);
					finalVector.push_back(vertice->z);
				}
				//face textures
				for (int textureID : face->textures) {
					glm::vec2* texture = obj->mesh->mappings[textureID];
					finalVector.push_back(texture->x);
					finalVector.push_back(texture->y);
				}
				//face normals
				for (int normalID : face->normals) {
					glm::vec3* normal = obj->mesh->normals[normalID];
					finalVector.push_back(normal->x);
					finalVector.push_back(normal->y);
					finalVector.push_back(normal->z);
				}

			}

			GLuint VBO;

			glGenVertexArrays(1, &group->vao);
			glGenBuffers(1, &VBO);

			glBindVertexArray(group->vao);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);

			glBufferData(GL_ARRAY_BUFFER, group->numVertices() * sizeof(GLfloat), finalVector.data(), GL_STATIC_DRAW);

			// vertices
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0));
			glEnableVertexAttribArray(0);

			// textures
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);

			// normals
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
			glEnableVertexAttribArray(2);
		}

	}


	//game loop
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		coreShader.Use();
	
		for (int i = 0; i < objects.size(); ++i) {
			if (objects[i]->isProjectile) {
				//moveBullet(objects[i], deltaTime);
				//processCollisions(i);
			}

			objects[i]->draw(camera, &coreShader);
		}

		glfwSwapBuffers(window);
	}
}
