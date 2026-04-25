#include "Scene.h"
#include "Components.h"
#include "Systems.h"

Scene::Scene() {
    entt::entity squareEntity = m_Registry.create();
    entt::entity camera = m_Registry.create();
    entt::entity animatedEntity = m_Registry.create();
    entt::entity collider = m_Registry.create();

    m_Registry.emplace<CameraComponent>(camera);
    m_Registry.emplace<TransformComponent>(camera, vec3(10.0f, 25.0f, -1.0f));
    m_Registry.emplace<RigidBodyComponent>(camera);
    m_Registry.emplace<ColliderComponent>(camera, vec3(1.f, 3.f, 1.f));

    string path = "res/animations/pollo2.fbx";
    const char* texturePath = "res/animations/textures/Material_0_Diffuse.jpeg";
    auto& animatedModel = m_Registry.emplace<AnimatedModelComponent>(animatedEntity, path, texturePath);
    auto& transform = m_Registry.emplace<TransformComponent>(animatedEntity, vec3(0.0f, 50.0f, -10.0f));
    m_Registry.emplace<RigidBodyComponent>(animatedEntity);

    vec3 size = (animatedModel.max - animatedModel.min) * 0.03f;
    m_Registry.emplace<ColliderComponent>(animatedEntity, size);

    std::vector<vec3> collisionVertices;
    std::vector<unsigned int> collisionIndices;

    RenderableComponent renderable;
    renderable.shader = new myShader::Shader("res/shaders/vertex.glsl", "res/shaders/fragment.glsl");
    renderable.material = new Material("res/textures/brickGround.jpg");
    m_Registry.emplace<TransformComponent>(squareEntity, transform);

    m_Registry.emplace<MeshComponent>(squareEntity, collisionVertices, collisionIndices);
    m_Registry.emplace<RenderableComponent>(squareEntity, renderable);
}

Scene::~Scene() {

}
void Scene::initialize() {
    entt::entity mapEntity = m_Registry.create();
    m_Registry.emplace<MapComponent>(mapEntity);
    MapSystem::initialize(m_Registry);

    
    setUpMainMenu();
    SetUpGameUI();
    setUpPauseMenu();
    UiSystem::initialize(window);
}




void Scene::update(float deltaTime) {

    switch (state)
    {
    case MainMenu:
        glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        UiSystem::update(m_Registry);

        break;
    case PauseMenu:
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
        glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        UiSystem::update(m_Registry);

        break;
    case Game:
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
	    TransformSystem::update(m_Registry, deltaTime);
	    RenderSystem::update(m_Registry);
	    PhysicsSystem::update(m_Registry, deltaTime);
	    CameraSystem::update(m_Registry);
	    InputSystem::update(m_Registry);
        AnimatedModelSystem::update(m_Registry, deltaTime);
        MapSystem::update(m_Registry);
        PathFindingSystem::update(m_Registry, deltaTime);
        break;
    }

}

void Scene::setUpMainMenu()
{
    glm::vec2 buttonSize(windowWidth * 0.4f, windowHeight * 0.1f);
    float buttonSpacing = windowHeight * 0.05f;

    // Game Title
    entt::entity titleEntity = m_Registry.create();
    UITransformComponent titleTransform;
    titleTransform.Size = glm::vec2(buttonSize.x, buttonSize.y * 0.5f);
    titleTransform.Position = glm::vec2(windowWidth / 2 - buttonSize.x / 2, windowHeight / 2 + buttonSize.y + buttonSpacing * 2);

    UITextComponent titleText;
    titleText.text = "Endless Ashfall";
    titleText.color = glm::vec4(1.0f);
    titleText.fontSize = 48.0f;
    titleText.whenToDraw = GameState::MainMenu;

    m_Registry.emplace<UITransformComponent>(titleEntity, titleTransform);
    m_Registry.emplace<UITextComponent>(titleEntity, titleText);

    // Play Button
    entt::entity playButtonEntity = m_Registry.create();
    UITransformComponent playBtnTransform;
    playBtnTransform.Size = buttonSize;
    playBtnTransform.Position = glm::vec2(windowWidth / 2 - buttonSize.x / 2, windowHeight / 2 + buttonSpacing / 2);
    m_Registry.emplace<UITransformComponent>(playButtonEntity, playBtnTransform);

    UIButtonComponent playButton;
    playButton.onClick = []() { state = Game; };
    playButton.whenToDraw = GameState::MainMenu;
    m_Registry.emplace<UIButtonComponent>(playButtonEntity, playButton);

    entt::entity playTextEntity = m_Registry.create();
    UITextComponent playText;
    playText.text = "Play";
    playText.color = glm::vec4(1.0f);
    playText.fontSize = 24.0f;
    playText.whenToDraw = GameState::MainMenu;
    m_Registry.emplace<UITransformComponent>(playTextEntity, playBtnTransform);
    m_Registry.emplace<UITextComponent>(playTextEntity, playText);

    // Exit Button
    entt::entity exitButtonEntity = m_Registry.create();
    UITransformComponent exitBtnTransform;
    exitBtnTransform.Size = buttonSize;
    exitBtnTransform.Position = glm::vec2(windowWidth / 2 - buttonSize.x / 2, windowHeight / 2 - buttonSize.y - buttonSpacing / 2);
    m_Registry.emplace<UITransformComponent>(exitButtonEntity, exitBtnTransform);

    UIButtonComponent exitButton;
    exitButton.onClick = [this]() {glfwSetWindowShouldClose(window, GLFW_TRUE);};
    exitButton.whenToDraw = GameState::MainMenu;
    m_Registry.emplace<UIButtonComponent>(exitButtonEntity, exitButton);

    entt::entity exitTextEntity = m_Registry.create();
    UITextComponent exitText;
    exitText.text = "Exit";
    exitText.color = glm::vec4(1.0f);
    exitText.fontSize = 24.0f;
    exitText.whenToDraw = GameState::MainMenu;
    m_Registry.emplace<UITransformComponent>(exitTextEntity, exitBtnTransform);
    m_Registry.emplace<UITextComponent>(exitTextEntity, exitText);
}


