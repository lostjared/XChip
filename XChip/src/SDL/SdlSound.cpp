#include <cstdio>
#include <cmath>
#include <SDL2/SDL.h>
#include <XChip/SDL/SdlSound.h>

#define FREQ 600 /* the frequency we want */


namespace xchip {







SdlSound::SdlSound() noexcept
	: _initialized(false)
{

}





bool SdlSound::Initialize() noexcept
{
	if(SDL_Init(SDL_INIT_AUDIO)) 
	{
		printf("SDL Failed to initialize: %s\n", SDL_GetError());
		return false;
  	}

  	SDL_zero(_want);
	_want.freq = 44100;
	_want.format = AUDIO_S16;
	_want.channels = 1;
	_want.samples = 4096;
	_want.callback = SdlSound::audio_callback;
	_want.userdata = this;
	_dev = SDL_OpenAudioDevice(NULL, 0, &_want, &_have, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);

	_audioLen = _have.freq;
	_audioPos = 0;
	_audioFreq = 1.0 * FREQ / _have.freq; /* 1.0 to make it a float */
	_audioVol = 6000; /* ~1/5 max volume */




	_initialized = true;

	return true;
}

void SdlSound::Dispose() noexcept
{
	SDL_CloseAudioDevice(_dev);
}



bool SdlSound::IsInitialized()
{
	return _initialized;
}


bool SdlSound::IsPlaying()
{
	return _audioLen > 0;
}

void SdlSound::Play()
{
	_audioLen = 1;
	SDL_PauseAudioDevice(_dev, 0);
}

void SdlSound::Stop()
{
	_audioLen = 0;
	SDL_PauseAudioDevice(_dev, 1);
}









void SdlSound::audio_callback(void* sdlSound, Uint8* const stream, int len) 
{
	auto _this = (SdlSound*) sdlSound;

	len /= sizeof(Sint16); /* 16 bit */
	Sint16* buf = (Sint16*)stream;

	for(int i = 0; i < len; i++)
	{ 
		buf[i] = _this->_audioVol * sin(2 * M_PI * i * _this->_audioFreq);
	}

	return;
}





}