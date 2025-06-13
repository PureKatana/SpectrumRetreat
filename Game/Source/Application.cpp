#include "Application.h"
#include "Audio_Manager.h"

Application::Application(int width, int height)
    : m_width(width)
    , m_height(height)
    , m_camera(
        Vector3(0.0f, 0.0f, 10.0f),   // eye
        Vector3(0.0f, 0.0f, 0.0f),   // center
        Vector3(0.0f, 1.0f, 0.0f),   // up
        60.0f * (3.14159265f / 180.0f),
        static_cast<float>(width) / height,
        0.1f,
        100.0f
    )
    , m_player()
    
    
{
	m_player.setCamera(&m_camera);

    m_player.reset();
}

Application::~Application()
{
    shutdown();
}

/// Initialize GLFW and create a window
bool Application::init()
{
    if (!glfwInit()) return false;
    m_window = glfwCreateWindow(m_width, m_height, "Test", nullptr, nullptr);
    if (!m_window) return false;
    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return false;

	// ImGui setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // capture mouse
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (!loadResources())
        return false;

    m_lastFrame = static_cast<float>(glfwGetTime());

    return true;
}

/// Load resources: textures, shaders, meshes
bool Application::loadResources()
{
    //TEXTURES

    if (!loadTexture("transparent_gray_color", "../../Assets/Textures/Solid_gray.png")) 
        return false;
    m_resourceManager.get<Texture>("transparent_gray_color")->setOpacity(0.3);
    if (!loadTexture("gray_color", "../../Assets/Textures/Solid_gray.png"))
        return false;
    if (!loadTexture("invisible", "../../Assets/Textures/Solid_gray.png"))
        return false;
    m_resourceManager.get<Texture>("invisible")->setOpacity(0);
    if (!loadTexture("dragon_stamp", "../../Assets/Textures/dragon_stamp.jpg"))
        return false;
    if (!loadTexture("red_color", "../../Assets/Textures/red_solid.jpg"))
        return false;
    if (!loadTexture("transparent_red_color", "../../Assets/Textures/red_solid.jpg"))
        return false;
    m_resourceManager.get<Texture>("transparent_red_color")->setOpacity(0.3);
    if (!loadTexture("blue_color", "../../Assets/Textures/blue_solid.jpg"))
        return false;
    if (!loadTexture("transparent_blue_color", "../../Assets/Textures/blue_solid.jpg"))
        return false;
    m_resourceManager.get<Texture>("transparent_blue_color")->setOpacity(0.3);
    if (!loadTexture("jade_color", "../../Assets/Textures/jade_2.jpg"))
        return false;
    if (!loadTexture("yellow_color", "../../Assets/Textures/yellow_solid.jpg"))
        return false;

    if (!loadTexture("transparent_yellow_color", "../../Assets/Textures/yellow_solid.jpg"))
        return false;
    m_resourceManager.get<Texture>("transparent_yellow_color")->setOpacity(0.3);

    //SHADERS

    if (!loadShader("LightShader", "../../Assets/Shaders/LightsVert.glsl", "../../Assets/Shaders/LightsFrag.glsl")) 
        return false;

    //MESHES

    if (!loadMesh(nullptr, "floor", "../../Assets/Meshes/floor.obj", "gray_color"))
        return false;
    if (!loadMesh(nullptr, "wall", "../../Assets/Meshes/wall.obj", "gray_color"))
        return false;
    if (!loadMesh(nullptr, "cube", "../../Assets/Meshes/rounded_cube.obj", "red_color"))
        return false;
    if (!loadMesh(nullptr, "door", "../../Assets/Meshes/Door.obj", "red_color"))
        return false;
    if (!loadMesh(nullptr, "oCube", "../../Assets/Meshes/1x1x1cube.obj", "gray_color"))
        return false;
    //if (!loadMesh(nullptr, "dragon", "../../Assets/Meshes/dragon.obj", "jade_color"))
      //  return false;

    if (!Mesh::loadInstances( "../../Assets/Levels/LevelTest.txt", m_LevelMeshes, m_resourceManager))
        return false;

    Audio_Manager::getInstance() -> playSound("../../Assets/Sounds/Music/funny_background_music.wav", true);
    
    createLights();
    createLevel();

    return true;
}

// Load a texture from file
bool Application::loadTexture(const std::string& name, const std::string& path)
{
    auto* texture = m_resourceManager.create<Texture>(name);
    if (!texture -> loadTexture(path)) 
    {
        std::cerr << "Texture load failed: " << path << "\n";
        return false;
    }
    return true;
}

