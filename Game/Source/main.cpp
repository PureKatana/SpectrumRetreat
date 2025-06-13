#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include"ResourceManager.h"
#include "Model.h"
#include "stb_image.h"
#include <filesystem>
#include "Application.h"

int main(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);

	{
	Application app(1920,1080);

		app.init();
		app.run();
	}
	
	_CrtDumpMemoryLeaks();
}