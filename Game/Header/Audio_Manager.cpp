#include "Audio_Manager.h"
#include <iostream>

Audio_Manager* Audio_Manager::s_instance = nullptr;

Audio_Manager::Audio_Manager()
    : m_soundEngine(nullptr)
    , m_isInitialized(false)
{
    // Start the sound engine with default parameters
    m_soundEngine = irrklang::createIrrKlangDevice();

    if (!m_soundEngine)
    {
        std::cerr << "Error: Could not start IrrKlang sound engine!\n";
        m_isInitialized = false;
    }
    else
    {
        m_isInitialized = true;
    }
}

Audio_Manager* Audio_Manager::getInstance()
{
    if (s_instance == nullptr)
    {
        // Create the instance only if it doesn't exist yet
        s_instance = new Audio_Manager();
    }
    return s_instance;
}

Audio_Manager::~Audio_Manager()
{
    if (m_soundEngine)
    {
        m_soundEngine -> drop();
        m_soundEngine = nullptr;
		std::cout << "Audio_Manager destroyed and sound engine released.\n";
    }
    s_instance = nullptr;
}

void Audio_Manager::playSound(const char* filePath, bool loop)
{
    if (m_isInitialized && m_soundEngine)
    {
        m_soundEngine -> play2D(filePath, loop);
    }
    else
    {
        std::cerr << "Audio_Manager not initialized or sound engine is null. Cannot play sound: " << filePath << "\n";
    }
}

void Audio_Manager::stopAllSounds()
{
    if (m_isInitialized && m_soundEngine)
    {
        m_soundEngine -> stopAllSounds();
	}
    else
    {
        std::cerr << "Audio_Manager not initialized or sound engine is null. Cannot stop sounds.\n";
    }
}
