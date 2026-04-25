#pragma once
#include "Systems.h"
#include <algorithm>
#include <iostream>

static mat4 g_view;
static mat4 g_projection;

//TransformSystem
TransformSystem::TransformSystem() {}
TransformSystem::~TransformSystem() {}

void TransformSystem::update(entt::registry& registry, float deltaTime) {
    auto view = registry.view<TransformComponent, RigidBodyComponent>();
    for (auto entity : view) {
        auto& transform = view.get<TransformComponent>(entity);
        auto& velocity = view.get<RigidBodyComponent>(entity);

    }
}


//PhysicsSystem
PhysicsSystem::PhysicsSystem() {}
PhysicsSystem::~PhysicsSystem() {}

void PhysicsSystem::update(entt::registry& registry, float deltaTime)
{
    const float GRAVITY = -9.81f;
    auto view = registry.view<TransformComponent, RigidBodyComponent, ColliderComponent>();

    for (auto entity : view) {
        auto [transform, rigidbody, collider] = view.get<TransformComponent, RigidBodyComponent, ColliderComponent>(entity);

        if (!rigidbody.IsStatic) {
            rigidbody.Velocity.y += GRAVITY * deltaTime;

            glm::vec3 totalVelocity = rigidbody.HorizontalVelocity + glm::vec3(0, rigidbody.Velocity.y, 0);

            glm::vec3 newPosition = transform.Position + totalVelocity * deltaTime;

            bool collided = checkCollisions(registry, entity, newPosition, collider);

            if (!collided) {
                transform.Position = newPosition;
                if (!collided) {
                    rigidbody.IsGrounded = false;
                }
            }
            else {
                if (rigidbody.Velocity.y < 0) {
                    rigidbody.IsGrounded = true;
                    rigidbody.Velocity.y = 0;
                }
                glm::vec3 slideVelocity = rigidbody.HorizontalVelocity;
                newPosition = transform.Position + slideVelocity * deltaTime;
                if (!checkCollisions(registry, entity, newPosition, collider)) {
                    transform.Position = newPosition;
                }
            }
        }
    }
}


bool PhysicsSystem::checkCollisions(entt::registry& registry, entt::entity entity, const glm::vec3& newPosition, const ColliderComponent& collider)
{
    ColliderComponent entityCollider = collider;
    entityCollider.min = newPosition - collider.Size * 0.5f;
    entityCollider.max = newPosition + collider.Size * 0.5f;

    auto otherEntities = registry.view<TransformComponent, ColliderComponent>();
    for (auto otherEntity : otherEntities) {
        if (otherEntity == entity) continue;

        auto [otherTransform, otherCollider] = otherEntities.get<TransformComponent, ColliderComponent>(otherEntity);
        otherCollider.min = otherTransform.Position - otherCollider.Size * 0.5f;
        otherCollider.max = otherTransform.Position + otherCollider.Size * 0.5f;

        if (entityCollider.Intersects(otherCollider)) {
            if (registry.all_of<CameraComponent>(entity) && registry.all_of<AnimatedModelComponent>(otherEntity)) {
                auto& animatedModel = registry.get<AnimatedModelComponent>(otherEntity);

                if (animatedModel.mesh->getAnimation() != 0) {
                    animatedModel.mesh->setAnimation(0);
                }

                float attackDuration = 1;
                registry.emplace_or_replace<TimerComponent>(otherEntity, attackDuration, 11);
            }

            if (registry.all_of<AnimatedModelComponent>(entity) || registry.all_of<AnimatedModelComponent>(otherEntity)) {
                continue;
            }

            return true;
        }
    }

    return checkTerrainCollision(registry, entityCollider);
}



