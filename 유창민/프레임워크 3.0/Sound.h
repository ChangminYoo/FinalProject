#pragma once

#include "fmod/inc/fmod.hpp"
#include "fmod/inc/fmod_dsp.h"
#include "fmod/inc/fmod_errors.h"
#include <windows.h>
#include <conio.h>

#pragma comment(lib, "fmod/lib/fmod64_vc.lib")

using namespace std;

class CSound
{
public:
	CSound();
	~CSound();

	enum SoundType
	{
		TITLE,//주음악
		STEP,//발걸음
		HIT,//피격
		SKILL1,
		SKILL2,
		SKILL3,
		SKILL4,
		MONSTER,
		COUNT
	};

	static CSound& GetInstance()
	{
		static CSound s;
		return s;
	}

	void DeleteSound();
	void PlaySoundEffect(SoundType id);
	void PauseSound(SoundType id);
	void ContinueSound(SoundType id);
	void PauseAllGameSounds();
	void SetVolume(SoundType id, float vol);

	FMOD::System* system = NULL;
	FMOD::Sound* sound[COUNT];
	FMOD::Channel* channel[COUNT];

public:
	void Init()
	{
		FMOD::System_Create(&system);
		system->init(32, FMOD_INIT_NORMAL, nullptr);

		system->createSound("sound/title2.ogg", FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &sound[TITLE]);
		system->createSound("sound/step.ogg", FMOD_DEFAULT , 0, &sound[STEP]);
		system->createSound("sound/hit1.ogg",  FMOD_DEFAULT , 0, &sound[HIT]);
		system->createSound("sound/skill1.ogg", FMOD_DEFAULT , 0, &sound[SKILL1]);
		system->createSound("sound/skill2.ogg", FMOD_DEFAULT , 0, &sound[SKILL2]);
		system->createSound("sound/title.ogg", FMOD_DEFAULT , 0, &sound[SKILL3]);
		system->createSound("sound/title.ogg", FMOD_DEFAULT , 0, &sound[SKILL4]);
		system->createSound("sound/title.ogg", FMOD_DEFAULT, 0, &sound[MONSTER]);
	}



};

