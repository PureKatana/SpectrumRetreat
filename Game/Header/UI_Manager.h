#pragma once
#include "imgui.h"
#include "Phone.h"
#include"glad/glad.h"
#include <GLFW/glfw3.h>
#include <functional>

class UI_Manager
{
public:
    
    void drawPhone(const Phone& phone, float radius = 200.0f);
	void drawCursor(float radius = 10.0f);
    void drawMenu(GLFWwindow* window, std::function<void()> onRestartClicked, const char* text);

};