bool PhysicsSystem::checkTerrainCollision(const entt::registry& registry, const ColliderComponent& entityCollider) {
    auto mapView = registry.view<MapComponent>();
    if (mapView.empty()) {
        return false;
    }

    auto& map = mapView.get<MapComponent>(mapView.front());

    glm::ivec2 entityChunkCoords = {
        static_cast<int>(std::floor(entityCollider.min.x / ChunkComponent::CHUNK_SIZE)),
        static_cast<int>(std::floor(entityCollider.min.z / ChunkComponent::CHUNK_SIZE))
    };

    for (int dx = -1; dx <= 1; dx++) {
        for (int dz = -1; dz <= 1; dz++) {
            glm::ivec2 checkChunkCoords = entityChunkCoords + glm::ivec2(dx, dz);

            auto it = std::find_if(map.chunkEntities.begin(), map.chunkEntities.end(),
                [&registry, &checkChunkCoords](entt::entity chunkEntity) {
                    return registry.get<ChunkComponent>(chunkEntity).chunkPosition == checkChunkCoords;
                });

            if (it != map.chunkEntities.end()) {
                const auto& chunk = registry.get<ChunkComponent>(*it);
                for (const auto& terrainCollider : chunk.collisionBoxes) {
                    if (entityCollider.Intersects(terrainCollider)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}



void PathFindingSystem::update(entt::registry& registry, float deltaTime)
{
    auto view1 = registry.view<CameraComponent, TransformComponent>();
    vec3 objective = vec3(0);
    for (auto entity : view1) {
        objective = view1.get<TransformComponent>(entity).Position;
    }

    auto view = registry.view<AnimatedModelComponent, TransformComponent, RigidBodyComponent>();
    for (auto entity : view) {
        auto& transform = view.get<TransformComponent>(entity);
        auto& animatedModel = view.get<AnimatedModelComponent>(entity);
        auto& rigidBody = view.get<RigidBodyComponent>(entity);

        vec3 direction = glm::normalize(objective - transform.Position);
        float angle = atan2(direction.x, direction.z);
        transform.Rotation = glm::quat(vec3(0, angle, 0));

        float speed = 2.0f;
        vec3 forward = glm::rotate(transform.Rotation, vec3(0, 0, 1));
        rigidBody.HorizontalVelocity = forward * speed;
    }
}
AnimationTimerSystem::AnimationTimerSystem() {}
AnimationTimerSystem::~AnimationTimerSystem() {}
void AnimationTimerSystem::update(entt::registry& registry, float deltaTime)
{
    auto view = registry.view<TimerComponent, AnimatedModelComponent>();
    for (auto entity : view) {
        auto& timer = view.get<TimerComponent>(entity);
        auto& animatedModel = view.get<AnimatedModelComponent>(entity);

        timer.timeRemaining -= deltaTime;

        if (timer.timeRemaining <= 0.0f) {
            animatedModel.mesh->setAnimation(timer.nextAnimation);
            registry.remove<TimerComponent>(entity);
        }
    }
}

//CameraSystem
CameraSystem::CameraSystem() {}
CameraSystem::~CameraSystem() {}

void CameraSystem::update(entt::registry& registry) {
    auto view = registry.view<TransformComponent, CameraComponent>();
    for (auto entity : view) {
        auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

        if (camera.IsActive) {

            camera.Front.x = cos(radians(camera.YAW)) * cos(radians(camera.PITCH));
            camera.Front.y = sin(radians(camera.PITCH));
            camera.Front.z = sin(radians(camera.YAW)) * cos(radians(camera.PITCH));
            camera.Front = normalize(camera.Front);
            
            camera.Right = normalize(cross(camera.Front, camera.WorldUp));
            camera.Up = normalize(cross(camera.Right, camera.Front));

            vec3 cameraPosition = transform.Position;
            vec3 cameraTarget = cameraPosition + camera.Front;
            vec3 upVector = normalize(cross(camera.Right, camera.Front));

            g_view = lookAt(cameraPosition, cameraTarget, upVector);
            g_projection = perspective(radians(camera.FOV), 1920.f / 1080.f, camera.NearPlane, camera.FarPlane);
            break;
        }
    }
}

//InputSystem
double InputSystem::lastX = windowWidth / 2;
double InputSystem::lastY = windowHeight / 2;
bool InputSystem::firstMouse = true;
double InputSystem::mouseXOffset = 0.0;
double InputSystem::mouseYOffset = 0.0;

bool InputSystem::moveForward = false;
bool InputSystem::moveBackward = false;
bool InputSystem::moveLeft = false;
bool InputSystem::moveRight = false;
bool InputSystem::jump = false;

void InputSystem::update(entt::registry& registry)
{
    auto view = registry.view<TransformComponent, CameraComponent, RigidBodyComponent>();
    for (auto entity : view) {
        auto [transform, camera, rigidbody] = view.get<TransformComponent, CameraComponent, RigidBodyComponent>(entity);

        if (camera.IsActive) {
            // Camera rotation
            camera.YAW += mouseXOffset * camera.SENSITIVITY;
            camera.PITCH += mouseYOffset * camera.SENSITIVITY;

            camera.PITCH = glm::clamp(camera.PITCH, -89.0f, 89.0f);

            float speed = 10.f;
            glm::vec3 front(
                cos(glm::radians(camera.YAW)) * cos(glm::radians(camera.PITCH)),
                0,
                sin(glm::radians(camera.YAW)) * cos(glm::radians(camera.PITCH))
            );
            front = glm::normalize(front);
            glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));

            glm::vec3 moveDirection(0.0f);

            if (moveForward) moveDirection += front;
            if (moveBackward) moveDirection -= front;
            if (moveLeft) moveDirection -= right;
            if (moveRight) moveDirection += right;

            if (glm::length(moveDirection) > 0) {
                moveDirection = glm::normalize(moveDirection);
                rigidbody.HorizontalVelocity = moveDirection * speed;
            }
            else {
                rigidbody.HorizontalVelocity = glm::vec3(0.0f);
            }

            if (jump && rigidbody.IsGrounded) {
                rigidbody.Velocity.y = 5.0f;
                rigidbody.IsGrounded = false;
            }
        }
    }

    mouseXOffset = 0.0;
    mouseYOffset = 0.0;
}



void InputSystem::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    mouseXOffset = xpos - lastX;
    mouseYOffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;
    mouseState.x = xpos;
    mouseState.y = ypos;
}

void InputSystem::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            if (mouseState.isReleased) {
                mouseState.isPressed = true;
                mouseState.isReleased = false;
                mouseState.clickComplete = false;
            }
        }
        else if (action == GLFW_RELEASE) {
            if (mouseState.isPressed) {
                mouseState.isPressed = false;
                mouseState.isReleased = true;
                mouseState.clickComplete = true;
            }
        }
    }
}