// Load a shader from file
bool Application::loadShader(const std::string& name, const std::string& vert, const std::string& frag)
{
    auto* shader = m_resourceManager.create<Shader>(name);
    if (!shader -> setVertexShader(vert) || !shader->setFragmentShader(frag) || !shader->link()) 
    {
        std::cerr << "Shader load failed: " << vert << " / " << frag << "\n";
        return false;
    }
    return true;
}
bool Application::loadMesh(
    Mesh** meshPtr,
    const std::string& modelName,
    const std::string& objPath,
    const std::string& textureName
)
{
    // 1) create & load the Model
    auto* model = m_resourceManager.create<Model>(modelName);
    if (!model -> loadFromOBJ(objPath)) {
        std::cerr << "Model load failed: " << objPath << "\n";
        return false;
    }
    model -> uploadToGPU();

    // 2) create the Mesh, give it its texture
    if (meshPtr)
    {
        *meshPtr = new Mesh(model);
        Texture* tex = m_resourceManager.get<Texture>(textureName);
        (*meshPtr) -> setTexture(tex);
    }
    
    return true;
}
// Create lights
bool Application::createLights()
{
    if (!LightInstance::LoadInstances(
        "../../Assets/Levels/lights.txt",
        m_resourceManager,
        m_dirLights,
        m_pointLights,
        m_spotLights))
    {
        std::cerr << "Failed to load lights\n";
        return false;
    }
}
// Create level
void Application::createLevel()
{
    for (const auto& [key, mesh] : m_LevelMeshes)
    {
        auto collider = Collider::createFromMesh(ColliderType::BOX, mesh);
        ColorState color;
        GameObjectType type;

        //Determine type and color based on key
        if (key >= 0 && key <= 3)
        {
			// 0-3: Doors : Inactive, Red, Blue, Yellow
            type = GameObjectType::DOOR;
            color = static_cast<ColorState>(key % 4);
        }
        else if (key >= 4 && key <= 7)
        {
			// 4-7: Buttons : Inactive, Red, Blue, Yellow
            type = GameObjectType::BUTTON;
            color = static_cast<ColorState>(key % 4);
        }
        else if (key >= 8 && key <= 11)
        {
			// 8-11: Moving Objects : Inactive, Red, Blue, Yellow
            type = GameObjectType::MOVING_OBJECT;
			color = static_cast<ColorState>(key % 4);
        }
        else if (key == 12)
        {
			// 12: Obstacle
            type = GameObjectType::OBSTACLE;
            color = ColorState::E_INACTIVE;
        }
        else if (key == 13)
        {
			// 13: End Point
            type = GameObjectType::END_POINT;
            color = ColorState::E_INACTIVE;
        }
        else if (key == 14)
        {
			// 14: Death Zone
            type = GameObjectType::DEATH_ZONE;
			color = ColorState::E_INACTIVE;
        }
        else
        {
            continue; // skip unknown keys
        }

        auto newGameObject = new GameObject(mesh, std::move(collider), color, type, m_resourceManager);

        // Add collider to m_colliders if it exists
        if (newGameObject->m_collider)
        {
            m_colliders.push_back(newGameObject->m_collider.get());
        }

        // Move the newly created GameObject into the vector
        m_gameObjects.push_back(newGameObject);
    }
}
// Run the application
void Application::run()
{
    float rotation = 0.0f;
    while (!glfwWindowShouldClose(m_window))
    {
        // Poll events first to ensure ImGui gets fresh input
        glfwPollEvents();

        // deltaTime calculation
        float now = static_cast<float>(glfwGetTime());
        float deltaTime = now - m_lastFrame;
        m_lastFrame = now;

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Process application-level input (like pausing)
        processInput(deltaTime); // This now only toggles m_isRunning and cursor

        if (m_isRunning)
        {
            // Game logic and rendering when running
            m_player.handleInput(m_window, deltaTime, m_colliders);
            handleCollisions(deltaTime);
            m_player.updatePosition(deltaTime);
            updateMovingGameObjects(deltaTime);
            render(); 
            m_uiManager.drawPhone(m_player.getPhone());
            m_uiManager.drawCursor(5);
        }
		else if (!m_isRunning && m_gameWin) // Game is won, draw the win menu
        {
            m_uiManager.drawMenu(m_window, [this]() {
                resetGame();
                }, "YOU WIN!");
        }
        else // Game is paused, draw the menu
        {
            m_uiManager.drawMenu(m_window, [this]() {
                resetGame();
            }, "Pause Menu");
        }

        // Render ImGui stuff
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers
        glfwSwapBuffers(m_window);
    }
}

