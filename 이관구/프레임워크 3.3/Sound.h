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
		SKILL3,
		SKILL4,
		SKILL5,
		SKILL6,
		SKILL7,
		SKILL8,
		HIT1,//ÇÇ°Ý
		IMP_ATTACK

	};
private:
	FMOD::System *system;
	FMOD::Sound  *titleSound, *shotSound1, *shotSound2, *tetrikeSound, *diceSound, *waveSound, *shieldSound, *hammerSound, *MeteorSound,*hitSound1;
	FMOD::Channel *channel = 0;

public:
	void PlaySoundEffect(SoundType id);
	void PlaySoundBG();
	void StopSound();
	void ReleaseSound();
	void Update();

};

