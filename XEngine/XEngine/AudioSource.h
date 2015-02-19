
#include<Windows.h>
#include <SDL\SDL.h>
#include <SDL\SDL_mixer.h>
#include <string>

using std::string;
/*

This uses SDL libraries to Run audio
*/
enum AudioType
{
	Music,
	FX
};
class AudioSource
{
	Mix_Chunk* effect;
	Mix_Music* music;
	AudioType type;
public :
	AudioSource(char* filename, AudioType type);
	~AudioSource();
	void Play();
	void Play(int repeat);
	void Stop();
	static void PlayOneShot(char* filename);
	void SetVolumeChannel(int channel, int vol);
	void SetVolumeMusic(int volume);


};