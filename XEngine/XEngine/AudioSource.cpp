#include "AudioSource.h"



AudioSource::AudioSource(char* filename, AudioType audioType)
{
	type = audioType;
	switch (audioType)
	{
	case Music:music = Mix_LoadMUS(filename); break;
	case FX:effect = Mix_LoadWAV(filename); break;
	}

	if (music == NULL)
	{
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
	}

}
AudioSource::~AudioSource()
{
	if (effect)
	{
		Mix_FreeChunk(effect);
	}
	if (music)
	{
		Mix_FreeMusic(music);
	}
	return;
}
void AudioSource::Play()
{
	switch (type)
	{
	case Music:Mix_PlayMusic(music, 1); break;
	case FX:Mix_PlayChannel(-1, effect, 0); break;
	}

}
void AudioSource::Play(int repeat)
{
	switch (type)
	{
	case Music:Mix_PlayMusic(music, repeat); break;
	case FX:Mix_PlayChannel(-1, effect, repeat); break;
	}
}
void AudioSource::Stop()
{
	Mix_FadeOutMusic(400);
}
void AudioSource::PlayOneShot(char* filename)
{

	Mix_Chunk *effect = Mix_LoadWAV(filename); 
	Mix_PlayChannel(-1, effect, 0);
	//Mix_FreeChunk(effect);
}


void SetVolumeChannel(int channel, int vol)
{

}
void SetVolumeMusic(int volume)
{

}