void Scene::setUpPauseMenu()
{
    glm::vec2 buttonSize(windowWidth * 0.4f, windowHeight * 0.1f);
    float buttonSpacing = windowHeight * 0.05f;

    // Pause Menu Title
    entt::entity pauseTitleEntity = m_Registry.create();
    UITransformComponent pauseTitleTransform;
    pauseTitleTransform.Size = glm::vec2(buttonSize.x, buttonSize.y * 0.5f);
    pauseTitleTransform.Position = glm::vec2(windowWidth / 2 - buttonSize.x / 2, windowHeight / 2 + buttonSize.y + buttonSpacing * 2);

    UITextComponent pauseTitle;
    pauseTitle.text = "Paused";
    pauseTitle.color = glm::vec4(1.0f);
    pauseTitle.fontSize = 32.0f;
    pauseTitle.whenToDraw = GameState::PauseMenu;

    m_Registry.emplace<UITransformComponent>(pauseTitleEntity, pauseTitleTransform);
    m_Registry.emplace<UITextComponent>(pauseTitleEntity, pauseTitle);

    // Resume Button
    entt::entity resumeButtonEntity = m_Registry.create();
    UITransformComponent resumeBtnTransform;
    resumeBtnTransform.Size = buttonSize;
    resumeBtnTransform.Position = glm::vec2(windowWidth / 2 - buttonSize.x / 2, windowHeight / 2 + buttonSpacing / 2);

    UIButtonComponent resumeButton;
    resumeButton.onClick = []() { state = Game; };
    resumeButton.whenToDraw = GameState::PauseMenu;

    m_Registry.emplace<UITransformComponent>(resumeButtonEntity, resumeBtnTransform);
    m_Registry.emplace<UIButtonComponent>(resumeButtonEntity, resumeButton);

    entt::entity resumeTextEntity = m_Registry.create();
    UITextComponent resumeText;
    resumeText.text = "Resume";
    resumeText.color = glm::vec4(1.0f);  // White color
    resumeText.fontSize = 24.0f;
    resumeText.whenToDraw = GameState::PauseMenu;

    m_Registry.emplace<UITransformComponent>(resumeTextEntity, resumeBtnTransform);
    m_Registry.emplace<UITextComponent>(resumeTextEntity, resumeText);

    // Main Menu Button
    entt::entity mainMenuButtonEntity = m_Registry.create();
    UITransformComponent mainMenuBtnTransform;
    mainMenuBtnTransform.Size = buttonSize;
    mainMenuBtnTransform.Position = glm::vec2(windowWidth / 2 - buttonSize.x / 2, windowHeight / 2 - buttonSize.y - buttonSpacing / 2);

    UIButtonComponent mainMenuButton;
    mainMenuButton.onClick = []() { state = GameState::MainMenu;};
    mainMenuButton.whenToDraw = GameState::PauseMenu;

    m_Registry.emplace<UITransformComponent>(mainMenuButtonEntity, mainMenuBtnTransform);
    m_Registry.emplace<UIButtonComponent>(mainMenuButtonEntity, mainMenuButton);

    entt::entity mainMenuTextEntity = m_Registry.create();
    UITextComponent mainMenuText;
    mainMenuText.text = "Main Menu";
    mainMenuText.color = glm::vec4(1.0f);
    mainMenuText.fontSize = 24.0f;
    mainMenuText.whenToDraw = GameState::PauseMenu;

    m_Registry.emplace<UITransformComponent>(mainMenuTextEntity, mainMenuBtnTransform);
    m_Registry.emplace<UITextComponent>(mainMenuTextEntity, mainMenuText);
}


void Scene::SetUpGameUI()
{

}


