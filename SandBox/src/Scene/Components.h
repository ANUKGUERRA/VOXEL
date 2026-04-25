#pragma once
#include"../Core/config.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Material.h"
#include "../Animation/Animation.h"


struct TransformComponent
{
    vec3 Position = vec3(0.0f);
    quat Rotation;
    vec3 Scale = vec3(1.0f);

    TransformComponent() = default;
    TransformComponent(const vec3& pos) : Position(pos) {}
    TransformComponent(const vec3& pos, const vec3& rot) : Position(pos), Rotation(rot) {}
};

struct MeshComponent
{
    GLuint VAO = 0;  
    GLuint VBO = 0;  
    GLuint EBO = 0;  
    GLsizei indexCount = 0;

    MeshComponent(const vector<vec3>& vertices, const vector<GLuint>& indices) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
        glEnableVertexAttribArray(0);

        indexCount = static_cast<GLsizei>(indices.size());

        glBindVertexArray(0);
    }

    ~MeshComponent() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
};

struct ColliderComponent
{
    vec3 Size = vec3(1.0f);
    vec3 min;
    vec3 max;

    ColliderComponent() = default;
    ColliderComponent(const vec3& size) : Size(size), min(-size / 2.0f), max(size / 2.0f) {}
    ColliderComponent(const vec3& min, const vec3& max) : Size(max - min), min(min), max(max) {}

    bool Intersects(const ColliderComponent& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
            (min.y <= other.max.y && max.y >= other.min.y) &&
            (min.z <= other.max.z && max.z >= other.min.z);
    }
};

struct RigidBodyComponent {
    glm::vec3 Velocity = glm::vec3(0.0f);
    glm::vec3 HorizontalVelocity = glm::vec3(0.0f);
    glm::vec3 Acceleration = glm::vec3(0.0f);
    bool IsStatic = false;
    bool IsGrounded = false;
};

struct CameraComponent
{
    float YAW = -90.0f;
    float PITCH = 0.0f;
    float SENSITIVITY = 0.1f;
    vec3 Front = vec3(0.0f, 0.0f, -1.0f);
    vec3 Up = vec3(0.0f, 1.0f, 0.0f);
    vec3 Right = vec3(1.0f, 0.0f, 0.0f);
    vec3 WorldUp = vec3(0.0f, 1.0f, 0.0f);

    float FOV = 45.0f;
    float NearPlane = 0.1f;
    float FarPlane = 1000.0f;
    bool IsActive = true;
};

struct RenderableComponent
{
    myShader::Shader* shader;
    Material* material;
};

struct AnimatedModelComponent {
    AnimatedMesh* mesh;
    myShader::Shader* shader;
    Material* material;
    vec3 min;
    vec3 max;
    AnimatedModelComponent(const string& path, const char* texturePath)
    {
        mesh = new AnimatedMesh();
        mesh->LoadMesh(path);
        mesh->GetBoundingBox(min, max);
        shader = new myShader::Shader("res/shaders/animation.vs", "res/shaders/animation.frs");
        material = new Material(texturePath);
    }
};

struct TimerComponent {
    float timeRemaining;
    int nextAnimation;

    TimerComponent(float duration, int nextAnim) : timeRemaining(duration), nextAnimation(nextAnim) {}
};

//map components
struct ChunkComponent {
    static const int CHUNK_SIZE = 32;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    GLuint VAO, VBO, EBO, texture;
    glm::ivec2 chunkPosition;
    bool needsUpdate = true;

    std::array<int, CHUNK_SIZE* CHUNK_SIZE> heights;
    vector<ColliderComponent> collisionBoxes;
};

struct MapComponent {
    static const int RENDER_DISTANCE = 3;
    static const int TOTAL_CHUNKS = (2 * RENDER_DISTANCE + 1) * (2 * RENDER_DISTANCE + 1);
    std::vector<entt::entity> chunkEntities;
    glm::ivec2 currentChunk;
};


//UI
struct UITransformComponent {
    glm::vec2 Position = glm::vec2(0.0f);
    glm::vec2 Size = glm::vec2(100.0f, 5.0f);
};

struct UIButtonComponent {
    std::function<void()> onClick;
    bool isHovered = false;
    glm::vec4 color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    glm::vec4 hoverColor = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
    GameState whenToDraw;
};

struct UITextComponent {
    std::string text;
    glm::vec4 color = glm::vec4(1.0f);
    float fontSize = 16.0f;
    GameState whenToDraw;
};