#include <cstdio>
#include <cmath>
#include <new>
#include <SDL2/SDL.h>
#include <XChip/SDL/SdlSound.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/Timer.h>



#define FREQ ((float)300) // tone



namespace xchip {



SdlSound::SdlSound() noexcept
	:  SdlMedia(System::Sound),
	_want(nullptr),
	_have(nullptr),
	_dev(0),
	_playing(false),
	_audioPos(0),
	_audioLen(0),
	_audioFreq(0),
	_audioVol(0),
	_cycleTime(0),
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
	_want->samples = static_cast<decltype(_want->samples)>(1024 * 4);
	_want->callback = SdlSound::audio_callback<Sint16>;
	_want->userdata = this;
	
	_dev = SDL_OpenAudioDevice(NULL, 0, _want, _have, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
	
	if (_dev < 2)
	{
		this->Dispose();
		utility::LOGerr("SdlSound: no valid audio device. SDL:"_s + SDL_GetError());
		return false;
	}

	_audioVol = 6000;
	_cycleTime = (_have->freq / 60.f);

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


void SdlSound::SetCountdownFreq(const float hz)
{
	if (_want == nullptr)
		return;

	_cycleTime = (_have->freq / hz);
}


void SdlSound::Play(const uint8_t soundTimer)
{
	if (!_playing)
	{
		_audioLen = _cycleTime * soundTimer;
		_audioFreq = (FREQ + 2 * soundTimer) / _have->freq;
	}

	else
	{
		SDL_LockAudioDevice(_dev);
		_audioLen += _cycleTime * soundTimer;
		_audioFreq = (FREQ + 2 * soundTimer) / _have->freq;
		SDL_UnlockAudioDevice(_dev);
	}

	_playing = true;
	SDL_PauseAudioDevice(_dev, 0);
}


void SdlSound::Stop()
{
	_playing = false;
	_audioPos = 0;
	SDL_PauseAudioDevice(_dev, 1);
}


template<class T>
void SdlSound::audio_callback(void* sdlSound, Uint8* const stream, int len)
{
	auto _this = (SdlSound*)sdlSound;

	constexpr auto _2pi = static_cast<float>(2 * M_PI);
	const int bufsize = (len / sizeof(T));
	T* buf = (T*) stream;
	

	if (_this->_audioLen <= 0.0f)
	{
		for (int i = 0; i < bufsize; ++i)
			buf[i] = 0;

		_this->Stop();
		return;
	}

	const float freq = _this->_audioFreq;
	const float vol = _this->_audioVol;
	unsigned int pos = _this->_audioPos;


	for (int i = 0; i < bufsize; ++i, ++pos)
		buf[i] = static_cast<T>(vol * sin( _2pi * freq * pos ));

	
	_this->_audioPos = pos;
	_this->_audioLen -= bufsize;
}





}