void InputSystem::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_RELEASE) {
        bool pressed = (action == GLFW_PRESS);
        switch (key) {
        case GLFW_KEY_W: moveForward = pressed; break;
        case GLFW_KEY_S: moveBackward = pressed; break;
        case GLFW_KEY_A: moveLeft = pressed; break;
        case GLFW_KEY_D: moveRight = pressed; break;
        case GLFW_KEY_SPACE: jump = pressed; break;
        case GLFW_KEY_ESCAPE: 
            if (state == Game) {
                state = PauseMenu; 
            }

            break;


        }
    }
}

/**************Map**************/

void ChunkSystem::generate(entt::registry& registry, entt::entity chunkEntity,const std::array<ChunkComponent*, 4>& neighborChunks) {
    auto& chunk = registry.get<ChunkComponent>(chunkEntity);

    chunk.vertices.clear();
    chunk.indices.clear();

    NoiseSystem::generateNoise(registry, chunkEntity);

    const float BLOCK_SIZE = 1.0f;

    for (int x = 0; x < ChunkComponent::CHUNK_SIZE; x++) {
        for (int z = 0; z < ChunkComponent::CHUNK_SIZE; z++) {
            int height = chunk.heights[x + z * ChunkComponent::CHUNK_SIZE];

            glm::vec3 topLeftFront(x * BLOCK_SIZE, height * BLOCK_SIZE, z * BLOCK_SIZE);
            glm::vec3 topRightFront((x + 1) * BLOCK_SIZE, height * BLOCK_SIZE, z * BLOCK_SIZE);
            glm::vec3 topLeftBack(x * BLOCK_SIZE, height * BLOCK_SIZE, (z + 1) * BLOCK_SIZE);
            glm::vec3 topRightBack((x + 1) * BLOCK_SIZE, height * BLOCK_SIZE, (z + 1) * BLOCK_SIZE);

            // Top face
            addFace(chunk, topLeftFront, topLeftBack, topRightBack, topRightFront, glm::vec3(0, 1, 0));

            // Front face
            int frontHeight = (z > 0) ? chunk.heights[x + (z - 1) * ChunkComponent::CHUNK_SIZE]
                : (neighborChunks[0] ? neighborChunks[0]->heights[x + (ChunkComponent::CHUNK_SIZE - 1) * ChunkComponent::CHUNK_SIZE] : 0);
            if (height > frontHeight) {
                glm::vec3 bottomLeftFront(x * BLOCK_SIZE, frontHeight * BLOCK_SIZE, z * BLOCK_SIZE);
                glm::vec3 bottomRightFront((x + 1) * BLOCK_SIZE, frontHeight * BLOCK_SIZE, z * BLOCK_SIZE);
                addFace(chunk, bottomLeftFront, topLeftFront, topRightFront, bottomRightFront, glm::vec3(0, 0, -1));
            }

            // Back face
            int backHeight = (z < ChunkComponent::CHUNK_SIZE - 1) ? chunk.heights[x + (z + 1) * ChunkComponent::CHUNK_SIZE]
                : (neighborChunks[1] ? neighborChunks[1]->heights[x] : 0);
            if (height > backHeight) {
                glm::vec3 bottomLeftBack(x * BLOCK_SIZE, backHeight * BLOCK_SIZE, (z + 1) * BLOCK_SIZE);
                glm::vec3 bottomRightBack((x + 1) * BLOCK_SIZE, backHeight * BLOCK_SIZE, (z + 1) * BLOCK_SIZE);
                addFace(chunk, bottomRightBack, topRightBack, topLeftBack, bottomLeftBack, glm::vec3(0, 0, 1));
            }

            // Left face
            int leftHeight = (x > 0) ? chunk.heights[(x - 1) + z * ChunkComponent::CHUNK_SIZE]
                : (neighborChunks[2] ? neighborChunks[2]->heights[(ChunkComponent::CHUNK_SIZE - 1) + z * ChunkComponent::CHUNK_SIZE] : 0);
            if (height > leftHeight) {
                glm::vec3 bottomLeftBack(x * BLOCK_SIZE, leftHeight * BLOCK_SIZE, (z + 1) * BLOCK_SIZE);
                glm::vec3 bottomLeftFront(x * BLOCK_SIZE, leftHeight * BLOCK_SIZE, z * BLOCK_SIZE);
                addFace(chunk, bottomLeftBack, topLeftBack, topLeftFront, bottomLeftFront, glm::vec3(-1, 0, 0));
            }

            // Right face
            int rightHeight = (x < ChunkComponent::CHUNK_SIZE - 1) ? chunk.heights[(x + 1) + z * ChunkComponent::CHUNK_SIZE]
                : (neighborChunks[3] ? neighborChunks[3]->heights[z * ChunkComponent::CHUNK_SIZE] : 0);
            if (height > rightHeight) {
                glm::vec3 bottomRightBack((x + 1) * BLOCK_SIZE, rightHeight * BLOCK_SIZE, (z + 1) * BLOCK_SIZE);
                glm::vec3 bottomRightFront((x + 1) * BLOCK_SIZE, rightHeight * BLOCK_SIZE, z * BLOCK_SIZE);
                addFace(chunk, bottomRightFront, topRightFront, topRightBack, bottomRightBack, glm::vec3(1, 0, 0));
            }
        }
    }

    generateCollisionBoxes(chunk);
}

