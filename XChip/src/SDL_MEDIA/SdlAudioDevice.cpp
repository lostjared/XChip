#include <cmath>
#include <SDL2/SDL.h>
#include <XChip/SDL_MEDIA/SdlAudioDevice.h>
#include <XChip/SDL_MEDIA/SdlAudioDevice.h>
#include <XChip/Utility/Log.h>



namespace xchip {


SdlAudioDevice::~SdlAudioDevice()
{
	if (_initialized)
		this->Dispose();
}



bool SdlAudioDevice::Initialize() noexcept
{
	using namespace utility::literals;
	
	if (_initialized)
		this->Dispose();

	const auto cleanup = utility::make_scope_exit([this]() { 
		if (!this->_initialized) 
			this->Dispose(); 
	});


	SDL_zero(_want);
	_want.freq = 44100;
	_want.format = AUDIO_S16;
	_want.channels = 1;
	_want.samples = 1024;
	_want.callback = SdlAudioDevice::audio_callback<Sint16>;
	_want.userdata = this;

	_dev = SDL_OpenAudioDevice(NULL, 0, &_want, &_have, 0);

	if (_dev < 2)
	{
		utility::LOGerr("SdlSound: no valid audio device. SDL MSG: "_s + SDL_GetError());
		return false;
	}

	_len = 0.f;
	_pos = 0u;
	_amplitude = 16000;
	_cycleTime = _have.freq / 60.f;
	this->SetFreq(300);

	_initialized = true;
	return true;
}



void SdlAudioDevice::Dispose() noexcept
{
	if (_dev != 0)
	{
		SDL_CloseAudioDevice(_dev);
		_dev = 0;
	}

	_initialized = false;
}



void SdlAudioDevice::SetFreq(const float hz)
{
	if (!this->IsRunning()) {
		_freq = hz / _have.freq;
	}
	else {
		SDL_LockAudioDevice(_dev);
		_freq = hz / _have.freq;
		SDL_UnlockAudioDevice(_dev);
	}
}



void SdlAudioDevice::SetLenght(const unsigned int len)
{ 
	if (!this->IsRunning()) {
		_len = _cycleTime * len;
	}
	else {
		SDL_LockAudioDevice(_dev);
		_len = _cycleTime * len;
		SDL_UnlockAudioDevice(_dev);
	}
}


template<class T>
void SdlAudioDevice::audio_callback(void* userdata, Uint8* const stream, const int len)
{
	auto *const _this = reinterpret_cast<SdlAudioDevice*>(userdata);
	auto *const buff = reinterpret_cast<T*>(stream);
	const size_t bufflen = len / sizeof(T);
	
	constexpr float _2pi = 2 * M_PI;
	const auto ampl = _this->_amplitude;
	const auto freq = _this->_freq;
	auto pos = _this->_pos;


	if (_this->_len <= 0) 
	{
		// need a better workaround to stop clipping.
		auto downAmpl = ampl;
		for (size_t i = 0; i < bufflen; ++i, ++pos) 
		{
			buff[i] = static_cast<T>(downAmpl * std::sin(_2pi * freq * pos));	
			if (downAmpl > 100)
				downAmpl -= 60;
		}

		_this->Pause();
		return;
	}

	for (size_t i = 0; i < bufflen; ++i, ++pos)
		buff[i] = static_cast<T>(ampl * std::sin(_2pi * freq * pos));

	_this->_pos = pos;
	_this->_len -= bufflen;
	
}



}
