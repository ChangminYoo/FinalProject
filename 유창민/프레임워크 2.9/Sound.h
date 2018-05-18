#pragma once

#include "fmod.hpp"
#include "fmod_dsp.h"
#include "fmod_errors.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>

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

	FMOD::System* system;
	FMOD::Sound* sound[COUNT];
	FMOD::Channel* channel[COUNT];

public:
	void Init()
	{
		FMOD::System_Create(&system);
		system->init(32, FMOD_INIT_NORMAL, nullptr);

		system->createSound("sound/title.ogg", FMOD_CREATESAMPLE |  FMOD_LOOP_NORMAL, 0, &sound[TITLE]);
		system->createSound("sound/발걸음.ogg", FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &sound[STEP]);
		system->createSound("sound/피격1.ogg", FMOD_CREATESAMPLE  | FMOD_LOOP_NORMAL, 0, &sound[HIT]);
		system->createSound("sound/title.ogg", FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &sound[SKILL1]);
		system->createSound("sound/title.ogg", FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &sound[SKILL2]);
		system->createSound("sound/title.ogg", FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &sound[SKILL3]);
		system->createSound("sound/title.ogg", FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &sound[SKILL4]);
		
	}



};

