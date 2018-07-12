#pragma once

#include "fmod.hpp"
#include <windows.h>
#include <conio.h>
#include <string>

#pragma comment(lib, "fmod/lib/fmod64_vc.lib")
#pragma comment(lib, "fmod/lib/fmodL64_vc.lib")

using namespace std;


class CSound
{
public:
	CSound(void);
	~CSound(void);

	enum SoundType
	{
		SKILL1,
		SKILL2,
		HIT1,//ÇÇ°Ý

	};
private:
	FMOD::System *system;
	FMOD::Sound  *titleSound, *shotSound1, *shotSound2, *hitSound1;
	FMOD::Channel *channel = 0;

public:
	void PlaySoundEffect(SoundType id);
	void PlaySoundBG();
	void StopSound();
	void ReleaseSound();
	void Update();

};

