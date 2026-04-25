#pragma once
#include "../Core/config.h"
#include "Components.h"
#include "../Graphics/Material.h"
#include "../Graphics/Shader.h"


class TransformSystem
{
public:
	TransformSystem();
	~TransformSystem();
	static void update(entt::registry& registry, float deltaTime);
private:

};

class RenderSystem
{
public:
	RenderSystem();
	~RenderSystem();
	static void update(entt::registry& registry);
private:

};


class PhysicsSystem
{
public:
	PhysicsSystem();
	~PhysicsSystem();
	static void update(entt::registry& registry, float deltaTime);
	static bool checkWallCollision(entt::registry& registry, entt::entity entity, const glm::vec3& position, const ColliderComponent& collider);
private:
	static bool checkCollisions(entt::registry& registry, entt::entity entity, const vec3& newPosition, const ColliderComponent& collider);
	static bool checkTerrainCollision(const entt::registry& registry, const ColliderComponent& entityCollider);
};

class AnimationTimerSystem
{
	AnimationTimerSystem();
	~AnimationTimerSystem();
	static void update(entt::registry& registry, float deltaTime);
};
class CameraSystem
{
public:
	CameraSystem();
	~CameraSystem();
	static void update(entt::registry& registry);
private:
};

class InputSystem
{
public:
	InputSystem() = delete;
	static void update(entt::registry& registry);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static double lastX;
	static double lastY;
	static bool firstMouse;
	static double mouseXOffset;
	static double mouseYOffset;

	static bool moveForward;
	static bool moveBackward;
	static bool moveLeft;
	static bool moveRight;
	static bool jump;
private:

};

class AnimatedModelSystem {
public:
    AnimatedModelSystem();
    ~AnimatedModelSystem();
	static void update(entt::registry& registry, float deltaTime);
	void setBoneTransform(string name, mat4 Transform);
};

class PathFindingSystem {
public:
	PathFindingSystem();
	~PathFindingSystem();
	static void update(entt::registry& registry, float deltaTime);
};

/***Map***/
class ChunkSystem {
public:
	static void generate(entt::registry& registry, entt::entity chunkEntity);
	static void setupBuffers(entt::registry& registry, entt::entity chunkEntity);
	static void draw(entt::registry& registry, entt::entity chunkEntity);

	static void generate(entt::registry& registry, entt::entity chunkEntity, const std::array<ChunkComponent*, 4>& neighborChunks);
	static void addFace(ChunkComponent& chunk, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4, const glm::vec3& normal);
	static void generateCollisionBoxes(ChunkComponent& chunk);

};

class MapSystem {
public:
	static void initialize(entt::registry& registry);
	static void update(entt::registry& registry);
	static std::array<ChunkComponent*, 4> getNeighborChunks(entt::registry& registry, MapComponent& map, const glm::ivec2& chunkPos);
    
private:
	static void updateChunkPositions(entt::registry& registry, const glm::ivec2& newCurrentChunk);
	static myShader::Shader* shader;
    static std::unique_ptr<Material> material;
	static GLuint terrainTexture;
};

class NoiseSystem {
public:
	static void generateNoise(entt::registry& registry, entt::entity chunkEntity);
};

struct Character {
    GLuint TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    GLuint Advance;
};

class UiSystem {
public:
    static void initialize(GLFWwindow* window) {
        s_Window = window;
        initializeShaders();
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(windowWidth), 0.0f, static_cast<float>(windowHeight), -1.0f, 1.0f);