void ChunkSystem::addFace(ChunkComponent& chunk, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4, const glm::vec3& normal) {
    unsigned int baseIndex = chunk.vertices.size();

    chunk.vertices.emplace_back(v1, normal, glm::vec2(0, 1));
    chunk.vertices.emplace_back(v2, normal, glm::vec2(0, 0));
    chunk.vertices.emplace_back(v3, normal, glm::vec2(1, 0));
    chunk.vertices.emplace_back(v4, normal, glm::vec2(1, 1));

    chunk.indices.push_back(baseIndex);
    chunk.indices.push_back(baseIndex + 1);
    chunk.indices.push_back(baseIndex + 2);
    chunk.indices.push_back(baseIndex);
    chunk.indices.push_back(baseIndex + 2);
    chunk.indices.push_back(baseIndex + 3);
}

void ChunkSystem::generateCollisionBoxes(ChunkComponent& chunk) {
    chunk.collisionBoxes.clear();
    const float BLOCK_SIZE = 1.0f;

    for (int x = 0; x < ChunkComponent::CHUNK_SIZE; x++) {
        for (int z = 0; z < ChunkComponent::CHUNK_SIZE; z++) {
            int height = chunk.heights[x + z * ChunkComponent::CHUNK_SIZE];
            glm::vec3 min(x * BLOCK_SIZE + chunk.chunkPosition.x * ChunkComponent::CHUNK_SIZE * BLOCK_SIZE,
                0,
                z * BLOCK_SIZE + chunk.chunkPosition.y * ChunkComponent::CHUNK_SIZE * BLOCK_SIZE);
            glm::vec3 max((x + 1) * BLOCK_SIZE + chunk.chunkPosition.x * ChunkComponent::CHUNK_SIZE * BLOCK_SIZE,
                height * BLOCK_SIZE,
                (z + 1) * BLOCK_SIZE + chunk.chunkPosition.y * ChunkComponent::CHUNK_SIZE * BLOCK_SIZE);
            chunk.collisionBoxes.emplace_back(min, max);
        }
    }
}


