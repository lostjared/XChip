#include <cstdio>
#include <cmath>
#include <new>
#include <SDL2/SDL.h>
#include <XChip/SDL/SdlSound.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/Timer.h>

#define FREQ ((float)350) /* the frequency we want */


namespace xchip {

float _audioLen;


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

	if (_initialized)
		this->Dispose();

	if(SDL_Init(SDL_INIT_AUDIO)) 
	{
		utility::LOGerr("SdlSound Failed to initialize: "_s + SDL_GetError());
		return false;
  	}

	_want = new(std::nothrow) SDL_AudioSpec[2];
	_have = _want + 1;
	
	if (!_want)
	{
		utility::LOGerr("SdlSound unable to alloc SDL_AudioSpec");
		return false;
	}


  	SDL_memset(_want, 0, sizeof(SDL_AudioSpec));
	_want->freq = 44100;
	_want->format = AUDIO_S16;
	_want->channels = 1;
	_want->samples = static_cast<decltype(_want->samples)>(4096);
	_want->callback = SdlSound::audio_callback<Sint16>;
	_want->userdata = this;
	
	_dev = SDL_OpenAudioDevice(NULL, 0, _want, _have, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
	
	if (_dev < 2)
	{
		this->Dispose();
		utility::LOGerr("SdlSound: no valid audio device. SDL:"_s + SDL_GetError());
		return false;
	}


	_audioLen = 0;
	_audioPos = 0;
	_audioFreq = FREQ / _have->freq;
	_audioVol = 8000; // ~1/5 max volume
	

	_playing = false;
	_initialized = true;
	return true;
}



void SdlSound::Dispose() noexcept
{
	if (this->IsPlaying())
		this->Stop();
	
	SDL_CloseAudioDevice(_dev);

	delete[] _want;
	_want = nullptr;
	_have = nullptr;
	_initialized = false;
}


void SdlSound::Play(unsigned soundTimer)
{
	if (!_playing)
	{
		_playing = true;
		_audioLen = (_have->freq * soundTimer / 60.f);

	}

	else
	{
		SDL_LockAudioDevice(_dev);
		_audioLen += (_have->freq * soundTimer / 60.f);
		SDL_UnlockAudioDevice(_dev);
	}

	SDL_PauseAudioDevice(_dev, 0);
}


void SdlSound::Stop()
{
	_playing = false;
	_audioPos = 0;
	_audioLen = 0;
	SDL_PauseAudioDevice(_dev, 1);
}


template<class T>
void SdlSound::audio_callback(void* sdlSound, Uint8* const stream, int len)
{
	auto _this = (SdlSound*)sdlSound;

	constexpr auto _2pi = static_cast<float>(2 * M_PI);
	T* buf = (T*)stream;
	const int bufsize = len / sizeof(T);

	if (_audioLen < 0.0f)
	{
		for (int i = 0; i < bufsize; ++i)
			buf[i] = 0;

		_this->Stop();
		
		return;
	}

	for (int i = 0; i < bufsize; ++i)
	{
		buf[i] = static_cast<T>
			(_this->_audioVol *
				sin(_2pi * _this->_audioFreq * _this->_audioPos));
		
		++_this->_audioPos;
	}

	_audioLen -= bufsize;
}





}
