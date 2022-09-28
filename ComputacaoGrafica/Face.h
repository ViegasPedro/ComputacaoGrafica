#pragma once

#include <vector>

class Face {

public:

    std::vector<int> textures;
    std::vector<int> vertices;
    std::vector<int> normals;

    int verticesCount() {
        return vertices.size();
    }

    void addVerticeId(int idVertice) {
        this->vertices.push_back(idVertice);
    }
};