void ChunkSystem::setupBuffers(entt::registry& registry, entt::entity chunkEntity) {
    auto& chunk = registry.get<ChunkComponent>(chunkEntity);

    if (chunk.VAO == 0) {
        glGenVertexArrays(1, &chunk.VAO);
    }
    glBindVertexArray(chunk.VAO);

    if (chunk.VBO == 0) {
        glGenBuffers(1, &chunk.VBO);
    }
    glBindBuffer(GL_ARRAY_BUFFER, chunk.VBO);
    glBufferData(GL_ARRAY_BUFFER, chunk.vertices.size() * sizeof(Vertex), chunk.vertices.data(), GL_STATIC_DRAW);

    if (chunk.EBO == 0) {
        glGenBuffers(1, &chunk.EBO);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunk.indices.size() * sizeof(unsigned int), chunk.indices.data(), GL_STATIC_DRAW);
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    // UV
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void ChunkSystem::draw(entt::registry& registry, entt::entity chunkEntity) {
    auto& chunk = registry.get<ChunkComponent>(chunkEntity);

    glBindVertexArray(chunk.VAO);
    glDrawElements(GL_TRIANGLES, chunk.indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

GLuint MapSystem::terrainTexture = 0;
myShader::Shader* MapSystem::shader = nullptr;

std::unique_ptr<Material> MapSystem::material = nullptr;
//Material MapSystem::material;
void MapSystem::initialize(entt::registry& registry) {
    shader = new myShader::Shader("res/shaders/map.vs", "res/shaders/map.frs");
    //unsigned char checkerboard[4 * 4 * 4] = {
    //    255, 0, 0, 255,   0, 255, 0, 255,   255, 0, 0, 255,   0, 255, 0, 255,
    //    0, 255, 0, 255,   255, 0, 0, 255,   0, 255, 0, 255,   255, 0, 0, 255,
    //    255, 0, 0, 255,   0, 255, 0, 255,   255, 0, 0, 255,   0, 255, 0, 255,
    //    0, 255, 0, 255,   255, 0, 0, 255,   0, 255, 0, 255,   255, 0, 0, 255
    //};
    //
    //glGenTextures(1, &terrainTexture);
    //glGenTextures(1, &terrainTexture);
    //glBindTexture(GL_TEXTURE_2D, terrainTexture);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerboard);
    
    material = std::make_unique<Material>("res/textures/brickGround2.png");
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    auto view = registry.view<MapComponent>();
    for (auto entity : view) {
        auto& map = view.get<MapComponent>(entity);

        for (int x = -MapComponent::RENDER_DISTANCE; x <= MapComponent::RENDER_DISTANCE; ++x) {
            for (int z = -MapComponent::RENDER_DISTANCE; z <= MapComponent::RENDER_DISTANCE; ++z) {
                auto chunkEntity = registry.create();
                auto& chunk = registry.emplace<ChunkComponent>(chunkEntity);
                chunk.chunkPosition = glm::ivec2(x, z);
                chunk.needsUpdate = true;
                map.chunkEntities.push_back(chunkEntity);
            }
        }

        map.currentChunk = { 0, 0 };
    }
}

void MapSystem::update(entt::registry& registry) {
    auto view = registry.view<MapComponent>();

    auto playerView = registry.view<CameraComponent, TransformComponent>();
    auto playerEntity = playerView.front();
    const auto& playerTransform = registry.get<TransformComponent>(playerEntity);

    for (auto entity : view) {
        auto& map = view.get<MapComponent>(entity);

        glm::ivec2 newCurrentChunk = {
            static_cast<int>(std::floor(playerTransform.Position.x / ChunkComponent::CHUNK_SIZE)),
            static_cast<int>(std::floor(playerTransform.Position.z / ChunkComponent::CHUNK_SIZE))
        };

        if (map.currentChunk != newCurrentChunk) {
            updateChunkPositions(registry, newCurrentChunk);
            map.currentChunk = newCurrentChunk;
        }

        shader->use();
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, terrainTexture);
        //glUniform1i(glGetUniformLocation(shader->program, "terrainTexture"), 1);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material->texture);
        glUniform1i(glGetUniformLocation(shader->program, "terrainTexture"), 0);

        shader->setMat4("view", g_view);
        shader->setMat4("projection", g_projection);

        // Update and draw chunks
        for (auto chunkEntity : map.chunkEntities) {
            auto& chunk = registry.get<ChunkComponent>(chunkEntity);
            if (chunk.needsUpdate) {
                std::array<ChunkComponent*, 4> neighborChunks = getNeighborChunks(registry, map, chunk.chunkPosition);
                ChunkSystem::generate(registry, chunkEntity, neighborChunks);
                ChunkSystem::setupBuffers(registry, chunkEntity);
                chunk.needsUpdate = false;
            }
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(chunk.chunkPosition.x * ChunkComponent::CHUNK_SIZE, 0, chunk.chunkPosition.y * ChunkComponent::CHUNK_SIZE));
            shader->setMat4("model", model);
            ChunkSystem::draw(registry, chunkEntity);
        }

    }
}

std::array<ChunkComponent*, 4> MapSystem::getNeighborChunks(entt::registry& registry, MapComponent& map, const glm::ivec2& chunkPos) {
    std::array<ChunkComponent*, 4> neighbors = { nullptr, nullptr, nullptr, nullptr };

    const glm::ivec2 offsets[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} }; // Front, Back, Left, Right

    for (int i = 0; i < 4; ++i) {
        glm::ivec2 neighborPos = chunkPos + offsets[i];
        for (auto& chunkEntity : map.chunkEntities) {
            auto& chunk = registry.get<ChunkComponent>(chunkEntity);
            if (chunk.chunkPosition == neighborPos) {
                neighbors[i] = &chunk;
                break;
            }
        }
    }

    return neighbors;
}