        btnShader->use();
        glUniformMatrix4fv(glGetUniformLocation(btnShader->program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        textShader->use();
        glUniformMatrix4fv(glGetUniformLocation(textShader->program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        initializeTextRenderer();
        initializeBuffers();

    }

    static void update(entt::registry& registry) {

        glUseProgram(btnShader->program);
        renderUI(registry);
        updateButtons(registry);
    }

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(windowWidth), 0.0f, static_cast<float>(windowHeight), -1.0f, 1.0f);
    btnShader->use();
    glUniformMatrix4fv(glGetUniformLocation(btnShader->program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));  
    textShader->use();
    glUniformMatrix4fv(glGetUniformLocation(textShader->program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}


private:
    static GLFWwindow* s_Window;
    static myShader::Shader *btnShader, *textShader;
    static GLuint s_VAO, s_VBO;

    static std::map<GLchar, Character> Characters;
    static GLuint s_TextVAO, s_TextVBO;
    static bool wasMousePressed;


    

    static void initializeShaders() {
        btnShader = new myShader::Shader("res/shaders/btn.vs", "res/shaders/btn.frs");
        textShader = new myShader::Shader("res/shaders/text.vs", "res/shaders/text.frs");
    }

    static void initializeTextRenderer() {
        FT_Library ft;
        if (FT_Init_FreeType(&ft))
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

        FT_Face face;
        if (FT_New_Face(ft, "res/text.otf", 0, &face))
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

        FT_Set_Pixel_Sizes(face, 0, 48);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (unsigned char c = 0; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        glGenVertexArrays(1, &s_TextVAO);
        glGenBuffers(1, &s_TextVBO);
        glBindVertexArray(s_TextVAO);
        glBindBuffer(GL_ARRAY_BUFFER, s_TextVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    static void updateButtons(entt::registry& registry) {
        auto view = registry.view<UITransformComponent, UIButtonComponent>();

        double mouseY = windowHeight - mouseState.y;

        bool handledClick = false;  

        for (auto entity : view) {
            auto& button = view.get<UIButtonComponent>(entity);

            if (button.whenToDraw != state)
                continue;

            auto& transform = view.get<UITransformComponent>(entity);

            bool hovered = mouseState.x >= transform.Position.x &&
                mouseState.x <= transform.Position.x + transform.Size.x &&
                mouseY >= transform.Position.y &&
                mouseY <= transform.Position.y + transform.Size.y;

            button.isHovered = hovered;

            if (!handledClick && hovered && mouseState.clickComplete) {
                if (button.onClick) {
                    button.onClick();
                    handledClick = true;
                }
            }
        }

        mouseState.clickComplete = false;
    }



    static void renderUI(entt::registry& registry) {
        glUseProgram(btnShader->program);

        auto buttonView = registry.view<UITransformComponent, UIButtonComponent>();
        for (auto entity : buttonView) {
            
            auto& button = buttonView.get<UIButtonComponent>(entity);
            if (button.whenToDraw != state)
                continue;
            auto& transform = buttonView.get<UITransformComponent>(entity);
            renderButton(transform, button);
        }

        auto textView = registry.view<UITransformComponent, UITextComponent>();
        for (auto entity : textView) {
            auto& text = textView.get<UITextComponent>(entity);
            if (text.whenToDraw != state)
                continue;
            auto& transform = textView.get<UITransformComponent>(entity);
            renderText(transform, text);
        }
    }
    static void initializeBuffers() {
        glGenVertexArrays(1, &s_VAO);
        glGenBuffers(1, &s_VBO);

        glBindVertexArray(s_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, s_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, nullptr, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }


    static void renderButton(const UITransformComponent& transform, const UIButtonComponent& button) {
        float vertices[] = {
            transform.Position.x, transform.Position.y,
            transform.Position.x + transform.Size.x, transform.Position.y,
            transform.Position.x + transform.Size.x, transform.Position.y + transform.Size.y,
            transform.Position.x, transform.Position.y + transform.Size.y
        };

        glBindVertexArray(s_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, s_VBO);

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glm::vec4 color = button.isHovered ? button.hoverColor : button.color;
        GLint colorLoc = glGetUniformLocation(btnShader->program, "uColor");
        glUniform4fv(colorLoc, 1, glm::value_ptr(color));

        btnShader->use();
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glBindVertexArray(0);
    }

    static void renderText(const UITransformComponent& transform, const UITextComponent& text) {
        textShader->use();

        float scale = text.fontSize / 36.0f;

        float textWidth = 0.0f;
        float maxHeight = 0.0f;
        for (const char& c : text.text) {
            Character ch = Characters[c];
            textWidth += (ch.Advance >> 6) * scale; 
            float height = ch.Size.y * scale;       
            if (height > maxHeight) {
                maxHeight = height;                 
            }
        }

        glm::vec2 buttonCenter(
            transform.Position.x + transform.Size.x / 2.0f,
            transform.Position.y + transform.Size.y / 2.0f
        );

        glm::vec2 textPosition(
            buttonCenter.x - textWidth / 2.0f,                   
            buttonCenter.y - maxHeight / 2.0f                    
        );

        glm::vec2 pos = textPosition;

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(s_TextVAO);

        for (char c : text.text) {
            Character ch = Characters[c];

            float xpos = pos.x + ch.Bearing.x * scale;
            float ypos = pos.y - (ch.Size.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;

            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };

            glBindTexture(GL_TEXTURE_2D, ch.TextureID);

            glBindBuffer(GL_ARRAY_BUFFER, s_TextVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            pos.x += (ch.Advance >> 6) * scale;
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

};

