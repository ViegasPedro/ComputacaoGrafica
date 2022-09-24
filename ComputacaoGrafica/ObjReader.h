#pragma once
#include "Obj3D.h"

class ObjReader {

public:

    Obj3D* readObjFromFile(std::string fileName) {

        Obj3D* obj3D = new Obj3D();
        Mesh* mesh = new Mesh();
        Group* currentGroup = new Group();
        obj3D->mesh = mesh;
        int verticesCount = 0;

        std::ifstream objFile(fileName);

        int count = 0;
        while (!objFile.eof()) {

            count++;
            std::string line;
            getline(objFile, line);
            std::stringstream sline;
            sline << line;
            std::string firstToken;
            sline >> firstToken;

            std::cout << "\n Linha: " << count << " Token: " << firstToken;

            if (firstToken == "v") {
                float x, y, z;
                sline >> x >> y >> z;
                calculateBoundingBox(obj3D, x, y, z, count);
                mesh->vertices.push_back(new glm::vec3(x, y, z));
            }
            else if (firstToken == "vn") {
                float x, y, z;
                sline >> x >> y >> z;
                mesh->normals.push_back(new glm::vec3(x, y, z));
            }
            else if (firstToken == "vt") {
                float x, y;
                sline >> x >> y;
                mesh->mappings.push_back(new glm::vec2(x, y));
            }
            else if (firstToken == "g") {
                currentGroup = new Group();
                mesh->addGroup(currentGroup);
            }
            else if (firstToken == "f") {
                processFLine(sline, currentGroup);
            }
        }

        return obj3D;
    }

private:

    void processTriplet(Face* face, std::string token, Group* group) {
        std::stringstream stoken(token);
        std::string aux;


        getline(stoken, aux, '/');
        if (!aux.empty()) {
            face->vertices.push_back(stoi(aux) - 1);
        }

        getline(stoken, aux, '/');
        if (!aux.empty()) {
            face->textures.push_back(stoi(aux) - 1);
        }
        getline(stoken, aux);
        if (!aux.empty()) {
            face->normals.push_back(stoi(aux) - 1);
        }

        //group->numOfVertices++;
    }

    void calculateBoundingBox(Obj3D* obj3D, int x, int y, int z, int count) {
        if (count == 1) {
            obj3D->max.x = x;
            obj3D->max.y = y;
            obj3D->max.z = z;
            obj3D->min.x = x;
            obj3D->min.y = y;
            obj3D->min.z = z;
        }
        if (x > obj3D->max.x)
            obj3D->max.x = x;
        if (y > obj3D->max.y)
            obj3D->max.y = y;
        if (z > obj3D->max.z)
            obj3D->max.z = z;
        if (x < obj3D->min.x)
            obj3D->min.x = x;
        if (y < obj3D->min.y)
            obj3D->min.y = y;
        if (z < obj3D->min.z)
            obj3D->min.z = z;
    }

    void processFLine(std::stringstream& sline, Group* group) {
        Face* face = new Face();
        std::string first, second, third, fourth, token;

        sline >> first;
        processTriplet(face, first, group);
        sline >> second;
        processTriplet(face, second, group);
        sline >> third;
        processTriplet(face, third, group);

        sline >> fourth;
        if (!fourth.empty()) {
            std::stringstream newFLine(first + " " + third + " " + fourth);
            processFLine(newFLine, group);
        }

        group->addFace(face);
    }
};


