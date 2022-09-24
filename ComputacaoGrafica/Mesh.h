#pragma once
#include <vector>
#include "Group.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Mesh {

public:
    std::vector<Group*> groups;
    std::vector<glm::vec2*> mappings;
    std::vector<glm::vec3*> vertices;
    std::vector<glm::vec3*> normals;

    int addGroup(Group* group) {
        groups.push_back(group);
        return groups.size() - 1;
    };
};

