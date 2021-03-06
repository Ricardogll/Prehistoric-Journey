#ifndef __j1AUDIO_H__
#define __j1AUDIO_H__

#include "j1Module.h"
#include "SDL_mixer\include\SDL_mixer.h"
#include "p2List.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class j1Audio : public j1Module
{
public:

	j1Audio();

	// Destructor
	virtual ~j1Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// UnLoad WAV
	bool UnLoadFx(uint id);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	void RaiseVolumeMusic();
	void RaiseVolumeFx();
	void DecreaseVolumeMusic();
	void DecreaseVolumeFx();
	void SetMusicVolume(int volume);
	void SetFxVolume(int volume);
	uint GetMusicVolume() const;
	uint GetFxVolume() const;

private:

	_Mix_Music*			music;
	p2List<Mix_Chunk*>	fx;
	uint music_volume = 0u;
	uint fx_volume = 0u;
};

#endif // __j1AUDIO_H__