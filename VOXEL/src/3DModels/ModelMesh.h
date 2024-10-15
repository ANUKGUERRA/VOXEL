#pragma once

#include <vec3.hpp>
#include <string>
#include <vector>
#include "../Shader/Shader.h"
#include "../Collider.h"

using std::string;
using std::vector;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	string type;
};


class Mesh {
public:
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

	void draw(Shader &shader);
	Collider collider;
	void generateCollider();
private:
    unsigned int VAO, VBO, EBO;
	void generateMesh();
};