void MapSystem::updateChunkPositions(entt::registry& registry, const glm::ivec2& newCurrentChunk) {
    auto view = registry.view<MapComponent>();
    for (auto entity : view) {
        auto& map = view.get<MapComponent>(entity);

        std::unordered_set<glm::ivec2> newPositions;
        for (int x = -MapComponent::RENDER_DISTANCE; x <= MapComponent::RENDER_DISTANCE; ++x) {
            for (int z = -MapComponent::RENDER_DISTANCE; z <= MapComponent::RENDER_DISTANCE; ++z) {
                newPositions.insert(newCurrentChunk + glm::ivec2(x, z));
            }
        }

        for (auto& chunkEntity : map.chunkEntities) {
            auto& chunk = registry.get<ChunkComponent>(chunkEntity);
            if (newPositions.find(chunk.chunkPosition) == newPositions.end()) {
                for (const auto& newPos : newPositions) {
                    if (std::none_of(map.chunkEntities.begin(), map.chunkEntities.end(),
                        [&](const auto& e) { return registry.get<ChunkComponent>(e).chunkPosition == newPos; })) {
                        chunk.chunkPosition = newPos;
                        chunk.needsUpdate = true;
                        break;
                    }
                }
            }
            newPositions.erase(chunk.chunkPosition);
        }
    }
}


