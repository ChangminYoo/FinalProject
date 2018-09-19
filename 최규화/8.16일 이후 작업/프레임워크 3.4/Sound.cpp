#include "Sound.h"

CSound::CSound(void)
{
	FMOD::System_Create(&system);
	system->init(32, FMOD_INIT_NORMAL, nullptr);
	
	//system->createSound("sound/title2.ogg", FMOD_DEFAULT, 0, &titleSound);
	system->createSound("sound/hit1.ogg", FMOD_DEFAULT, 0,   &hitSound1);
	
	//스킬 사운드
	system->createSound("sound/skill1.ogg", FMOD_DEFAULT, 0, &shotSound1); //라이트 큐브
	system->createSound("sound/skill2.ogg", FMOD_DEFAULT, 0, &shotSound2); //헤비 큐브
	system->createSound("sound/tetrike.ogg", FMOD_DEFAULT, 0, &tetrikeSound); //테트라이크
	system->createSound("sound/dice.ogg", FMOD_DEFAULT, 0, &diceSound); //다이스트라이크
	//system->createSound(); //파동파
	//system->createSound(); //실드
	//system->createSound(); //해머
	//system->createSound(); //메테오
}


CSound::~CSound(void)
{
}


void CSound::PlaySoundEffect(SoundType id)
{
	if (id == HIT1)
	{
		system->playSound(hitSound1, 0, false, &channel);
		channel->setVolume(0.7f);
	}
	else if (id == SKILL1)
	{
		system->playSound(shotSound1, 0, false, &channel);
		channel->setVolume(0.7f);
	}
	else if (id == SKILL2)
	{
		system->playSound(shotSound2, 0, false, &channel);
		channel->setVolume(0.7f);
	}
	else if (id == SKILL3)
	{
		system->playSound(tetrikeSound, 0, false, &channel);
		channel->setVolume(0.9f);
	}
	else if (id == SKILL4)
	{
		system->playSound(diceSound, 0, false, &channel);
		channel->setVolume(0.7f);
	}


}

void CSound::PlaySoundBG()
{
	system->playSound(titleSound, 0, false, &channel);
	channel->setVolume(0.4f);
	titleSound->setMode(FMOD_LOOP_NORMAL);

}

void CSound::StopSound()
{

}

void CSound::ReleaseSound()
{
	titleSound->release();
	hitSound1->release();
	shotSound1->release();
	shotSound2->release();
	tetrikeSound->release();
	diceSound->release();

}

void CSound::Update()
{

}
