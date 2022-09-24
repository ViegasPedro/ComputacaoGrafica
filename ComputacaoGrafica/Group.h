#pragma once
#include <vector>
#include "Face.h"
#include <GL/glew.h>
#include <string>

class Group {

	//String name e material
	//array faces
	//vao
	//array vbos
public:
	std::string name;
	std::string material;
	std::vector<Face*> faces;
	GLuint vao;
	std::vector<GLuint> vbos;

	int addFace(Face* face) {
		faces.push_back(face);
		return faces.size() - 1;
	}

	int addVBO(GLuint* vbo) {
		vbos.push_back(*vbo);
		return vbos.size() - 1;
	}

	int numVertices() {
		int sum = 0;
		for (auto& face : faces) {
			sum += face->vertices.size();
		}
		return sum;
	}

};

