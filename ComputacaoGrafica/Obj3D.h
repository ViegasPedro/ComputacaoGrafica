#pragma once
#include "Camera.h"
#include "Mesh.h"
#include "Shader2.h"

class Obj3D {
public:
    Mesh* mesh;

    glm::vec3 min;
    glm::vec3 max;


    float scale;
    glm::vec3* position = new glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3* direction;

    bool destroyed = false;
    bool canBeDestroyed = true;
    bool reflected = false;
    bool isProjectile = false;

    void draw(Camera* camera, Shader2* shader) {
        
        if (destroyed) {
            return;
        }
        // Get a handle for our "MVP" uniform
        GLuint MatrixID = glGetUniformLocation(shader->ID, "MVP");
        
        glm::mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(camera->zoom), (float)800 / (float)600, 0.1f, 100.0f);
        //glm::mat4 projection = camera->getProjectionMatrix();
        glm::mat4 view = camera->getViewMatrix();
        //glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        //glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, *position);
        model = glm::scale(model, glm::vec3(scale, scale, scale));

        //glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

        glm::mat4 MVP = projection * view * model;
        // Send our transformation to the currently bound shader, 
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        shader->setMatrix4fv("MVP", MVP);

        for (Group* group : mesh->groups) {
            glBindVertexArray(group->vao);
            glDrawArrays(GL_TRIANGLES, 0, group->numVertices());
        }
        
    }

    float getRadius() {
        return (distance(min, max) / 2) * scale;
    }

    glm::vec3 getCenter() {
        return *position;
    }

    bool testCollision(Obj3D* object) {
        float distanceAB = distance(object->getCenter(), getCenter());
        float radiusSum = object->getRadius() + getRadius();
        return (distanceAB < radiusSum);
    }
};

