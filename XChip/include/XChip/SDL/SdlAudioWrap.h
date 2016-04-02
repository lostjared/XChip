#ifndef _XCHIP_SDLAUDIOWRAP_H_
#define _XCHIP_SDLAUDIOWRAP_H_
#include <cstdint>
#include <SDL2/SDL_audio.h>

namespace xchip
{


class SdlAudioDevice
{
public:
	using AudioCallback = void(*)(void* userdata, Uint8* stream, int len);

	SdlAudioDevice() noexcept;
	~SdlAudioDevice();

	bool Initialize(int wantedFreq, const SDL_AudioFormat wantedFormat,
		const Uint8 channels, const Uint16 samples,
		const AudioCallback callback, void* userdata) noexcept;

	void Dispose() noexcept;

	bool IsInitialized() const { return _initialized;  }

	const SDL_AudioSpec& GetWantedSpec() const { return _want; }
	const SDL_AudioSpec& GetCurrentSpec() const { return _have; }
	SDL_AudioSpec& GetWantedSpec() { return _want; }
	SDL_AudioSpec& GetCurrentSpec() { return _have; }

	int GetCurrentFreq() const { return _have.freq; }
	bool IsRunning() const { return SDL_GetAudioDeviceStatus(_dev) == SDL_AUDIO_PLAYING; }


	void Lock() { SDL_LockAudioDevice(_dev); }
	void Unlock() { SDL_UnlockAudioDevice(_dev); }
	void Run() { SDL_PauseAudioDevice(_dev, 0); }
	void Pause() { SDL_PauseAudioDevice(_dev, 1); }

private:
	SDL_AudioSpec _want, _have;
	SDL_AudioDeviceID _dev;
	bool _initialized;
};















}











#endif
