#include "Shader.h"

myShader::Shader::Shader() : program(0) {}
myShader::Shader::Shader(const string& vertexPath, const string& fragmentPath) : program(0) {
    string vertexCode = readFile(vertexPath);
    string fragmentCode = readFile(fragmentPath);
    compile(vertexCode, fragmentCode);
}

myShader::Shader::~Shader() {
    glDeleteProgram(program);
}

void myShader::Shader::load(const string& vertexPath, const string& fragmentPath) {
    string vertexCode = readFile(vertexPath);
    string fragmentCode = readFile(fragmentPath);
    compile(vertexCode, fragmentCode);
}
void myShader::Shader::use() {
    glUseProgram(program);
}


void myShader::Shader::setInt(const string& name, const int value) {
    GLint location = glGetUniformLocation(program, name.c_str());
    glUniform1i(location, value);
}

void myShader::Shader::setMat4(const string& name, const mat4& value) {
    GLint location = glGetUniformLocation(program, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(value));
}

void myShader::Shader::setVec3(const string& name, const vec3& value) {
    GLint location = glGetUniformLocation(program, name.c_str());
    glUniform3fv(location, 1, value_ptr(value));
}

void myShader::Shader::setMat4Array(const string& name, const vector<mat4>& mats) {
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniformMatrix4fv(location, mats.size(), GL_FALSE, &mats[0][0][0]);
}

string myShader::Shader::readFile(const string& path) {
    ifstream file;
    file.exceptions(ifstream::failbit | ifstream::badbit);
    stringstream stream;

        file.open(path);
        stream << file.rdbuf();
        file.close();


    return stream.str();
}

void myShader::Shader::compile(const string& vertexCode, const string& fragmentCode) {
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;
    int success;
    char infoLog[512];



    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }



    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
    }



    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