void NoiseSystem::generateNoise(entt::registry& registry, entt::entity chunkEntity) {
    auto& chunk = registry.get<ChunkComponent>(chunkEntity);

    FastNoiseLite fastNoise;
    fastNoise.SetSeed(1);
    fastNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    for (int x = 0; x < ChunkComponent::CHUNK_SIZE; x++) {
        for (int z = 0; z < ChunkComponent::CHUNK_SIZE; z++) {
            float globalX = (chunk.chunkPosition.x * ChunkComponent::CHUNK_SIZE) + x;
            float globalZ = (chunk.chunkPosition.y * ChunkComponent::CHUNK_SIZE) + z;

            float noiseValue = fastNoise.GetNoise(globalX, globalZ);

            int height = (int)std::floor(((noiseValue + 1) / 2) * 30);
            chunk.heights[x + z * ChunkComponent::CHUNK_SIZE] = height;
        }
    }
}
//RenderSystem
RenderSystem::RenderSystem() {}
RenderSystem::~RenderSystem() {}

void RenderSystem::update(entt::registry& registry) {
    auto view = registry.view<TransformComponent, MeshComponent, RenderableComponent>();
    for (auto entity : view) {
        auto [transform, mesh, renderable] = view.get<TransformComponent, MeshComponent, RenderableComponent>(entity);

        renderable.shader->use();
        glBindTexture(GL_TEXTURE_2D, renderable.material->texture);

        //Uniforms
        mat4 model = mat4(1.0f);
        model = translate(model, transform.Position);

        renderable.shader->setMat4("model", model);
        renderable.shader->setMat4("view", g_view);
        renderable.shader->setMat4("projection", g_projection);
        

        //Draw
        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
    }
}

//AnimatedModelSystem
AnimatedModelSystem::AnimatedModelSystem() {}
AnimatedModelSystem::~AnimatedModelSystem() {}

void AnimatedModelSystem::update(entt::registry& registry, float deltaTime) {
    auto view = registry.view<AnimatedModelComponent, TransformComponent>();
    for (auto entity : view) {
        auto& animatedModel = view.get<AnimatedModelComponent>(entity);
        auto& transform = view.get<TransformComponent>(entity);

        vector <mat4> Transforms;
        animatedModel.mesh->GetBoneTransforms(deltaTime,Transforms);
        animatedModel.shader->use();
        for (unsigned int i = 0; i < Transforms.size(); i++) {
            animatedModel.shader->setMat4("gBones[" + to_string(i) + "]", Transforms[i]);
        }

        // Texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, animatedModel.material->texture);
        animatedModel.shader->setInt("texture_diffuse1", 0);


        mat4 model = mat4(1.0f);
        model = translate(model, transform.Position - ((animatedModel.max * 0.03f - animatedModel.min* 0.03f ) / 2.f)); 

        model = model * glm::mat4_cast(transform.Rotation);

        model = scale(model, vec3(0.03f, 0.03f, 0.03f));

        animatedModel.shader->setMat4("model", model);

        animatedModel.shader->setMat4("view", g_view);
        animatedModel.shader->setMat4("projection", g_projection);

        animatedModel.mesh->Render();
    }
}


//UI
GLFWwindow* UiSystem::s_Window = nullptr;
myShader::Shader *UiSystem::btnShader, *UiSystem::textShader;
GLuint UiSystem::s_VAO = 0;
GLuint UiSystem::s_VBO = 0;

map<GLchar, Character> UiSystem::Characters;
GLuint UiSystem::s_TextVAO, UiSystem::s_TextVBO;
bool UiSystem::wasMousePressed = false;

