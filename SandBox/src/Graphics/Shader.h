#pragma once
#include "../Core/config.h"


namespace myShader {
    class Shader {
    public:
        Shader();
        Shader(const string& vertexPath, const string& fragmentPath);
        ~Shader();
        void use();
        void load(const string& vertexPath, const string& fragmentPath);
        void setInt(const string& name, const int value);
        void setMat4(const string& name, const mat4& mat);
        void setVec3(const string& name, const vec3& mat);
        void setMat4Array(const string& name, const vector<mat4>& mats);
        GLuint program;

    private:
        string readFile(const string& path);
        void compile(const string& vertexCode, const string& fragmentCode);
    };
}