void    Application::handleCollisions(float deltaTime)
{
    LibMath::Vector3 collisionNormal;
    m_player.m_grounded = false;
    std::unique_ptr<Collider> steppedCollider;

    for (auto& gameObject : m_gameObjects)
    {
        if (gameObject -> m_collider && m_player.getCollider())
        {
            LibMath::Vector3 steppedPosition = m_player.getPosition();
            steppedPosition += (m_player.getVelocity() * deltaTime);

            LibMath::Point3D p1(steppedPosition.m_x, steppedPosition.m_y, steppedPosition.m_z);
            LibMath::Point3D p2(steppedPosition.m_x, steppedPosition.m_y + m_player.m_height, steppedPosition.m_z);

            steppedCollider = Collider::createCapsuleManualSet(p1, p2, m_player.m_radius);

            if (gameObject -> m_collider -> checkCollision(*steppedCollider, collisionNormal))
            {
                if (gameObject -> m_type == GameObjectType::DEATH_ZONE)
                {
                    resetGame();
                }

                if (gameObject -> m_type == GameObjectType::END_POINT)
                {
					winGame(); // Player reached the end point
                }

                // Disable Collider if Player has same ColorState as GameObject and is a Door
                if (gameObject -> m_colorState == m_player.getPhone().getState() && gameObject -> m_type == GameObjectType::DOOR)
                {
					continue; // skip this collider
                }
                Vector3 velocity = m_player.getVelocity();
                velocity.projectOnto(collisionNormal);
                if (collisionNormal.dot(Vector3(0, 1, 0)) >= 0.9f)
                {
                    m_player.m_grounded = true;
                }
                if (velocity.dot(collisionNormal) < 0)
                {
                    m_player.AddVelocity(-velocity);
                }
            }
        }
	}
}

void Application::updateMovingGameObjects(float deltaTime)
{
    for (auto& gameObject : m_gameObjects)
    {
        if (gameObject -> m_type == GameObjectType::MOVING_OBJECT)
        {
            // Remove old collider pointer from m_colliders
            auto it = std::find(m_colliders.begin(), m_colliders.end(), gameObject -> m_collider.get());
            if (it != m_colliders.end())
                m_colliders.erase(it);

            // Update transform (which updates the collider)
            gameObject -> updateTransform(deltaTime, LibMath::Matrix4::createTransform(
                LibMath::Vector3(0, -1, -30),
                LibMath::Radian(0),
				LibMath::Vector3(1, 1, 1))); 

            // Add new collider pointer to m_colliders
            if (gameObject -> m_collider)
                m_colliders.push_back(gameObject -> m_collider.get());
        }
    }
}

