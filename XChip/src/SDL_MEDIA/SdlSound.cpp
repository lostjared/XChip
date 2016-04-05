#include <new>

#include <SDL2/SDL.h>
#include <XChip/SDL_MEDIA/SdlSound.h>
#include <XChip/SDL_MEDIA/SdlAudioDevice.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/Timer.h>

namespace xchip {



SdlSound::SdlSound() noexcept
	:  SdlMedia(System::Sound)
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
	
	else if(!_device->Initialize()) 
	{
		this->Dispose();
		return false;
	}

	_initialized = true;
	return true;
}



void SdlSound::Dispose() noexcept
{
	if (_device)
	{
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



void SdlSound::SetCountdownFreq(const float hertz)
{
	_device->SetCycleTime(hertz);
}



void SdlSound::Play(const uint8_t soundTimer)
{
	_device->SetFreq(defaultFreq + 2 * soundTimer);
	_device->SetLenght(soundTimer);
	_device->Run();
}



void SdlSound::Stop()
{
	_device->Pause();
}




}
