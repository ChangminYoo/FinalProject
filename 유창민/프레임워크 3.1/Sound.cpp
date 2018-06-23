#include "Sound.h"

void ErrorCheck(FMOD_RESULT &result)
{
	if (result != FMOD_OK)
	{
		//printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}
}

CSound::CSound()
{
	system = NULL;
	sound[COUNT] = NULL;
	channel[COUNT] = NULL;

}
CSound::~CSound()
{
}


void CSound::DeleteSound()
{
	for (int i = 0; i < SoundType::COUNT; ++i)
	{
		sound[i]->release();
	}

	system->close();
	system->release();
}

void CSound::PlaySoundEffect(SoundType id)
{
	system->playSound(sound[id], 0, false, &channel[id]);
}
void CSound::PauseSound(SoundType id)
{
	channel[id]->setPaused(true);
}
void CSound::ContinueSound(SoundType id)
{
	channel[id]->setPaused(false);
}
void CSound::PauseAllGameSounds()
{
	for (int i = 0; i < SoundType::COUNT; ++i)
	{
		channel[i]->setPaused(true);
	}
}

void CSound::SetVolume(SoundType id, float vol)
{
	channel[id]->setVolume(vol);
}
