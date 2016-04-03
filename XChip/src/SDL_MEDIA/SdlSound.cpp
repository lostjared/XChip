#include <cstdio>
#include <cmath>
#include <new>

#include <SDL2/SDL.h>
#include <XChip/SDL_MEDIA/SdlSound.h>
#include <XChip/SDL_MEDIA/SdlAudioDevice.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/Timer.h>

namespace xchip {




SdlSound::SdlSound() noexcept
	:  SdlMedia(System::Sound),
	_device(nullptr),
	_audioPos(0),
	_tone(512),
	_audioLen(0),
	_audioFreq(0),
	_audioVol(0),
	_cycleTime(0),
	_initialized(false)
{
	utility::LOG("Creating SdlSound object...");
}

SdlSound::~SdlSound()
{
	if(_initialized)
		this->Dispose();

	utility::LOG("Destroying SdlSound object...");
}



bool SdlSound::Initialize() noexcept
{
	using namespace utility::literals;

	if (_initialized)
		this->Dispose();

	else if (!this->InitSubSystem())
		return false;

	_device = new(std::nothrow) SdlAudioDevice;
	
	if (!_device)
	{
		utility::LOGerr("Could not allocate SdlAudioDevice...");
		return false;
	}
	
	if(!_device->Initialize(44100, AUDIO_S16, 1, 4*1024, 
                           SdlSound::audio_callback<Sint16>, this))
	{
		this->Dispose();
		return false;
	}


	_audioVol = 6000;
	_cycleTime = (_device->GetCurrentFreq() / 60.f);
	_initialized = true;

	return true;
}



void SdlSound::Dispose() noexcept
{
	if (_device)
	{
		if (_device->IsInitialized())
			_device->Dispose();

		delete _device;
		_device = nullptr;
	}

	_initialized = false;
}


bool SdlSound::IsPlaying() const
{
	return _device->IsRunning();
}

void SdlSound::SetCountdownFreq(const float hz)
{
	if (!_device->IsInitialized())
		return;

	_cycleTime = (_device->GetCurrentFreq() / hz);
}


void SdlSound::Play(const uint8_t soundTimer)
{
	if (!_device->IsRunning())
	{
		_audioLen = _cycleTime * soundTimer;
		_audioFreq = (_tone + 2 * soundTimer) / _device->GetCurrentFreq();
	}

	else
	{
		_device->Lock();
		_audioLen += _cycleTime * soundTimer;
		_audioFreq = (_tone + 2 * soundTimer) / _device->GetCurrentFreq();
		_device->Unlock();
	}

	_device->Run();
}


void SdlSound::Stop()
{
	_device->Pause();
	_audioPos = 0;
}


template<class T>
void SdlSound::audio_callback(void* sdlSound, uint8_t* const stream, int len)
{
	auto _this = static_cast<SdlSound*>(sdlSound);

	constexpr auto _2pi = static_cast<float>(2 * M_PI);
	const int bufsize = (len / sizeof(T));
	T* const buf = (T*) stream;
	

	

	const float freq = _this->_audioFreq;
	const float vol = _this->_audioVol;
	unsigned int pos = _this->_audioPos;

	


	if (_this->_audioLen <= 0.0f)
	{
		// pathetic temporary work around the clipping...
		float downVol = vol;
		for (int i = 0; i < bufsize; ++i, ++pos) 
		{
			buf[i] = static_cast<T>(downVol * sin(_2pi * freq * pos));
			if (downVol > 100) 
				downVol -= 44;
		}

		_this->Stop();
		return;
	}


	for (int i = 0; i < bufsize; ++i, ++pos)
		buf[i] = static_cast<T>(vol * sin( _2pi * freq * pos ));

	
	_this->_audioPos = pos;
	_this->_audioLen -= bufsize;
}





}
