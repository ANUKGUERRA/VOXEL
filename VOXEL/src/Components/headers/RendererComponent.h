#ifndef RENDERER_COMPONENT_H
#define RENDERER_COMPONENT_H
#include <GL/glew.h>
#include <vector>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "../../Shader/Shader.h"
#include "Component.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "../../Application.h"
class Application;
class RendererComponent : public Component
{
public:
    RendererComponent(CameraComponent* camera, TransformComponent* transform) : camera(Application::camera), transform(transform) 
    {
    }
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
	};

	GLuint VAO, VBO, EBO;
    unsigned int n_elements;

    CameraComponent *camera;
    TransformComponent *transform;

    Shader shader;

    void setShader(const std::string& vertexPath, const std::string& fragmentPath) 
    {
        shader.load(vertexPath, fragmentPath);
    }

    void updateUniforms() 
    {
        shader.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera->getZoom()),(float)Application::widowWidth / (float)Application::windowHeight, 0.1f, 1000.0f);
        glm::mat4 view = camera->getViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model);

        shader.setVec3("lightDir", glm::vec3(0.7f, -0.4f, 0.6f));
        shader.setVec3("viewPos", transform->position);
        shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setVec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));
    }

    void draw()
    {
        updateUniforms();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, n_elements, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        shader.unuse();
    }

    void setUpBuffers(std::vector<Vertex> vertices, std::vector<int> indices)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);

        // Normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);

        // UV
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
        n_elements = indices.size();
    }
};

#endif

