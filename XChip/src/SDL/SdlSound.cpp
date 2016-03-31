#include <cstdio>
#include <cmath>
#include <new>
#include <SDL2/SDL.h>
#include <XChip/SDL/SdlSound.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/Timer.h>

#define FREQ 300 /* the frequency we want */


namespace xchip {




SdlSound::SdlSound() noexcept
	:  SdlMedia(System::Sound),
	_initialized(false)
{
	utility::LOG("Creating SdlSound...");
}

SdlSound::~SdlSound()
{
	if(_initialized)
		this->Dispose();

	utility::LOG("Destroying SdlSound...");
}



bool SdlSound::Initialize() noexcept
{
	using namespace utility::literals;
	if(SDL_Init(SDL_INIT_AUDIO)) 
	{
		utility::LOGerr("SdlSound Failed to initialize: "_s + SDL_GetError());
		return false;
  	}

	_want = new(std::nothrow) SDL_AudioSpec[2];
	_have = &_want[1];

	if (!_want || !_have)
	{
		utility::LOGerr("SdlSound unable to alloc SDL_AudioSpec");
		return false;
	}


  	SDL_memset(_want, 0, sizeof(SDL_AudioSpec));
	_want->freq = 44100;
	_want->format = AUDIO_S16;
	_want->channels = 1;
	_want->samples = 256;
	_want->callback = SdlSound::audio_callback;
	_want->userdata = this;
	
	_dev = SDL_OpenAudioDevice(NULL, 0, _want, _have, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);

	_audioLen = 0;
	_audioPos = 0;
	_audioFreq = 1.0f * FREQ / _have->freq;
	_audioVol = 6000; // ~1/5 max volume

	SDL_PauseAudioDevice(_dev, 0); // start audio loop
	_initialized = true;

	return true;
}



void SdlSound::Dispose() noexcept
{

	if (_initialized)
	{
		if (this->IsPlaying())
			this->Stop();

		SDL_PauseAudioDevice(_dev, 1);
		SDL_CloseAudioDevice(_dev);

		delete[] _want;
		_want = nullptr;
		_have = nullptr;
		_initialized = false;
	}
}



void SdlSound::Play()
{
	//SDL_LockAudioDevice(_dev);
	_audioLen = 1;
	//SDL_UnlockAudioDevice(_dev);
	
}

void SdlSound::Stop()
{
	//SDL_LockAudioDevice(_dev);
	_audioLen = 0;
	_audioPos = 0;
	//SDL_UnlockAudioDevice(_dev);
}










void SdlSound::audio_callback(void* sdlSound, uint8_t* const stream, int len) 
{
	auto _this = (SdlSound*) sdlSound;
	if (_this->_audioLen == 0) 
	{
		SDL_memset(stream, 0, len);
		return;
	}

	static constexpr auto _2pi = 2 * M_PI;
	len /= sizeof(Sint16);
	Uint16* buf = (Uint16*)stream;
	
	for (int i = 0; i < len; i++)
	{
		buf[i] = static_cast<Sint16>(_this->_audioVol *
			                         sin( _2pi * _this->_audioPos * _this->_audioFreq));
		++_this->_audioPos;
	}

}





}
