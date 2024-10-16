//#include "ModelMesh.h"
//
//Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) 
//{
//	this->vertices = vertices;
//	this->indices = indices;
//	this->textures = textures;
//
//	Mesh::generateMesh();
//}
//
//void Mesh::generateMesh()
//{
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),&indices[0], GL_STATIC_DRAW);
//
//
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
//
//
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
//
//
//    glEnableVertexAttribArray(2);
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
//
//    glBindVertexArray(0);
//}
//
//void Mesh::draw(Shader& shader)
//{
//    unsigned int diffuseNr = 1;
//    unsigned int specularNr = 1;
//    for (unsigned int i = 0; i < textures.size(); i++)
//    {
//        glActiveTexture(GL_TEXTURE0 + i);
//
//        string number;
//        string name = textures[i].type;
//        if (name == "texture_diffuse")
//            number = std::to_string(diffuseNr++);
//        else if (name == "texture_specular")
//            number = std::to_string(specularNr++);
//
//        shader.setInt(("material." + name + number).c_str(), i);
//        glBindTexture(GL_TEXTURE_2D, textures[i].id);
//    }
//    glActiveTexture(GL_TEXTURE0);
//
//
//    glBindVertexArray(VAO);
//    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//    glBindVertexArray(0);
//}
//
//void Mesh::generateCollider()
//{
//    glm::vec3 min;
//    glm::vec3 max;
//    
//    if (vertices.size() > 0) {
//        min = vertices[0].Position;
//        max = vertices[0].Position;
//
//        for (unsigned int i = 1; i < vertices.size(); i++) 
//        {
//            glm::vec3 pos = vertices[i].Position;
//
//            min.x = std::min(min.x, pos.x);
//            min.y = std::min(min.y, pos.y);
//            min.z = std::min(min.z, pos.z);
//
//            max.x = std::max(max.x, pos.x);
//            max.y = std::max(max.y, pos.y);
//            max.z = std::max(max.z, pos.z);
//        }
//    }
//    collider = Collider(min, max);
//}