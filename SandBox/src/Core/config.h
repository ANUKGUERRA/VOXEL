#pragma once
#define GLM_ENABLE_EXPERIMENTAL
//opengl
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//glm
#include <glm.hpp>
#include <gtx/hash.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>

//c++
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cmath>

#include <fstream>
#include <sstream>
#include <future>

//entt
#include <entt.hpp>

//assimp
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/quaternion.h>
#include <assimp/Importer.hpp>
#include <assimp/vector3.h>
#include <assimp/matrix4x4.h>


//stb image
#include <stb_image.h>

//noise lite
#include <FastNoiseLite.h>

//Text
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>

//Sound


using namespace std;
using namespace glm;

inline int windowWidth = 800;
inline int windowHeight = 600;

inline double mouseX = 0.0f;
inline double mouseY = 0.0f;
inline GLFWwindow* window;
enum GameState { MainMenu, Game, PauseMenu};
struct MouseState {
	bool isPressed = false;
	bool isReleased = true;
	bool clickComplete = false;
	double x = 0.0;
	double y = 0.0;
};

inline MouseState mouseState;
inline GameState state;



struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;

	Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& texCoord) : position(pos), normal(norm), uv(texCoord) {}
};