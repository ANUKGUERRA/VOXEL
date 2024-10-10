#include "Colider.h"


void Colider::coliderDrawSetUp() 
{
    glm::vec3 vertices_AABB[8] = {
        // Bottom face
        glm::vec3(min.x, min.y, min.z), // 0
        glm::vec3(max.x, min.y, min.z), // 1
        glm::vec3(max.x, min.y, max.z), // 2
        glm::vec3(min.x, min.y, max.z), // 3

        // Top face
        glm::vec3(min.x, max.y, min.z), // 4
        glm::vec3(max.x, max.y, min.z), // 5
        glm::vec3(max.x, max.y, max.z), // 6
        glm::vec3(min.x, max.y, max.z)  // 7
    };

    unsigned int indices_AABB[24] = {
        // Bottom face
        0, 1, 1, 2, 2, 3, 3, 0,
        // Top face
        4, 5, 5, 6, 6, 7, 7, 4,
        // Side edges
        0, 4, 1, 5, 2, 6, 3, 7
    };


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_AABB), vertices_AABB, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_AABB), indices_AABB, GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}

void Colider::coliderDraw()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

