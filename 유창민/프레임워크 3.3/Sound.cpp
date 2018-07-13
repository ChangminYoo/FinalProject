#include "Sound.h"

CSound::CSound(void)
{
	FMOD::System_Create(&system);
	system->init(32, FMOD_INIT_NORMAL, nullptr);
	
	system->createSound("sound/title2.ogg", FMOD_DEFAULT, 0, &titleSound);
	system->createSound("sound/hit1.ogg", FMOD_DEFAULT, 0,   &hitSound1);
	system->createSound("sound/skill1.ogg", FMOD_DEFAULT, 0, &shotSound1);
	system->createSound("sound/skill2.ogg", FMOD_DEFAULT, 0, &shotSound2);

}


CSound::~CSound(void)
{
}


void CSound::PlaySoundEffect(SoundType id)
{
	if (id == HIT1)
	{
	//	system->playSound(hitSound1, 0, false, &channel);
	//	channel->setVolume(0.5f);
	}
	else if (id == SKILL1)
	{
	//	system->playSound(shotSound1, 0, false, &channel);
	//	channel->setVolume(0.7f);
	}
	else if (id == SKILL1)
	{
	//	system->playSound(shotSound2, 0, false, &channel);
	//	channel->setVolume(0.7f);
	}
}

void CSound::PlaySoundBG()
{
//	system->playSound(titleSound, 0, false, &channel);
	
//	titleSound->setMode(FMOD_LOOP_NORMAL);

}

void CSound::StopSound()
{

}

void CSound::ReleaseSound()
{
	//titleSound->release();
	//hitSound1->release();
	//shotSound1->release();
	//shotSound2->release();
}

void CSound::Update()
{

}
