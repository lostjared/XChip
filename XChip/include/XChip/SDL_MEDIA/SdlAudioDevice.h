#ifndef _XCHIP_SDLAUDIODEVICE_H_
#define _XCHIP_SDLAUDIODEVICE_H_
#include <cstdint>
#include <SDL2/SDL_audio.h>



namespace xchip {


class SdlAudioDevice
{
public:
	using AudioCallback = void(*)(void* userdata, Uint8* stream, int len);

	SdlAudioDevice() noexcept = default;
	~SdlAudioDevice();
	SdlAudioDevice(const SdlAudioDevice&) = delete;
	SdlAudioDevice& operator=(const SdlAudioDevice&) = delete;

	bool Initialize(int wantedFreq, const SDL_AudioFormat wantedFormat,
		const Uint8 channels, const Uint16 samples,
		const AudioCallback callback, void* userdata) noexcept;
	void Dispose() noexcept;

	bool IsInitialized() const;
	int GetCurrentFreq() const;
	bool IsRunning() const;
	const SDL_AudioSpec& GetWantedSpec() const;
	const SDL_AudioSpec& GetCurrentSpec() const;
	SDL_AudioSpec& GetWantedSpec();
	SDL_AudioSpec& GetCurrentSpec();
	
	void Lock();
	void Unlock();
	void Run();
	void Pause();

private:
	SDL_AudioSpec _want, _have;
	SDL_AudioDeviceID _dev = 0;
	bool _initialized = false;
};








inline bool SdlAudioDevice::IsInitialized() const { return _initialized;  }
inline int SdlAudioDevice::GetCurrentFreq() const { return _have.freq; };
inline bool SdlAudioDevice::IsRunning() const {
	return SDL_GetAudioDeviceStatus(_dev) == SDL_AUDIO_PLAYING; 
}

inline const SDL_AudioSpec& SdlAudioDevice::GetWantedSpec() const { return _want; }
inline const SDL_AudioSpec& SdlAudioDevice::GetCurrentSpec() const { return _have; }
inline SDL_AudioSpec& SdlAudioDevice::GetWantedSpec() { return _want; }
inline SDL_AudioSpec& SdlAudioDevice::GetCurrentSpec() { return _have; }


inline void SdlAudioDevice::Lock() { SDL_LockAudioDevice(_dev); }
inline void SdlAudioDevice::Unlock() { SDL_UnlockAudioDevice(_dev); }
inline void SdlAudioDevice::Run() { SDL_PauseAudioDevice(_dev, 0); }
inline void SdlAudioDevice::Pause() { SDL_PauseAudioDevice(_dev, 1); }



}











#endif
