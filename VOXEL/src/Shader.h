#pragma once

#include "../include/libs.h"
//#include "camera.h"
//#include "shader.h"
//#include "cube.h"


class Shader {
public:
    Shader();
    ~Shader();
    void load(const std::string& vertexPath, const std::string& fragmentPath);
    void setMat4(const std::string& name, const glm::mat4& mat);
    GLuint program;

private:
    std::string readFile(const std::string& path);
    void compile(const std::string& vertexCode, const std::string& fragmentCode);
};
