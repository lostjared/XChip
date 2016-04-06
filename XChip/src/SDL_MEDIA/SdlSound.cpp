#include <cmath>
#include <cstdlib>
#include <cstring>

#include <SDL2/SDL.h>
#include <XChip/SDL_MEDIA/SdlSound.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/Timer.h>



namespace xchip {

inline float SdlSound::GetFreq() const { return _freq * _specs[Have].freq; }
inline bool SdlSound::IsPlaying() const { return SDL_GetAudioDeviceStatus(_dev) == SDL_AUDIO_PLAYING; }
inline void SdlSound::SetCountdownFreq(const float hertz) { _cycleTime = _specs[Have].freq / hertz; }
inline void SdlSound::SetCycleTime(const float hz) { _cycleTime = _specs[Have].freq / hz; }
inline void SdlSound::SetFreq(const float hz) { _freq = hz / _specs[Have].freq; }
inline void SdlSound::SetLenght(const unsigned int len) { _len = _cycleTime * len; }



SdlSound::SdlSound() noexcept
	:  SdlMedia(System::Sound),
	_specs(nullptr),
	_dev(0)
{
	utility::LOG("Creating SdlSound object...");
}



SdlSound::~SdlSound()
{
	if (_initialized)
		this->Dispose();

	utility::LOG("Destroying SdlSound object...");
}



bool SdlSound::Initialize() noexcept
{
	using namespace utility::literals;

	if (_initialized)
		this->Dispose();

	const auto cleanup = utility::make_scope_exit([this]() {
		if (!this->_initialized)
			this->Dispose();
	});


	if (!this->InitSubSystem())
		return false;

	else if (!this->InitDevice())
		return false;

	_initialized = true;
	return true;
}



void SdlSound::Dispose() noexcept
{
	if (_dev != 0)
	{
		SDL_CloseAudioDevice(_dev);
		_dev = 0;
	}

	if (_specs)
	{
		free(_specs);
		_specs = nullptr;
	}


	_initialized = false;
}




void SdlSound::Play(const uint8_t soundTimer)
{
	if (!this->IsPlaying()) 
	{
		SetFreq(defaultFreq + 2 * soundTimer);
		SetLenght(soundTimer);
	}
	
	else
	{
		SDL_LockAudioDevice(_dev);
		SetFreq(defaultFreq + 2 * soundTimer);
		SetLenght(soundTimer);
		SDL_UnlockAudioDevice(_dev);
	}

	SDL_PauseAudioDevice(_dev, 0);
}





void SdlSound::Stop()
{
	if (this->IsPlaying())
	{
		SDL_LockAudioDevice(_dev);
		_len = 0;
		SDL_UnlockAudioDevice(_dev);
	}
}






bool SdlSound::InitDevice()
{
	using namespace utility::literals;
	constexpr auto specsSize = sizeof(SDL_AudioSpec) * 2;

	_specs = static_cast<SDL_AudioSpec*>(malloc(specsSize));
	
	if(!_specs)
	{
		utility::LOGerr("SdlSound: Could not allocate SDL_AudioSpecs...");
		return false;
	}


	memset(_specs, 0, specsSize);
	_specs[Want].freq = 44100;
	_specs[Want].format = AUDIO_S16;
	_specs[Want].channels = 1;
	_specs[Want].samples = 1024;
	_specs[Want].callback = SdlSound::audio_callback<Sint16>;
	_specs[Want].userdata = this;

	_dev = SDL_OpenAudioDevice(NULL, 0, &_specs[Want], &_specs[Have], 0);

	if (_dev < 2) 
	{
		utility::LOGerr("SdlSound: Failed to open audio device: "_s + SDL_GetError());
		return false;
	}

	_len = 0.f;
	_pos = 0u;
	_amplitude = 16000;
	_cycleTime = _specs[Have].freq / 60.f;
	this->SetFreq(defaultFreq);

	return true;
}








template<class T>
void SdlSound::audio_callback(void* userdata, uint8_t* const stream, const int len)
{
	auto *const _this = reinterpret_cast<SdlSound*>(userdata);
	auto *const buff = reinterpret_cast<T*>(stream);
	const size_t bufflen = len / sizeof(T);


	constexpr auto _2pi = static_cast<float>(2 * M_PI);
	const auto ampl = _this->_amplitude;
	const auto freq = _this->_freq;
	auto pos = _this->_pos;


	if (_this->_len > 0)
	{
		for (size_t i = 0; i < bufflen; ++i, ++pos)
			buff[i] = static_cast<T>(ampl * std::sin(_2pi * freq * pos));

		_this->_pos = pos;
		_this->_len -= bufflen;
	}

	else
	{	// need a better workaround to stop clipping.
		auto downAmpl = ampl;
		for (size_t i = 0; i < bufflen; ++i, ++pos)
		{
			buff[i] = static_cast<T>(downAmpl * std::sin(_2pi * freq * pos));
			if (downAmpl > 100)
				downAmpl -= 60;
		}

		// pause the device from the callback function.
		// is this ok ?
		_this->_pos = 0;
		SDL_PauseAudioDevice(_this->_dev, 1);
		
	}

}





}