// Process input
void Application::processInput(float deltaTime)
{
    // Handle Escape key press/release for toggling pause
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) 
    {
        m_escPressedLastFrame = false;
    }

    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
    {
        if (!m_escPressedLastFrame) 
        {
            m_isRunning = !m_isRunning; // Toggle the game state

            // Set cursor mode based on game state
            if (m_isRunning) 
            {
                glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else 
            {
                glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }
        m_escPressedLastFrame = true; // Mark that Escape was pressed this frame
    }
}

// Render the scene
void Application::render()
{
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto* shader = m_resourceManager.get<Shader>("LightShader");
    shader -> use();

    GLuint pid = shader->getID();
    static GLint locViewPos = glGetUniformLocation(pid, "uViewPos");
    static GLint locMatDiffuse = glGetUniformLocation(pid, "uMatDiffuse");
    static GLint locMatSpecular = glGetUniformLocation(pid, "uMatSpecular");
    static GLint locMatShininess = glGetUniformLocation(pid, "uMatShininess");
    static GLint locNumDir = glGetUniformLocation(pid, "numDir");
    static GLint locNumPoint = glGetUniformLocation(pid, "numPoint");
    static GLint locNumSpot = glGetUniformLocation(pid, "numSpot");

    // Camera pos
    Vector3 camPos = m_camera.getPosition();
    glUniform3fv(locViewPos, 1, &camPos.m_x);

    // Material
    glUniform4f(locMatDiffuse, 1, 1, 1, 1);
    glUniform4f(locMatSpecular, 1, 1, 1, 1);
    glUniform1f(locMatShininess, 64.0f);

    // Upload light counts
    glUniform1i(locNumDir, static_cast<int>(m_dirLights.size()));
    glUniform1i(locNumPoint, static_cast<int>(m_pointLights.size()));
    glUniform1i(locNumSpot, static_cast<int>(m_spotLights.size()));

    // Upload each light
    int i = 0;
    for (auto* light : m_dirLights) light->uploadUniforms(*shader, i++);
    i = 0;
    for (auto* light : m_pointLights) light->uploadUniforms(*shader, i++);
    i = 0;
    for (auto* light : m_spotLights) light->uploadUniforms(*shader, i++);

    // draw meshes
    Matrix4 viewProj = m_camera.getProjectionMatrix() * m_camera.getViewMatrix();
    //drawSceneGraph(m_sceneRoot.get(), shader, viewProj);
    std::vector<GameObject*> transparentList;

    glDepthMask(GL_TRUE);              // enable depth writes
    glDisable(GL_BLEND);
    for (auto& gameObject : m_gameObjects)
    {
        if (gameObject -> m_mesh)
        {
            if (gameObject -> m_type == GameObjectType::DOOR || gameObject -> m_type == GameObjectType::DEATH_ZONE)
            {
                transparentList.push_back(gameObject);
            }
            else
            {
                gameObject->m_mesh->draw(shader, viewProj);
            }

        }
	}
    
    glDepthMask(GL_FALSE);             // **disable** depth writes
    glEnable(GL_BLEND);
    for (auto& gameObject : transparentList)
    {
        gameObject->m_mesh->draw(shader, viewProj);;
    }
    glDepthMask(GL_TRUE);
}
void Application::winGame()
{
    m_isRunning = false; // Stop the game
    m_gameWin = true; // Set game win state
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
// Shutdown the application
void Application::shutdown()
{
    // Delete dynamically allocated Light Instances
    for (auto* L : m_dirLights)
    {
        delete L;
    }
    m_dirLights.clear(); 

    for (auto* L : m_pointLights)
    {
        delete L;
    }
    m_pointLights.clear();

    for (auto* L : m_spotLights)
    {
        delete L;
    }
    m_spotLights.clear();

    // Delete dynamically allocated GameObject instances
    for (auto* go : m_gameObjects)
    {
        delete go; // Deletes the GameObject instance
    }
    m_gameObjects.clear(); // Clear the vector after deleting contents

    // Clear the transparent game objects list (they point to objects already deleted above)
    m_transparent_gameObjects.clear();

    // Delete dynamically allocated Mesh instances
    for (auto& pair : m_LevelMeshes) // Iterate through the multimap
    {
        delete pair.second; // Delete the Mesh* pointer
    }
    m_LevelMeshes.clear(); // Clear the multimap after deleting contents

    // Clear the collider list (these are non-owning pointers, owned by GameObjects)
    m_colliders.clear();

	// Shutdown ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (Audio_Manager::getInstance() != nullptr)
    {
        delete Audio_Manager::getInstance();
    }

    // Clean up GLFW window and terminate GLFW
    if (m_window)
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
        m_window = nullptr; // Set to nullptr after destruction
    }
	std::cout << "Application shutdown complete.\n";
}
void Application::resetGame()
{
    // Clear existing game objects and colliders (if dynamically allocated/managed)
    for (auto* go : m_gameObjects)
    {
        delete go; // Delete the GameObject instances
    }
    m_gameObjects.clear(); // Clear the vector itself
    m_colliders.clear(); // Clear the vector of raw collider pointers

    // Delete existing Mesh instances loaded by Mesh::LoadInstances ---
    for (auto& pair : m_LevelMeshes) // Iterate through the multimap
    {
        delete pair.second; // Delete the Mesh* pointer
    }
    m_LevelMeshes.clear(); // Clear the multimap itself

    // Re-initialize game state variables
    m_player.reset();
    m_camera = Camera( // Reset camera to initial position
        Vector3(0.0f, 0.0f, 10.0f),   // eye
        Vector3(0.0f, 0.0f, 0.0f),   // center
        Vector3(0.0f, 1.0f, 0.0f),   // up
        60.0f * (3.14159265f / 180.0f),
        static_cast<float>(m_width) / m_height,
        0.1f,
        100.0f
    );
    m_player.setCamera(&m_camera); // Re-set camera for the new player instance

    // Re-load mesh instances (and then re-create game objects)
    if (!Mesh::loadInstances("../../Assets/Levels/LevelTest.txt", m_LevelMeshes, m_resourceManager))
    {
        // Handle error: if reloading fails, your game is in a bad state.
        std::cerr << "CRITICAL ERROR: Failed to reload mesh instances during game reset!\n";
        return;
    }

    // Re-create the level and game objects
    createLevel();

    // Reset any other relevant game state
    m_lastFrame = static_cast<float>(glfwGetTime()); // Reset frame time
    m_isRunning = true; // Ensure game starts running after restart
	m_gameWin = false; // Reset game win state
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Re-disable cursor
}
