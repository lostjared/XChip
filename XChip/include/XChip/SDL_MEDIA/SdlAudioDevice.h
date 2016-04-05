#ifndef _XCHIP_SDLAUDIODEVICE_H_
#define _XCHIP_SDLAUDIODEVICE_H_
#include <cstdint>
#include <SDL2/SDL_audio.h>



namespace xchip {


class SdlAudioDevice
{
public:
	SdlAudioDevice() noexcept = default;
	~SdlAudioDevice();
	SdlAudioDevice(const SdlAudioDevice&) = delete;
	SdlAudioDevice& operator=(const SdlAudioDevice&) = delete;

	bool Initialize() noexcept;
	void Dispose() noexcept;

	bool IsInitialized() const;
	float GetFreq() const;
	bool IsRunning() const;
	
	void SetFreq(const float hz);
	void SetCycleTime(const float hz);
	void SetLenght(const unsigned int len);
	void Run();
	void Pause();


private:
	template<class T>
	static void audio_callback(void* userdata, Uint8* stream, int len);

	SDL_AudioSpec _want, _have;
	SDL_AudioDeviceID _dev;
	float _cycleTime;
	float _freq;
	float _len;
	unsigned int _pos;
	int _amplitude;
	bool _initialized = false;
};








inline bool SdlAudioDevice::IsInitialized() const { return _initialized;  }
inline float SdlAudioDevice::GetFreq() const { return _freq * _have.freq; }
inline bool SdlAudioDevice::IsRunning() const {
	return SDL_GetAudioDeviceStatus(_dev) == SDL_AUDIO_PLAYING; 
}

inline void SdlAudioDevice::SetCycleTime(const float hz) { _cycleTime = _have.freq / hz; }
inline void SdlAudioDevice::Run() { SDL_PauseAudioDevice(_dev, 0); }
inline void SdlAudioDevice::Pause() { SDL_PauseAudioDevice(_dev, 1); _pos = 0; }



}











#endif
