#pragma once

#include "../include/libs.h"


class Shader {
public:
    Shader();
    ~Shader();
    void load(const std::string& vertexPath, const std::string& fragmentPath);
    void setInt(const std::string& name, const int value);
    void setMat4(const std::string& name, const glm::mat4& mat);
    void setVec3(const std::string& name, const glm::vec3& mat);
    GLuint program;

private:
    std::string readFile(const std::string& path);
    void compile(const std::string& vertexCode, const std::string& fragmentCode);
};
