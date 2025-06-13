#pragma once


#include "Physics/Physics.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>


#include <vector>
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "ResourceManager.h"
#include "UI_Manager.h"
#include "Shader.h"
#include "Texture.h"
#include "Model.h"
#include "Mesh.h"
#include "Camera.h"
#include "Physics/Collider.h"
#include "Physics/Physics.h"
#include "SceneGraph.h"
#include "Player.h"
#include "LibMath/Matrix/Matrix4.h"
#include "LibMath/Vector/Vector3.h"
#include "GameObject.h"


#include "Light.h"
#include "LightInstance.h"

class Application
{
public:
    Application(int width = 1280, int height = 720);
    ~Application();

    bool    init();
    void    run();
    void    shutdown();
    void    resetGame();

private:
    int             m_width;
    int             m_height;
    GLFWwindow*     m_window = nullptr;
    ResourceManager m_resourceManager;
    Player          m_player;
    Camera          m_camera;
	bool			m_isRunning = true;
    bool            m_escPressedLastFrame = false;
	bool            m_gameWin = false;
    UI_Manager      m_uiManager;
    
    // lights
    std::vector<DirectionalLightInstance*>          m_dirLights;
    std::vector<PointLightInstance*>                m_pointLights;
    std::vector<SpotLightInstance*>                 m_spotLights;

    float                                           m_lastFrame = 0.0f;

    std::unordered_multimap<int, Mesh*>             m_LevelMeshes;
	std::vector<GameObject*>                        m_gameObjects;
    std::vector<GameObject*>                        m_transparent_gameObjects;
    std::vector<Collider*>                          m_colliders;

    bool    loadTexture(const std::string& name, const std::string& path);
    bool    loadShader(const std::string& name, const std::string& vert, const std::string& frag);
    bool    loadMesh(
        Mesh** meshPtr,
        const std::string& modelName,
        const std::string& objPath,
        const std::string& textureName
    );
    bool    loadResources();
    bool    createLights();
    void    createLevel();
    void    processInput(float deltaTime);
    void    handleCollisions(float deltaTime);
	void    updateMovingGameObjects(float deltaTime);
    void    render();
    void    winGame();
};