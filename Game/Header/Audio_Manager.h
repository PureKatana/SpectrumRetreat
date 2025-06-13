#pragma once

#include <irrKlang.h>

class Audio_Manager
{
public:
	static Audio_Manager* getInstance();

	~Audio_Manager();
	void playSound(const char* filePath, bool loop = false);
	void stopAllSounds();

private:

	Audio_Manager();

	// Delete copy constructor and copy assignment operator to prevent cloning
	Audio_Manager(const Audio_Manager&) = delete;
	Audio_Manager& operator=(const Audio_Manager&) = delete;

	static Audio_Manager*		s_instance;

	irrklang::ISoundEngine*		m_soundEngine;
	bool						m_isInitialized;
};